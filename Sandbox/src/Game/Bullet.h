#pragma once
#include "Engine.h"
#include "ShipScript.h"
#include "Engine/ECS/Registry.h"
#include "AsteriodScript.h"
class BulletScript : public xEngine::Script
{
public:
	glm::vec2 velocity;
	ShipScript* shipScript;
	virtual void onCreate() override
	{
		
	}
	virtual void onUpdate(DeltaTime& dt) override
	{
		timeLeft -= dt.getSeconds<float>();
		if (timeLeft <= 0)
		{
			shipScript->BulletDied();
			xEngine::Script::getEntity().getScene()->destroyEntity(xEngine::Script::getEntity());
			return;	
		}
		xEngine::Component::TransformComponent& ts = getComponent<xEngine::Component::TransformComponent>();
		ts.x += dt.getSeconds<float>() * velocity.x;
		ts.y += dt.getSeconds<float>() * velocity.y;

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
		//see if any asteriods where hit
		auto Asteriods = getEntity().getScene()->getRegistry().View<xEngine::Component::TagComponent>();
		Registry &registry = getEntity().getScene()->getRegistry();
		//xEngine::Component::TransformComponent& ts = getComponent<xEngine::Component::TransformComponent>();
		for (auto ent : Asteriods)
		{
			if (registry.getComponent<xEngine::Component::TagComponent>({ ent,getEntity().getScene() }).m_tag = "Asteriod")
			{
				//std::cout << "check" << std::endl;
				xEngine::Component::TransformComponent& asteriodTransform = registry.getComponent<xEngine::Component::TransformComponent>(Entity(ent, getEntity().getScene()));
				if (CirclePointCollision({ ts.x,ts.y }, { asteriodTransform.x, asteriodTransform.y }, asteriodTransform.x_scale / 2))
				{
					shipScript->BulletDied();
					//registry.destroyEntity({ ent,getEntity().getScene() });
					Entity(ent, getEntity().getScene()).getComponent<xEngine::Component::ScriptComponent>().getScript<AsteriodScript>()->BulletHit();
					getEntity().getScene()->destroyEntity(getEntity());
					return;
				}
			}
		}
	}


private:
	float timeLeft = 1.5f;

	//true if collided
	static bool CirclePointCollision(const glm::vec2& point, const glm::vec2& circleCenter, float circleRadius)
	{
		//collided if the distance between the point and center is less than the radius
		if (glm::distance(point, circleCenter) <= circleRadius)
			return true;
		else
			return false;
	}
};

void createBullet(const glm::vec2& pos, const glm::vec2& velocity, Scene* scene, float angle, ShipScript* asteriodScript);




