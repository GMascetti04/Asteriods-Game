#pragma once
#include "Engine.h"

void createAsteriod(const glm::vec2& pos, const glm::vec2& velocity, Scene* scene, float angle, Texture* texture, int level);

class AsteriodScript : public xEngine::Script
{
public:
	virtual void onUpdate(DeltaTime& dt) override
	{
		xEngine::Components::TransformComponent& ts = getComponent<xEngine::Components::TransformComponent>();
		ts.x += velocity.x * dt.getSeconds<float>();
		ts.y += velocity.y * dt.getSeconds<float>();

		if (ts.x < -560)
		{
			ts.x = 560;
		}
		if (ts.x > 560)
		{
			ts.x = -560;
		}

		if (ts.y < -560)
		{
			ts.y = 560;
		}
		if (ts.y > 560)
		{
			ts.y = -560;
		} 
	}

	void BulletHit() {
		level--;
		if (level > 0)
		{
			//spawn 2 new asteriods in its place
			
			//std::cout << level << std::endl;
			xEngine::Components::TransformComponent& ts = getComponent<xEngine::Components::TransformComponent>();
			xEngine::Components::Sprite2DComponent& sprite = getComponent<xEngine::Components::Sprite2DComponent>();
			Texture* text = ((Texture*)sprite.texture);
			float x = ts.x;
			float y = ts.y;
			float velocities[] = { 150.0f,120.0f,100.0f };
			createAsteriod({ x, y }, Random::get().onUnitCircle() * velocities[level], getEntity().getScene(), 0.0f, text, level);
			createAsteriod({ x, y }, Random::get().onUnitCircle() * velocities[level], getEntity().getScene(), 0.0f, text, level);

		}
		getEntity().getScene()->destroyEntity(getEntity());
		
	}

	glm::vec2 velocity;
	int level = 3;
private:
	
};


//void SpawnAsteriod(); //will create a random asteriod
