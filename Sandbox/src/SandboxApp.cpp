#include <Hazel.h>
#include "imgui/imgui.h"

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer():Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		m_VertexArray.reset(Hazel::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.1f, 0.3f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.2f, 0.3f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f
		};
		std::shared_ptr<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

		Hazel::BufferLayout layout = {
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Hazel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Hazel::VertexArray::Create());
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};
		std::shared_ptr<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Hazel::ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSource = R"(
				#version 330 core
				
				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec4 a_Color;

				out vec3 v_Position;
				out vec4 v_Color;

				uniform mat4 u_ViewProjectionMatrix;

				void main()
				{
					v_Position = a_Position;
					v_Color = a_Color;
					gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
				}
		)";

		std::string fragmentSource = R"(
				#version 330 core
				
				out vec4 color;

				in vec3 v_Position;
				in vec4 v_Color;

				void main()
				{
					color = vec4(v_Position * 0.5 + 0.5, 1.0);
					color = v_Color;
				}
		)";

		m_Shader.reset(new Hazel::Shader(vertexSource, fragmentSource));

		std::string blueVertexSource = R"(
				#version 330 core
				
				layout(location = 0) in vec3 a_Position;

				out vec3 v_Position;

				uniform mat4 u_ViewProjectionMatrix;

				void main()
				{
					v_Position = a_Position;
					gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
				}
		)";

		std::string blueFragmentSource = R"(
				#version 330 core
				
				out vec4 color;

				in vec3 v_Position;

				void main()
				{
					color = vec4(0.2, 0.3, 0.8, 1.0);
				}
		)";

		m_BlueShader.reset(new Hazel::Shader(blueVertexSource, blueFragmentSource));
	}

	void OnUpdate() override
	{
		if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
			m_CameraPos.y += m_CameraMoveSpeed;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
			m_CameraPos.y -= m_CameraMoveSpeed;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
			m_CameraPos.x += m_CameraMoveSpeed;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
			m_CameraPos.x -= m_CameraMoveSpeed;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
			m_CameraRotation += m_CameraRotateSpeed;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
			m_CameraRotation -= m_CameraRotateSpeed;



		m_Camera.SetPosition(m_CameraPos);
		m_Camera.SetRotation(m_CameraRotation);

		Hazel::Renderer::BeginScene(m_Camera);

		Hazel::Renderer::Submit(m_BlueShader, m_SquareVA);
		Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		Hazel::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello world!");
		ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override
	{
	}

private:
	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;

	std::shared_ptr<Hazel::Shader> m_BlueShader;
	std::shared_ptr<Hazel::VertexArray> m_SquareVA;

	Hazel::OrthographicCamera m_Camera;
	
	glm::vec3 m_CameraPos = { 0.0f, 0.0f, 0.0f };
	float m_CameraMoveSpeed = 0.1f;

	float m_CameraRotateSpeed = 2.0f;
	float m_CameraRotation = 0.0f;
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