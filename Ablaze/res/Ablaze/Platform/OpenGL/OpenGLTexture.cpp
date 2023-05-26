#include "pch/AblazePch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"
#include "glad/glad.h"

namespace Ablaze {
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_Path(path)
	{
		//从文件中加载图片宽高等信息
		int width, height, channels;
		//翻转图像
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

		//图象从纹理图象空间映射到帧缓冲图象空间
		// (映射需要重新构造纹理图像,这样就会造成应用到多边形上的图像失真),
		//这时就可用glTexParmeteri()函数来确定如何把纹理象素映射成像素
		//指定缩小线性过滤
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//指定放大线性过滤
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