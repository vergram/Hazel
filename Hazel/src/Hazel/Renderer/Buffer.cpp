#include "hzpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazel {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::NONE:     HZ_CORE_ASSERT(false, "None RendererAPI is currently unsupported!"); return nullptr;
			case RendererAPI::OPENGL:   return new OpenGLVertexBuffer(vertices, size);
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::NONE:         HZ_CORE_ASSERT(false, "None RendererAPI is currently unsupported!"); return nullptr;
		case RendererAPI::OPENGL:       return new OpenGLIndexBuffer(indices, count);
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}