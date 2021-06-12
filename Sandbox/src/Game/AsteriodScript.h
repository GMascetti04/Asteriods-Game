#pragma once
#include "Engine.h"
class AsteriodScript : public xEngine::Script
{
public:
	virtual void onUpdate(DeltaTime& dt) override
	{
		xEngine::Component::TransformComponent& ts = getComponent<xEngine::Component::TransformComponent>();
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

	glm::vec2 velocity;
private:
};

void createAsteriod(const glm::vec2& pos, const glm::vec2& velocity, Scene* scene, float angle, Texture* texture);
