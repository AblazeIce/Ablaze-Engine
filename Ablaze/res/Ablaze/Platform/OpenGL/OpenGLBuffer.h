#pragma once
#include "Ablaze/Render/Buffer.h"

namespace Ablaze {
	class OpenGLVertexBuffer :public VertexBuffer {
	public:
		OpenGLVertexBuffer(float* vertices,uint32_t size);
		virtual	~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual inline const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual inline void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};
	class OpenGLIndexBuffer :public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual	~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual inline uint32_t GetCount() const override { return m_Count; }

	private:
		uint32_t m_Count;
		uint32_t m_RendererID;
	};
}