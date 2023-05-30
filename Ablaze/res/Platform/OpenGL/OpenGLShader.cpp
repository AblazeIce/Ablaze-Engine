#include "AblazePch.h"
#include "OpenGLShader.h"
#include <fstream>
#include "glad/glad.h"

#include "glm/gtc/type_ptr.hpp"
namespace Ablaze {
	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex") {
			return GL_VERTEX_SHADER;
		}
		if ((type == "fragment" || type == "pixel"))
			return GL_FRAGMENT_SHADER;
		ABLAZE_CORE_ASSERTS(false, "Unknown shader type!");
		return 0;
	}
	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		ABLAZE_PROFILE_FUNCTION()

		std::string Source = ReadFile(filepath);
		auto shaderSource = PreProcess(Source);
		Compile(shaderSource);

		//-------Assets/Shaders/Texture.hlsl-------
		//-------获得Texture字符串-----------------
		auto lastSlash=filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash+1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);


	}
	OpenGLShader::OpenGLShader(const std::string& name,const std::string& vertexSrc, const std::string& fragmentSrc)
		:m_Name(name)
	{
		ABLAZE_PROFILE_FUNCTION()

		std::unordered_map<GLenum, std::string>	sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}
	OpenGLShader::~OpenGLShader()
	{
		ABLAZE_PROFILE_FUNCTION()

		glDeleteProgram(m_RendererID);
	}
	void OpenGLShader::Bind() const
	{
		ABLAZE_PROFILE_FUNCTION()

		glUseProgram(m_RendererID);
	}
	void OpenGLShader::Unbind() const
	{
		ABLAZE_PROFILE_FUNCTION()

		glUseProgram(0);
	}
	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		ABLAZE_PROFILE_FUNCTION()
		UploadUniformInt(name, value);
	}
	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		ABLAZE_PROFILE_FUNCTION()
		UploadUniformFloat(name, value);
	}
	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		ABLAZE_PROFILE_FUNCTION()

		UploadUniformFloat3(name, value);
	}
	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		ABLAZE_PROFILE_FUNCTION()

		UploadUniformFloat4(name, value);

	}
	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value) {
		ABLAZE_PROFILE_FUNCTION()		
		UploadUniformMat4(name, value);

	}
	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}
	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}
	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}
	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y,value.z);
	}
	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z,value.w);
	}
	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		ABLAZE_PROFILE_FUNCTION()

		std::string result;
		std::ifstream in(filepath, std::ios::in, std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);//基地址为文件结束处，偏移地址为0，于是指针定位在文件结束处
			result.resize(in.tellg());//返回文件流大小并设置给result的大小
			in.seekg(0, std::ios::beg);//指针定位到头部
			in.read(&result[0], result.size());//将文件流内容读取给result
			in.close();
		}
		else {
			ABLAZE_CORE_ERROR("Could not open file '{0}'", filepath);
		}
		return result;
	}
	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		ABLAZE_PROFILE_FUNCTION()

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos)//"string::npos"means find failed
		{
			//--------\r:回车，回到本行行首；\n:换行，切换到下一行--------
			//--------\r\n:回车换行---------------------------------------
			//--------字符流的每一行最后都有\r\n两个字符
			size_t eol = source.find_first_of("\r\n", pos);//在pos位置开始查找\r\n字符串
			ABLAZE_CORE_ASSERTS(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;

			std::string type = source.substr(begin, eol - begin);//查找出vertex或fragment
			ABLAZE_CORE_ASSERTS(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = 
				source.substr(nextLinePos,
					pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
			/*pos = source.find(typeToken, eol);
			shaderSources[ShaderTypeFromString(type)] =
				source.substr(eol,
					pos - (eol == std::string::npos ? source.size() - 1 : eol));*/

		}
		return shaderSources;
	}
	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		ABLAZE_PROFILE_FUNCTION()

		m_RendererID = glCreateProgram();
		ABLAZE_CORE_ASSERTS(shaderSources.size() <= 2, "We only support 2 shaders for now");
		std::array<GLenum,2> glShaderIDs;
		int glShaderIDIndex = 0;
		for (auto& kv : shaderSources) {
			GLenum type = kv.first;
			const std::string& source = kv.second;
			GLuint shader = glCreateShader(type);
			const GLchar* sourceChar = (const GLchar*)source.c_str();
			glShaderSource(shader, 1, &sourceChar, 0);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.
				ABLAZE_CORE_ERROR("{0}", infoLog.data());
				ABLAZE_CORE_ASSERTS(GL_FALSE, "Shader compilation failure!");
				break;
			}
			glAttachShader(m_RendererID, shader);
			glShaderIDs[glShaderIDIndex++]=shader;
		}
		GLuint program = m_RendererID;
		glLinkProgram(program);
		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for(auto& id:glShaderIDs)
				glDeleteShader(id);

			// Use the infoLog as you see fit.
			ABLAZE_CORE_ERROR("{0}", infoLog.data());
			ABLAZE_CORE_ASSERTS(GL_FALSE, "Shader link failure!");
			return;
		}

		// Always detach shaders after a successful link.
		for(auto& id:glShaderIDs)
			glDetachShader(program, id);
	}
}