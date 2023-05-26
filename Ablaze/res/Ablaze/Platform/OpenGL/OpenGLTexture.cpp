#include "pch/AblazePch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"
#include "glad/glad.h"

namespace Ablaze {
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_Path(path)
	{
		//���ļ��м���ͼƬ��ߵ���Ϣ
		int width, height, channels;
		//��תͼ��
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		ABLAZE_CORE_ASSERTS(data, "Failed to load image");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat=0, dataformat=0;
		if (channels == 4) {
			internalFormat = GL_RGBA8;
			dataformat = GL_RGBA;
		}
		else if (channels == 3) {
			internalFormat = GL_RGB8;
			dataformat = GL_RGB;
		}
		ABLAZE_CORE_ASSERTS(internalFormat & dataformat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		//ͼ�������ͼ��ռ�ӳ�䵽֡����ͼ��ռ�
		// (ӳ����Ҫ���¹�������ͼ��,�����ͻ����Ӧ�õ�������ϵ�ͼ��ʧ��),
		//��ʱ�Ϳ���glTexParmeteri()������ȷ����ΰ���������ӳ�������
		//ָ����С���Թ���
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//ָ���Ŵ����Թ���
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RendererID, 0, 0,0, m_Width, m_Height, dataformat, GL_UNSIGNED_BYTE,data);

		stbi_image_free(data);
	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}
	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}