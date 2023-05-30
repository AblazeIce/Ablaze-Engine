#include "AblazePch.h"
#include "Renderer2D.h"

#include "Ablaze/Render/VertexArray.h"
#include "Ablaze/Render/Shader.h"
#include "Ablaze/Render/RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>
namespace Ablaze {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		//float TexIndex;
		//float TilingFactor;

		// Editor-only
		//int EntityID;
	};
	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;

		// Editor-only
		int EntityID;
	};

	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		// Editor-only
		int EntityID;
	};
	struct  Renderer2DData
	{
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		//static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		std::shared_ptr<VertexArray> QuadVertexArray;
		std::shared_ptr<VertexBuffer> QuadVertexBuffer;
		std::shared_ptr<Shader> QuadShader;
		std::shared_ptr<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		//std::shared_ptr<VertexArray> CircleVertexArray;
		//std::shared_ptr<VertexBuffer> CircleVertexBuffer;
		//std::shared_ptr<Shader> CircleShader;

		//std::shared_ptr<VertexArray> LineVertexArray;
		//std::shared_ptr<VertexBuffer> LineVertexBuffer;
		//std::shared_ptr<Shader> LineShader;

		//uint32_t QuadIndexCount = 0;
		//QuadVertex* QuadVertexBufferBase = nullptr;
		//QuadVertex* QuadVertexBufferPtr = nullptr;

		//uint32_t CircleIndexCount = 0;
		//CircleVertex* CircleVertexBufferBase = nullptr;
		//CircleVertex* CircleVertexBufferPtr = nullptr;

		//uint32_t LineVertexCount = 0;
		//LineVertex* LineVertexBufferBase = nullptr;
		//LineVertex* LineVertexBufferPtr = nullptr;

		//float LineWidth = 2.0f;

		//std::array<std::shared_ptr<Texture2D>, MaxTextureSlots> TextureSlots;
		//uint32_t TextureSlotIndex = 1; // 0 = white texture

		//glm::vec4 QuadVertexPositions[4];

		////Renderer2D::Statistics Stats;

		//struct CameraData
		//{
		//	glm::mat4 ViewProjection;
		//};
		//CameraData CameraBuffer;
		////std::shared_ptr<UniformBuffer> CameraUniformBuffer;
	};

	static Renderer2DData s_Data;
	void Renderer2D::Init()
	{
		ABLAZE_PROFILE_FUNCTION()

		s_Data.QuadVertexArray = VertexArray::Create();
		//-----------只设置顶点缓存数组，不传入数据--------
		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		//-----------在EndScene()传入数据---------------

		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
		//----------顶点缓存池---------
		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
		//顶点索引数组
		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)//每次前进6，相当于前进一个Quad索引数
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		std::shared_ptr<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data.QuadShader=Shader::Create("Assets/Shaders/Texture_1.hlsl");
		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetInt("u_Texture", 0);
	}
	void Renderer2D::Shutdown()
	{
		ABLAZE_PROFILE_FUNCTION()
	}
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		ABLAZE_PROFILE_FUNCTION()

		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		//---------------开始场景---------------------------------------
		//---------------s_Data.QuadVertexBufferPtr指向顶点缓存池头部-----
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}
	void Renderer2D::EndScene()
	{
		ABLAZE_PROFILE_FUNCTION()

		//---------------结束场景---------------------------------------
		//----------记录传入顶点缓存池的数据的长度----------
		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		//----------传入数据到顶点缓存池的数据顶点数组----------
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}
	void Renderer2D::Flush()
	{
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		ABLAZE_PROFILE_FUNCTION();

		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		s_Data.QuadShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data.WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.QuadShader->SetMat4("u_Transform", transform);

		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		ABLAZE_PROFILE_FUNCTION();

		s_Data.QuadShader->SetFloat4("u_Color", tintColor);
		s_Data.QuadShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.QuadShader->SetMat4("u_Transform", transform);
			 
		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);

	}
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		ABLAZE_PROFILE_FUNCTION();

		s_Data.QuadShader->SetFloat4("u_Color", color);
		s_Data.QuadShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data.WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			*glm::rotate(glm::mat4(1.0f),rotation,glm::vec3(0,0,1))
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.QuadShader->SetMat4("u_Transform", transform);
			  
		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
	}
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		ABLAZE_PROFILE_FUNCTION();
		s_Data.QuadShader->SetFloat4("u_Color", tintColor);
		s_Data.QuadShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 0, 1))
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.QuadShader->SetMat4("u_Transform", transform);
			  
		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
	}
}