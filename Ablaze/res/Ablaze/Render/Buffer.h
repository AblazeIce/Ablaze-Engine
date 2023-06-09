#pragma once
namespace Ablaze {

	enum class ShaderDataType
	{
		None = 0,Float, Float2, Float3, Float4,Int, Int2, Int3, Int4,Mat3,Mat4,Bool
	};
	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type)
		{
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 4*2;
		case ShaderDataType::Float3:	return 4*3;
		case ShaderDataType::Float4:	return 4*4;
		case ShaderDataType::Int:		return 4;
		case ShaderDataType::Int2:		return 4*2;
		case ShaderDataType::Int3:		return 4*3;
		case ShaderDataType::Int4:		return 4*4;
		case ShaderDataType::Mat3:		return 4*3*3;
		case ShaderDataType::Mat4:		return 4*4*4;
		case ShaderDataType::Bool:		return 1;
		}
		ABLAZE_CORE_ASSERTS(false, "Unknown ShaderDataType!");
		return 0;
	}
	
	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;
		BufferElement() = default;
		BufferElement(ShaderDataType type,const std::string& name,bool normalized=false)
			:Name(name),Type(type),Size(ShaderDataTypeSize(type)),Offset(0),Normalized(normalized){}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case Ablaze::ShaderDataType::Float:		return 1;
			case Ablaze::ShaderDataType::Float2:	return 2;
			case Ablaze::ShaderDataType::Float3:	return 3;
			case Ablaze::ShaderDataType::Float4:	return 4;
			case Ablaze::ShaderDataType::Int:		return 1;
			case Ablaze::ShaderDataType::Int2:		return 2;
			case Ablaze::ShaderDataType::Int3:		return 3;
			case Ablaze::ShaderDataType::Int4:		return 4;
			case Ablaze::ShaderDataType::Mat3:		return 9;
			case Ablaze::ShaderDataType::Mat4:		return 16;
			case Ablaze::ShaderDataType::Bool:		return 1;
			}
			ABLAZE_CORE_ASSERTS(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout {
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			:m_Elements(elements){
			CalculateOffsetAndStride();
		}
		inline uint32_t GetStride() const { return m_Stride; }
		inline	const std::vector<BufferElement>& GetElements()	const { return m_Elements; }

		inline std::vector<BufferElement>::const_iterator begin() const{ return m_Elements.begin(); }
		inline std::vector<BufferElement>::const_iterator end() const{ return  m_Elements.end(); }
	private:
		inline void CalculateOffsetAndStride() {
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements) {
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride;
	};

	class VertexBuffer {
	public:
		virtual ~VertexBuffer() = default;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual inline const BufferLayout& GetLayout() const = 0;
		virtual inline void SetLayout(const BufferLayout& layout) = 0;

		static std::shared_ptr<VertexBuffer> Create(uint32_t size);
		static std::shared_ptr<VertexBuffer> Create(float* vertices, uint32_t size);
	};
	class IndexBuffer {
	public:
		virtual ~IndexBuffer() = default;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual inline uint32_t GetCount() const = 0;

		static std::shared_ptr<IndexBuffer> Create(uint32_t* indices, uint32_t size);
	};
}