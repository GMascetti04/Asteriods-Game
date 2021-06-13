#pragma once
#include "xpch.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ShaderProgram.h"
#include "Engine/Utilities/Vec.h"
#include "Engine/Renderer/Texture.h"
#include "Camera.h"
#include <Engine/ECS/Components.h>
#include "glm/gtc/matrix_transform.hpp"
#include "Engine/Debug/Profiler.h"
#include "Engine/Renderer/FrameBuffer.h"
/*
* The Renderer class is adapted and simplified from code in the
*  Hazel Github repository (see Game.cpp citation [9])
*/
class Renderer {
public:
	Renderer() = delete;
	~Renderer() = delete;
	Renderer(const Renderer&) = delete;

	static void Init();
	static void Shutdown();

	static void DrawParticleSystem(xEngine::Components::ParticleSystem& ps);
	static void DrawEntity(xEngine::Components::Sprite2DComponent& sprite, xEngine::Components::TransformComponent& transform);

	static void StartFrame();
	static void EndFrame();
	static int getDrawCallsPerFrame();
	
	static void SetModeParticle();
	static void SetModeNormal(); //change name

	static ShaderProgram* getParticleProgram(); //temp!!

	constexpr static unsigned int MAX_INDEX_LENGTH = 10000 * 6;
	constexpr static int MAX_VERTEX_LENGTH = 10000 * 4;

	static glm::vec4 GetClearColor();
	static void SetClearColor(const glm::vec4& color);
	static void Clear();

	static void DrawQuad(const Vec2<float>& center, const Vec2<float>& dimensions, float angle, Vec4<float> color);
	static void DrawQuad(const Vec2<float>& center, const Vec2<float>& dimensions, float angle, SpriteTexture* texture, Vec4<float> color);

private:
	
	

	static glm::vec4 s_clear_color;
};