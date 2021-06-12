#include "xpch.h"
#include "ShipScript.h"
#include "Engine.h"
#include "Bullet.h"
#include "AsteriodScript.h"


void ShipScript::onUpdate(DeltaTime& dt)
{
	float speed = 100.0f;
	float angular = 250;
	xEngine::Component::TransformComponent& ts = getComponent<xEngine::Component::TransformComponent>();
	float Force = 1000;
	float mass = 2;
	float friction = 200;

	if (Input::keyPressed(codes::KeyCode::left))
	{
		ts.rot += angular * dt.getSeconds<float>();
	}

	if (Input::keyPressed(codes::KeyCode::right))
	{
		ts.rot -= angular * dt.getSeconds<float>();
	}

	if(getComponent<xEngine::Component::Sprite2DComponent>().visible)
	if (Input::keyPressed(codes::KeyCode::left_control))
	{
		//x-direction
		float theta = ts.rot + 90.0f;
		float particleSpeed = 40;

		//Impulse Momentum Theorem
		velocity.x += dt.getSeconds<float>() * Force * glm::cos(glm::radians(theta)) / mass;
		velocity.y += dt.getSeconds<float>() * Force * glm::sin(glm::radians(theta)) / mass;

		xEngine::Component::ParticleSystem& ps = getComponent<xEngine::Component::ParticleSystem>();
		//std::cout << ts.x - ts.y_scale / 2.0f * glm::cos(glm::radians(theta)) << std::endl;
		ps.EmitParticle(
			{ ts.x - ts.y_scale / 2.0f * glm::cos(glm::radians(theta)), ts.y - ts.y_scale / 2.0f * glm::sin(glm::radians(theta)) },                                          //position
			{ -particleSpeed * glm::cos(glm::radians(theta)),        //velocity
			-particleSpeed * glm::sin(glm::radians(theta)) }, { 0,0 }, //acceleration
			{ 240 / 255.0f,115 / 255.0f,26 / 255.0f,1.0f },                                   //starting color
			{ 255 / 255.0f,255 / 255.0f,0 / 255.0f,0.0f },                                   //ending color
			{ 20,20 },                                                 //starting size
			{ 10,10 },                                                 //ending size
			ts.rot + 45.0f,                                              //angle
			0.07f                                                     //life span
		);
	}
	else
	{
		velocity = { 0,0 };
	}

	lastPosition = { ts.x, ts.y };

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
	std::cout << std::fixed;

	float theta = ts.rot + 90.0f;
	//std::cout << "theta: " << theta << std::endl;
	//check for collision
	auto Asteriods = getEntity().getScene()->getRegistry().View<xEngine::Component::TagComponent>();
	Registry& registry = getEntity().getScene()->getRegistry();
	float gamma = glm::atan(ts.x_scale / ts.y_scale); //in radians
	float L = 0.5f * glm::sqrt(ts.y_scale * ts.y_scale + ts.x_scale * ts.x_scale);
	//std::cout << "a: " <<  glm::cos(glm::radians(theta)) << std::endl;
	glm::vec2 a = { ts.x + ts.y_scale / 2.0f * glm::cos(glm::radians(theta)),ts.y + ts.y_scale / 2.0f * glm::sin(glm::radians(theta)) };
	glm::vec2 b = { ts.x - L * glm::sin(glm::radians(90.0f) - glm::radians(theta) - gamma),ts.y - L * glm::cos(glm::radians(90.0f) - glm::radians(theta) - gamma) };
	glm::vec2 c = { ts.x - L * glm::cos(glm::radians(theta) - gamma),ts.y - L * glm::sin(glm::radians(theta) - gamma) };
	//xEngine::Component::TransformComponent& ts = getComponent<xEngine::Component::TransformComponent>();
	//Renderer::DrawQuad({ a.x, a.y }, { 5,5 }, 0.0, { 1.0f,0.0f,0.0f,1.0f });
	//Renderer::DrawQuad({ b.x, b.y }, { 5,5 }, 0.0, { 1.0f,0.0f,0.0f,1.0f });
	//Renderer::DrawQuad({ c.x, c.y }, { 5,5 }, 0.0, { 1.0f,0.0f,0.0f,1.0f });



	//check if ship hit asteriod
	if(getComponent<xEngine::Component::Sprite2DComponent>().visible)
	for (auto ent : Asteriods)
	{
		
		if (registry.getComponent<xEngine::Component::TagComponent>({ ent,getEntity().getScene() }).m_tag = "Asteriod")
		{
			//std::cout << "check" << std::endl;
			xEngine::Component::TransformComponent& asteriodTransform = registry.getComponent<xEngine::Component::TransformComponent>(Entity(ent, getEntity().getScene()));
		
			if (CircleTriangleCollide({ asteriodTransform.x,asteriodTransform.y }, asteriodTransform.y_scale / 2.0f, a, b, c))
			{
				//ship hit an asteriod
				getComponent<xEngine::Component::Sprite2DComponent>().visible = false;

				float speed = 500;
				for (int i = 0; i < 40; i++)
				{
					Explosion.getComponent<xEngine::Component::ParticleSystem>().EmitParticle(
						{ ts.x - ts.y_scale / 2.0f * glm::cos(glm::radians(theta)), ts.y - ts.y_scale / 2.0f * glm::sin(glm::radians(theta)) },                                          //position
						Random::get().onUnitCircle()*speed,       //velocity
						{ 0,0 }, //acceleration
						{ 255/255.0f, 251/255.0f, 166/255.0f,1.0f },                                   //starting color
						{ 255/255.0f, 157/255.0f, 10/255.0f,0.8f },                                   //ending color
						{ 20,20 },                                                 //starting size
						{ 10,10 },                                                 //ending size
						Random::get().FloatRange(0.0f,360.0f),                                              //angle
						0.5f
					);
				}
			}
		}
	}



	//std::cout << "x: " << ts.x << " y: " << ts.y << std::endl;
}

void ShipScript::onEvent(Event& event)
{
	float BulletSpeed = 600;
	xEngine::Component::TransformComponent& ts = getComponent<xEngine::Component::TransformComponent>();
	if (Event::checkProperty<KeyPressEvent>(event, &KeyPressEvent::get_keyCode, codes::KeyCode::space))
	{
		if (activeBullets < 4)
		{
			getComponent<xEngine::Component::AudioSource>().PlaySound(0);
			createBullet({ ts.x, ts.y }, { BulletSpeed * glm::cos(glm::radians(ts.rot + 90.0f)),BulletSpeed * glm::sin(glm::radians(ts.rot + 90.0f)) }, Script::getEntity().getScene(), ts.rot, this);
			activeBullets++;
		}
		
	}
	if (Event::checkProperty<KeyPressEvent>(event, &KeyPressEvent::get_keyCode, codes::KeyCode::P))
	{
		createAsteriod({0.0,0.0}, {100,100}, Script::getEntity().getScene(), 0.0f, asteriodTexture);
	}

	if (Event::checkProperty<KeyPressEvent>(event, &KeyPressEvent::get_keyCode, codes::KeyCode::A))
	{
		getComponent<xEngine::Component::Sprite2DComponent>().visible = true;
	}
}

void ShipScript::onCreate()
{
	Explosion = getEntity().getScene()->createEntity();
	Explosion.addComponent<xEngine::Component::ParticleSystem>(40).Init();
}

void ShipScript::onDestroy()
{
	Explosion.getComponent<xEngine::Component::ParticleSystem>().Destroy();
	getEntity().getScene()->destroyEntity(Explosion);
}

bool ShipScript::CircleTriangleCollide(const glm::vec2& circleCenter, float circleRadius, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
{
	if(CircleLineCollide(circleCenter, circleRadius, a, b))
		return true;
	if (CircleLineCollide(circleCenter, circleRadius, b, c))
		return true;
	if (CircleLineCollide(circleCenter, circleRadius, a, c))
		return true;
	return false;
}

bool ShipScript::CircleLineCollide(const glm::vec2& circleCenter, float circleRadius, const glm::vec2& start, const glm::vec2& end)
{
	glm::vec2 X = start - circleCenter;
	glm::vec2 Y = end - start;
	float a = glm::dot(Y, Y);
	float b = 2.0f * glm::dot(X, Y);
	float c = glm::dot(X, X) - circleRadius * circleRadius;
	float disc = b * b - 4.0f * a * c;
	if (disc < 0.0f)
	{
		return false;
	}
	else
	{
		disc = glm::sqrt(disc);
		float tMin = (-b - disc) / (2.0f * a);
		float tMax = (-b + disc) / (2.0f * a);
		if (tMin >= 0.0f && tMin <= 1.0f)
		{
			return true;
		}
		if (tMax >= 0.0f && tMax <= 1.0f)
		{
			return true;
		}
		return false;
	}
}
