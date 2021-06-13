#include "xpch.h"
#include "AsteriodScript.h"

void createAsteriod(const glm::vec2& pos, const glm::vec2& velocity, Scene* scene, float angle, Texture* texture, int level)
{
	//std::cout <<  "before" << texture << std::endl;
	Entity ent = scene->createEntity();
	float sizes[] = {60,80,100};
	
	ent.addComponent<xEngine::Component::TransformComponent>(pos.x, pos.y, angle, sizes[level-1], sizes[level-1]);
	ent.addComponent<xEngine::Component::Sprite2DComponent>(true, "", 0, texture, Vec4<float>{1.0f, 1.0f, 1.0f, 1.0f});
	//bool vis, const char* lay, int ord, SpriteTexture* tex, Vec4<float> col
	ent.addComponent<xEngine::Component::ScriptComponent>().Attach<AsteriodScript>(ent);
	ent.getComponent<xEngine::Component::ScriptComponent>().getScript<AsteriodScript>()->velocity = velocity;
	ent.getComponent<xEngine::Component::ScriptComponent>().getScript<AsteriodScript>()->level = level;
	ent.addComponent<xEngine::Component::TagComponent>().m_tag = "Asteriod";
	//std::cout <<"after: " << texture << std::endl;
}
