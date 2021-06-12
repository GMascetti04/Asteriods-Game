#pragma once
#include "Engine.h"

//ship script will create the asteriods (for now)
class ShipScript : public xEngine::Script
{
public:
	virtual void onUpdate(DeltaTime& dt) override;
	virtual void onEvent(Event& event) override;
	virtual void onCreate() override;
	virtual void onDestroy() override;

	void BulletDied()
	{
   		activeBullets--;
		if (activeBullets < 0)
			activeBullets = 0;
	}

	Texture* asteriodTexture;
private:
	glm::vec2 velocity = {0,0};
	glm::vec2 lastPosition = { 0,0 };
	Entity Explosion;
	int activeBullets = 0;

	static bool CircleTriangleCollide(const glm::vec2& circleCenter, float circleRadius, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);
	static bool CircleLineCollide(const glm::vec2& circleCenter, float circleRadius, const glm::vec2& a, const glm::vec2& b);
};


