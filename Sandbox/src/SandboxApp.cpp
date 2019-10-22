#include <Hazel.h>
#include "imgui/imgui.h"

#include <Platform/OpenGL/OpenGLShader.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

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
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
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
				uniform mat4 u_TransformMatrix;

				void main()
				{
					v_Position = a_Position;
					v_Color = a_Color;
					gl_Position = u_ViewProjectionMatrix * u_TransformMatrix * vec4(a_Position, 1.0);
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

		m_Shader.reset(Hazel::Shader::Create(vertexSource, fragmentSource));

		std::string flatColorShaderVertexSource = R"(
				#version 330 core
				
				layout(location = 0) in vec3 a_Position;

				out vec3 v_Position;

				uniform mat4 u_ViewProjectionMatrix;
				uniform mat4 u_TransformMatrix;

				void main()
				{
					v_Position = a_Position;
					gl_Position = u_ViewProjectionMatrix * u_TransformMatrix * vec4(a_Position, 1.0);
				}
		)";

		std::string flatColorShaderFragmentSource = R"(
				#version 330 core
				
				out vec4 color;

				in vec3 v_Position;

				uniform vec3 u_color;

				void main()
				{
					color = vec4(u_color, 1.0);
				}
		)";

		m_FlatColorShader.reset(Hazel::Shader::Create(flatColorShaderVertexSource, flatColorShaderFragmentSource));
	}

	void OnUpdate(Hazel::Timestep ts) override
	{
		if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
			m_CameraPos.y += m_CameraMoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
			m_CameraPos.y -= m_CameraMoveSpeed * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
			m_CameraPos.x += m_CameraMoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
			m_CameraPos.x -= m_CameraMoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
			m_CameraRotation += m_CameraRotateSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
			m_CameraRotation -= m_CameraRotateSpeed * ts;

		m_Camera.SetPosition(m_CameraPos);
		m_Camera.SetRotation(m_CameraRotation);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		Hazel::Renderer::BeginScene(m_Camera);

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_color", m_SquareColor);


		for (int y = -10; y < 10; y++)
		{
			for (int x = -10; x < 10; x++)
			{
				auto pos = glm::vec3(x * .11f, y * .11f, 0.0f);
				auto transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		Hazel::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override
	{
	}

private:
	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;

	std::shared_ptr<Hazel::Shader> m_FlatColorShader;
	std::shared_ptr<Hazel::VertexArray> m_SquareVA;

	Hazel::OrthographicCamera m_Camera;
	
	glm::vec3 m_CameraPos = { 0.0f, 0.0f, 0.0f };
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotateSpeed = 180.0f;
	float m_CameraRotation = 0.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.3f };
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