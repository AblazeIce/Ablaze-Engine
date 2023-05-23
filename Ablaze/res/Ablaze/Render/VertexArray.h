#pragma once
#include "Buffer.h"
#include <memory>
namespace Ablaze {
	class VertexArray {
	public:
		virtual ~VertexArray() = default;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		virtual inline const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual inline const std::shared_ptr<IndexBuffer>& GetIndexBuffers() const = 0;

		static VertexArray* Create();
	};
}