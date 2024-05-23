#include"Render.h"
#include"Walnut/Random.h"
#include"Walnut/Timer.h"
#include"Camera.h"

namespace Utils
{
	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);
		
		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}
}

Renderer::HitPayload Renderer::TraceRay( const Ray& ray)
{
	
	//glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
	//glm::vec3 rayOrigin(0.0f,0.0f,2.0f);
	//float radius = 0.5f;
	//rayDirection = glm::normalize(rayDirection);

	//at^2 + bt + c = 0
	//(bx^2 + by^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
	// a = ray origin
	// b = ray direction
	// r = radius
	// t = hit distance
	//float a = rayDirection.x * rayDirection.x + rayDirection.y * rayDirection.y + rayDirection.z * rayDirection.z;
	//const Sphere* closestSphere = nullptr;

	int closestSphere = -1;

	//float hitDistance = FLT_MAX;
	float hitDistance = std::numeric_limits<float>::max();

	for (size_t i = 0; i < m_ActiveScene->Spheres.size(); i++)
	{
		const Sphere& sphere = m_ActiveScene->Spheres[i];

		glm::vec3 origin = ray.Origin - sphere.Position;

		float a = glm::dot(ray.Direction, ray.Direction);
		float b = 2.0f * glm::dot(origin, ray.Direction);
		//float c = glm::dot(origin, origin) - radius * radius;

		float c = glm::dot(origin, origin) - sphere.Radius * sphere.Radius;

		//b^2 - 4ac
		float discriminant = b * b - 4.0f * a * c;
		if (discriminant < 0.0f)
			continue;

		//(-b +- sqrt(discriminant)) / 2a
		float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
		float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);
		if (closestT > 0.0f && closestT < hitDistance)
		{
			hitDistance = closestT;
			closestSphere = (int)i;
		}
	}

	if (closestSphere < 0)
		return Miss(ray);

	return ClosestHit(ray, hitDistance, closestSphere);

	
}

void Renderer::Render(const Scene& scene, const Camera& camera)
{
	m_ActiveScene = &scene;
	m_ActiveCamera = &camera;

	//Ray ray;
	//ray.Origin = camera.GetPosition();
	//render every pixel
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			//float aspectRation = m_FinalImage->GetWidth() /(float) m_FinalImage->GetHeight();
			//glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(),(float)y / (float)m_FinalImage->GetHeight() };
			//coord = coord * 2.0f - 1.0f; //-1 -> 1
			//ray.Direction = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];
			//coord.x *= aspectRation;
			glm::vec4 color = PerPixel(x,y);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y*m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);
		}
	}
	m_FinalImage->SetData(m_ImageData);
}

glm::vec4 Renderer::PerPixel(uint32_t x, uint32_t y)
{
	Ray ray;
	ray.Origin = m_ActiveCamera->GetPosition();
	ray.Direction = m_ActiveCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];

	glm::vec3 color(0.0f);
	float multiplier = 1.0f;
	int bounces = 2;
	for (int i = 0; i < bounces; i++)
	{

	Renderer::HitPayload payload = TraceRay(ray);
		if (payload.HitDistance < 0.0f)
		{
			glm::vec3 skyColor = glm::vec3(0.0f, 0.0f, 0.f);
			color += skyColor * multiplier;
			break;
		}

		glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));//light
		float lightIntensity = glm::max(glm::dot(payload.WorldNormal, -lightDir), 0.0f);// = cos(angle);

		const Sphere& sphere = m_ActiveScene->Spheres[payload.ObjectIndex];
		glm::vec3 sphereColor = sphere.Albedo;
		//glm::vec3 sphereColor(1, 0, 1);
		sphereColor *= lightIntensity;
		//sphereColor = (normal * 0.5f + 0.5f)*d;	
		//return glm::vec4(sphereColor, 1.0f);
		//return glm::vec4(hitpoint, 1.0f);
		color +=sphereColor * multiplier;
		multiplier *= 0.7f;
		ray.Origin = payload.WorldPosition + payload.WorldNormal * 0.0001f;
		ray.Direction = glm::reflect(ray.Direction, payload.WorldNormal);
	}
	return glm::vec4(color, 1.0f);
}


Renderer::HitPayload Renderer::ClosestHit(const Ray& ray, float hitDistance, int objectIndex)
{
	Renderer::HitPayload payload;
	payload.HitDistance = hitDistance;
	payload.ObjectIndex = objectIndex;

	const Sphere& closestSphere = m_ActiveScene->Spheres[objectIndex];
	//glm::vec3 h0 = rayOrigin + rayDirection * t0;
	glm::vec3 origin = ray.Origin - closestSphere.Position;

	payload.WorldPosition = origin + ray.Direction * hitDistance;

	payload.WorldNormal = glm::normalize(payload.WorldPosition); //hitpoint - shpereCenter
	payload.WorldPosition += closestSphere.Position;

	return payload;
}

Renderer::HitPayload Renderer::Miss(const Ray& ray)
{
	Renderer::HitPayload payload;
	payload.HitDistance = -1.0f;
	return payload;
}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		//No resize necessary
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;
		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}
	delete[]m_ImageData;
	m_ImageData = new uint32_t[width * height];
}

