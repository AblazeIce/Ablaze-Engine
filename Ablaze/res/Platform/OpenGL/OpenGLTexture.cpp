#include "AblazePch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

namespace Ablaze {
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		:m_Width(width),m_Height(height)
	{
		ABLAZE_PROFILE_FUNCTION()

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		//图象从纹理图象空间映射到帧缓冲图象空间
		// (映射需要重新构造纹理图像,这样就会造成应用到多边形上的图像失真),
		//这时就可用glTexParmeteri()函数来确定如何把纹理象素映射成像素
		//指定缩小线性过滤
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//指定放大线性过滤
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_Path(path)
	{
		ABLAZE_PROFILE_FUNCTION()

		//从文件中加载图片宽高等信息
		int width, height, channels;
		//翻转图像
		stbi_set_flip_vertically_on_load(1);

		//ABLAZE_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std:string&) ");
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
		m_InternalFormat = internalFormat;
		m_DataFormat = dataformat;
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

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0,0, m_Width, m_Height, dataformat, GL_UNSIGNED_BYTE,data);

		stbi_image_free(data);
	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		ABLAZE_PROFILE_FUNCTION()

		glDeleteTextures(1, &m_RendererID);
	}
	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		ABLAZE_PROFILE_FUNCTION()

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		ABLAZE_CORE_ASSERTS(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}
	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		ABLAZE_PROFILE_FUNCTION()

		glBindTextureUnit(slot, m_RendererID);
	}
}