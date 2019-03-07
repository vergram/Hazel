#include <Hazel.h>
#include "imgui/imgui.h"

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer():Layer("Example")
	{
	}

	void OnUpdate() override
	{
		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
		{
			HZ_TRACE("Polling Key: {0}", (char)HZ_KEY_A);
		}
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello world!");
		ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override
	{
		if (event.GetEventType() == Hazel::EventType::KeyPressed)
		{
			Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
			if(e.GetKeyCode() == HZ_KEY_A)
				HZ_TRACE("Event Key: {0}", (char)e.GetKeyCode());
		}
	}
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}