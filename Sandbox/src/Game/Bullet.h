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
		xEngine::Components::TransformComponent& ts = getComponent<xEngine::Components::TransformComponent>();
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
		auto Asteriods = getEntity().getScene()->getRegistry().View<xEngine::Components::TagComponent>();
		Registry &registry = getEntity().getScene()->getRegistry();
		//xEngine::Component::TransformComponent& ts = getComponent<xEngine::Component::TransformComponent>();
		for (auto ent : Asteriods)
		{
			if (registry.getComponent<xEngine::Components::TagComponent>({ ent,getEntity().getScene() }).m_tag = "Asteriod")
			{
				//std::cout << "check" << std::endl;
				xEngine::Components::TransformComponent& asteriodTransform = registry.getComponent<xEngine::Components::TransformComponent>(Entity(ent, getEntity().getScene()));
				if (CirclePointCollision({ ts.x,ts.y }, { asteriodTransform.x, asteriodTransform.y }, asteriodTransform.x_scale / 2))
				{
					shipScript->BulletDied();
					shipScript->score += 100;
					//registry.destroyEntity({ ent,getEntity().getScene() });
					Entity(ent, getEntity().getScene()).getComponent<xEngine::Components::ScriptComponent>().getScript<AsteriodScript>()->BulletHit();

					//get all the entities that have an asteriod script
					auto l = getEntity().getScene()->getRegistry().View<xEngine::Components::TagComponent>();
					int i = 0;
					for (auto ent : l)
					{
						if (l.get<xEngine::Components::TagComponent>(ent).m_tag == "Asteriod")
						{
							i++;
						}
					}
					if (i == 1)
						shipScript->NewLevel();
					
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




