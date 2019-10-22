#include "hzpch.h"
#include "Shader.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel {

	Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:     HZ_CORE_ASSERT(false, "None RendererAPI is currently unsupported!"); return nullptr;
		case RendererAPI::API::OPENGL:   return new OpenGLShader(vertexSource, fragmentSource);
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}