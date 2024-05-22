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

glm::vec4 Renderer::TraceRay(const Scene& scene, const Ray& ray)
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

	if (scene.Spheres.size() == 0)
		return glm::vec4(0, 0, 0, 1);

	const Sphere* closestSphere = nullptr;
	//float hitDistance = FLT_MAX;
	float hitDistance = std::numeric_limits<float>::max();

	for (const Sphere& sphere : scene.Spheres)
	{
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
		if (closestT < hitDistance)
		{
			hitDistance = closestT;
			closestSphere = &sphere;
		}
	}

	if (closestSphere == nullptr)
		return glm::vec4( 0.0,0.0f,0.f,1.0f );

	//glm::vec3 h0 = rayOrigin + rayDirection * t0;
	glm::vec3 origin = ray.Origin - closestSphere->Position;

	glm::vec3 hitpoint = origin + ray.Direction * hitDistance;
	
	glm::vec3 normal = glm::normalize(hitpoint); //hitpoint - shpereCenter
	glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));//light
	float d = glm::max(glm::dot(normal, -lightDir), 0.0f);// = cos(angle);
	glm::vec3 sphereColor = closestSphere->Albedo;
	//glm::vec3 sphereColor(1, 0, 1);
	sphereColor *= d;
	//sphereColor = (normal * 0.5f + 0.5f)*d;	
	return glm::vec4(sphereColor, 1.0f);
	//return glm::vec4(hitpoint, 1.0f);
}

void Renderer::Render(const Scene& scene, const Camera& camera)
{
	Ray ray;
	ray.Origin = camera.GetPosition();
	//render every pixel
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			//float aspectRation = m_FinalImage->GetWidth() /(float) m_FinalImage->GetHeight();
			//glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(),(float)y / (float)m_FinalImage->GetHeight() };
			//coord = coord * 2.0f - 1.0f; //-1 -> 1
			ray.Direction = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];
			//coord.x *= aspectRation;
			glm::vec4 color = TraceRay(scene,ray);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y*m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);
		}
	}
	m_FinalImage->SetData(m_ImageData);
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

