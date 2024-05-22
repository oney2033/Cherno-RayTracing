#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include"Walnut/Random.h"
#include"Walnut/Timer.h"
#include"Render.h"
#include"Camera.h"
#include"glm/gtc/type_ptr.hpp"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	ExampleLayer() :m_Camera(45.0f, 0.1f, 100.0f)
	{
	{
		Sphere sphere;
		sphere.Position = { 0.0f,0.0f,0.0f };
		sphere.Radius = 0.5f;
		sphere.Albedo = { 1.0f,0.0f,1.0f };
		m_Scene.Spheres.emplace_back(sphere);
	}

	{
		Sphere sphere;
		sphere.Position = { 1.0f,0.0f,-5.0f };
		sphere.Radius = 1.5f;
		sphere.Albedo = { 0.2f,0.3f,1.0f };
		m_Scene.Spheres.emplace_back(sphere);
	}
}

	virtual void OnUpdate(float ts) override
	{
		m_Camera.OnUpdate(ts);
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Setting");
		ImGui::Text("Last render: %.3fms", m_LastRenderTime);
		if (ImGui::Button("Render"))
		{
			Render();
		}
		ImGui::End();

		ImGui::Begin("Scene");

		for (size_t i = 0; i < m_Scene.Spheres.size(); i++)
		{
			ImGui::PushID(i);
			Sphere& sphere = m_Scene.Spheres[i];
			ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
			ImGui::DragFloat("Radiut", &sphere.Radius, 0.1f);
			ImGui::ColorEdit3("Position", glm::value_ptr(sphere.Albedo));
			ImGui::Separator();

			ImGui::PopID();
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;
		float aspectRation = m_ViewportWidth / (float)m_ViewportHeight;
		
		auto image = m_Renderer.GetFinalImage();
		if (image)
		{
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(),(float)image->GetHeight()},
				ImVec2(0,1),ImVec2(1,0));
		}

		ImGui::End();
		ImGui::PopStyleVar();
		Render();
	}
	void Render()
	{
		Timer timer;
		m_Renderer.OnResize(m_ViewportWidth,m_ViewportHeight);
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render(m_Scene,m_Camera);

		m_LastRenderTime = timer.ElapsedMillis();
	}

private:
	Renderer m_Renderer;
	Camera m_Camera;
	Scene m_Scene;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	float m_LastRenderTime;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}