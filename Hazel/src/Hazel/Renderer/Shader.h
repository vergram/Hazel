#pragma once

#include <string>
#include <glm/glm.hpp>


namespace Hazel {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& vec3) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& vec4) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& mat4) = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);

	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, Ref<Shader> shader);
		void Add(Ref<Shader> shader);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);
		Ref<Shader> Load(const std::string& filepath);

		Ref<Shader> Get(const std::string& name);

		bool Exist(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;

	};

}