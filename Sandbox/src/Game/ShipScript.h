#pragma once
#include "Engine.h"

//ship script will create the asteriods (for now)
class ShipScript : public xEngine::Script
{
public:
	virtual void onUpdate(DeltaTime& dt) override;
	virtual void onEvent(xEngine::Event& event) override;
	virtual void onCreate() override;
	virtual void onDestroy() override;
	virtual void onGuiUpdate() override;

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


	float angularSpeed = 250.0f;
	float mass = 2.0f;
	float maxSpeed = 850.0f;
	float thrustForce = 1000.0f;
	float frictionForce = 1000.0f;

	static bool CircleTriangleCollide(const glm::vec2& circleCenter, float circleRadius, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);
	static bool CircleLineCollide(const glm::vec2& circleCenter, float circleRadius, const glm::vec2& a, const glm::vec2& b);
};


