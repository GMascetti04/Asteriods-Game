#include "xpch.h"
#include "Bullet.h"
#include "AsteriodScript.h"
#include "ShipScript.h"

void createBullet(const glm::vec2& pos, const glm::vec2& velocity, Scene* scene, float angle, ShipScript* asteriodScript)
{

	Entity ent = scene->createEntity();
	ent.addComponent<xEngine::Components::TransformComponent>(pos.x, pos.y, angle, 5.0f, 5.0f);
	ent.addComponent<xEngine::Components::Sprite2DComponent>(true, "", 0, nullptr, Vec4<float>{1.0f, 0.0f, 0.0f, 1.0f});
	//bool vis, const char* lay, int ord, SpriteTexture* tex, Vec4<float> col
	ent.addComponent<xEngine::Components::ScriptComponent>().Attach<BulletScript>(ent);
	ent.getComponent<xEngine::Components::ScriptComponent>().getScript<BulletScript>()->velocity = velocity;
	ent.getComponent<xEngine::Components::ScriptComponent>().getScript<BulletScript>()->shipScript = asteriodScript;}

