#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Hazel {

	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();

		void Bind();
		void Unbind();

		void UniformMat4f(const std::string& name, const glm::mat4& matrix);
	private:
		uint32_t m_RenderID;
	};

}