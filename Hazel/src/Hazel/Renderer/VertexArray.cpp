#include "hzpch.h"

#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hazel {

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::NONE:     HZ_CORE_ASSERT(false, "None RendererAPI is currently unsupported!"); return nullptr;
			case RendererAPI::OPENGL:   return new OpenGLVertexArray();
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}