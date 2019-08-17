#include "hzpch.h"
#include "Renderer.h"

namespace Hazel {

	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData();

	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->UniformMat4f("u_ViewProjectionMatrix", s_SceneData->ViewProjectionMatrix);
		shader->UniformMat4f("u_TransformMatrix", transform);

		RenderCommand::DrawIndexed(vertexArray);
	}

}