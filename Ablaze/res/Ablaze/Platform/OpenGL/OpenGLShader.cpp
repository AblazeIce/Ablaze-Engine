#include "pch/AblazePch.h"
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
		std::string Source = ReadFile(filepath);
		auto shaderSource = PreProcess(Source);
		Compile(shaderSource);

	}
	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		std::unordered_map<GLenum, std::string>	sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}
	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}
	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}
	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
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
		std::string result;
		std::ifstream in(filepath, std::ios::in, std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);//����ַΪ�ļ���������ƫ�Ƶ�ַΪ0������ָ�붨λ���ļ�������
			result.resize(in.tellg());//�����ļ�����С�����ø�result�Ĵ�С
			in.seekg(0, std::ios::beg);//ָ�붨λ��ͷ��
			in.read(&result[0], result.size());//���ļ������ݶ�ȡ��result
			in.close();
		}
		else {
			ABLAZE_CORE_ERROR("Could not open file '{0}'", filepath);
		}
		return result;
	}
	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos)//"string::npos"means find failed
		{
			//--------\r:�س����ص��������ף�\n:���У��л�����һ��--------
			//--------\r\n:�س�����---------------------------------------
			//--------�ַ�����ÿһ�������\r\n�����ַ�
			size_t eol = source.find_first_of("\r\n", pos);//��posλ�ÿ�ʼ����\r\n�ַ���
			ABLAZE_CORE_ASSERTS(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;

			std::string type = source.substr(begin, eol - begin);//���ҳ�vertex��fragment
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
		m_RendererID = glCreateProgram();
		std::vector<GLenum> glShaderIDs(shaderSources.size());
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