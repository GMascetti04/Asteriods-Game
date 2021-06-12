#include "xpch.h"
#include "AsteriodScript.h"

void createAsteriod(const glm::vec2& pos, const glm::vec2& velocity, Scene* scene, float angle, Texture* texture)
{
	Entity ent = scene->createEntity();
	ent.addComponent<xEngine::Component::TransformComponent>(pos.x, pos.y, angle, 80, 80);
	ent.addComponent<xEngine::Component::Sprite2DComponent>(true, "", 0, texture, Vec4<float>{1.0f, 1.0f, 1.0f, 1.0f});
	//bool vis, const char* lay, int ord, SpriteTexture* tex, Vec4<float> col
	ent.addComponent<xEngine::Component::ScriptComponent>().Attach<AsteriodScript>(ent);
	ent.getComponent<xEngine::Component::ScriptComponent>().getScript<AsteriodScript>()->velocity = velocity;
	ent.addComponent<xEngine::Component::TagComponent>().m_tag = "Asteriod";
}
