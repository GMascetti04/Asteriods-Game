#include "xpch.h"
#include "ShipScript.h"
#include "Engine.h"
#include "Bullet.h"
#include "AsteriodScript.h"


void ShipScript::onUpdate(DeltaTime& dt)
{
	xEngine::Components::TransformComponent& ts = getComponent<xEngine::Components::TransformComponent>();


	if (Input::keyPressed(codes::KeyCode::left))
	{
		ts.rot += this->angularSpeed * dt.getSeconds<float>();
	}

	if (Input::keyPressed(codes::KeyCode::right))
	{
		ts.rot -= this->angularSpeed * dt.getSeconds<float>();
	}

	if(getComponent<xEngine::Components::Sprite2DComponent>().visible)
	if (Input::keyPressed(codes::KeyCode::left_control))
	{
		//x-direction
		float theta = ts.rot + 90.0f;
		float particleSpeed = 40;

		//Impulse Momentum Theorem
		velocity.x += dt.getSeconds<float>() * thrustForce * glm::cos(glm::radians(theta)) / mass;
		velocity.y += dt.getSeconds<float>() * thrustForce * glm::sin(glm::radians(theta)) / mass;

		float speed = glm::sqrt(glm::pow(velocity.x, 2) + glm::pow(velocity.y, 2));
		if (speed > maxSpeed)
		{
			speed = maxSpeed;
		}
		float angle = std::atan2f(velocity.y, velocity.x);
		velocity = { speed * glm::cos(angle), speed * glm::sin(angle) };


		xEngine::Components::ParticleSystem& ps = getComponent<xEngine::Components::ParticleSystem>();
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

		float speed = glm::sqrt(glm::pow(velocity.x,2)+glm::pow(velocity.y,2));
		speed -= frictionForce * dt.getSeconds<float>() / mass;
		if (speed < 0)
			speed = 0;
		float angle = std::atan2f(velocity.y, velocity.x);
		velocity = { speed*glm::cos(angle), speed*glm::sin(angle) };
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
	auto Asteriods = getEntity().getScene()->getRegistry().View<xEngine::Components::TagComponent>();
	Registry& registry = getEntity().getScene()->getRegistry();
	float gamma = glm::atan(ts.x_scale / ts.y_scale); //in radians
	float L = 0.5f * glm::sqrt(ts.y_scale * ts.y_scale + ts.x_scale * ts.x_scale);
	//std::cout << "a: " <<  glm::cos(glm::radians(theta)) << std::endl;
	glm::vec2 a = { ts.x + ts.y_scale / 2.0f * glm::cos(glm::radians(theta)),ts.y + ts.y_scale / 2.0f * glm::sin(glm::radians(theta)) };
	glm::vec2 b = { ts.x - L * glm::sin(glm::radians(90.0f) - glm::radians(theta) - gamma),ts.y - L * glm::cos(glm::radians(90.0f) - glm::radians(theta) - gamma) };
	glm::vec2 c = { ts.x - L * glm::cos(glm::radians(theta) - gamma),ts.y - L * glm::sin(glm::radians(theta) - gamma) };

	//check if ship hit asteriod
	if(getComponent<xEngine::Components::Sprite2DComponent>().visible)
	for (auto ent : Asteriods)
	{
		
		if (registry.getComponent<xEngine::Components::TagComponent>({ ent,getEntity().getScene() }).m_tag = "Asteriod")
		{
			//std::cout << "check" << std::endl;
			xEngine::Components::TransformComponent& asteriodTransform = registry.getComponent<xEngine::Components::TransformComponent>(Entity(ent, getEntity().getScene()));
		
			if (CircleTriangleCollide({ asteriodTransform.x,asteriodTransform.y }, asteriodTransform.y_scale / 2.0f, a, b, c))
			{
				//ship hit an asteriod
				getComponent<xEngine::Components::Sprite2DComponent>().visible = false;

				float speed = 500;
				for (int i = 0; i < 40; i++)
				{
					getComponent<xEngine::Components::AudioSource>().PlaySound(1);
					Explosion.getComponent<xEngine::Components::ParticleSystem>().EmitParticle(
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

void ShipScript::onEvent(xEngine::Event& event)
{
	float BulletSpeed = 950;
	xEngine::Components::TransformComponent& ts = getComponent<xEngine::Components::TransformComponent>();
	if (xEngine::Event::checkProperty<xEngine::Events::KeyPressEvent>(event, &xEngine::Events::KeyPressEvent::get_keyCode, codes::KeyCode::space))
	{
		if (activeBullets < 4)
		{
			float theta = ts.rot + 90.0f;
			glm::vec2 a = { ts.x + ts.y_scale / 2.0f * glm::cos(glm::radians(theta)),ts.y + ts.y_scale / 2.0f * glm::sin(glm::radians(theta)) };

			getComponent<xEngine::Components::AudioSource>().PlaySound(0);
			createBullet(a, { BulletSpeed * glm::cos(glm::radians(ts.rot + 90.0f)),BulletSpeed * glm::sin(glm::radians(ts.rot + 90.0f)) }, Script::getEntity().getScene(), ts.rot, this);
			activeBullets++;
		}
		
	}
	//if (xEngine::Event::checkProperty<xEngine::Events::KeyPressEvent>(event, &xEngine::Events::KeyPressEvent::get_keyCode, codes::KeyCode::P))
	//{
	//	createAsteriod({0.0,0.0}, {100,100}, Script::getEntity().getScene(), 0.0f, asteriodTexture, 3);
	//}

	if (xEngine::Event::checkProperty<xEngine::Events::KeyPressEvent>(event, &xEngine::Events::KeyPressEvent::get_keyCode, codes::KeyCode::A))
	{
		getComponent<xEngine::Components::Sprite2DComponent>().visible = true;
	}
}

void ShipScript::onCreate()
{
	Explosion = getEntity().getScene()->createEntity();
	Explosion.addComponent<xEngine::Components::ParticleSystem>(40).Init();
	NewLevel();
}

void ShipScript::onDestroy()
{
	Explosion.getComponent<xEngine::Components::ParticleSystem>().Destroy();
	getEntity().getScene()->destroyEntity(Explosion); 
}

void ShipScript::onGuiUpdate()
{
#if 0
	ImGui::Begin("Physics Properties");
	ImGui::SliderFloat("angular speed", (float*)&(this->angularSpeed), 0, 360);
	ImGui::SliderFloat("mass", (float*)&(this->mass), 1.00f, 20.0f);
	ImGui::SliderFloat("max Speed", (float*)&(this->maxSpeed), 100, 1000);
	ImGui::SliderFloat("thrust force", (float*)&(this->thrustForce), 100, 2000);
	ImGui::SliderFloat("frictional force", (float*)&(this->frictionForce), 100, 2000);
	ImGui::End();
#endif
	

	std::string scoreString = std::string("Score: ") + std::to_string(score);
	ImGui::SetNextWindowPos({300,0});
	ImGui::SetNextWindowSize({ 500,100 });
	ImGui::Begin("564", (bool*)0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiColorEditFlags_NoInputs | ImGuiWindowFlags_NoBackground);
	ImGui::SetWindowFontScale(2.0f);
	
	ImGui::Text(scoreString.c_str());
	ImGui::End();
}

void ShipScript::NewLevel()
{
	
		score += 1000;
		createAsteriod({ 0, 560 }, Random::get().onUnitCircle() * 141.42f, getEntity().getScene(), Random::get().FloatRange(0.0f,360.0f), asteriodTexture, 3);
		createAsteriod({ -560, 0 }, Random::get().onUnitCircle() * 141.42f, getEntity().getScene(), Random::get().FloatRange(0.0f, 360.0f), asteriodTexture, 3);
		createAsteriod({ 0, -560 }, Random::get().onUnitCircle() * 141.42f, getEntity().getScene(), Random::get().FloatRange(0.0f, 360.0f), asteriodTexture, 3);
		createAsteriod({ 560, 0 }, Random::get().onUnitCircle() * 141.42f, getEntity().getScene(), Random::get().FloatRange(0.0f, 360.0f), asteriodTexture, 3);
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
