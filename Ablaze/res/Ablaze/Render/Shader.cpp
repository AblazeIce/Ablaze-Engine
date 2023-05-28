#include "AblazePch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
namespace Ablaze {
	std::shared_ptr<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::API::None:
			ABLAZE_CORE_ASSERTS(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(name,vertexSrc,fragmentSrc);
		}
		ABLAZE_CORE_ASSERTS(false, "Unknown RendererAPI!");
		return nullptr;
	}
	std::shared_ptr<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::API::None:
			ABLAZE_CORE_ASSERTS(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(filepath);
		}
		ABLAZE_CORE_ASSERTS(false, "Unknown RendererAPI!");
		return nullptr;
	}
	void ShaderLibrary::Add(const std::string& name, const std::shared_ptr<Shader>& shader)
	{
		ABLAZE_CORE_ASSERTS(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}
	void ShaderLibrary::Add(const std::shared_ptr<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}
	std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}
	std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}
	std::shared_ptr<Shader> ShaderLibrary::Get(const std::string& name)
	{
		ABLAZE_CORE_ASSERTS(Exists(name), "Shader not found!");
		return m_Shaders[name];
	}
	bool ShaderLibrary::Exists(const std::string& name) const {
		return m_Shaders.find(name) != m_Shaders.end();
	}

}