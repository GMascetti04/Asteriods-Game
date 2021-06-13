/*
* AP Computer Science Principles Create Performance Task
* libraries used:
*	[1] GLFW - cross-platform window library
*	link: https://github.com/glfw/glfw
*	[2] GLEW - cross-platform extension loading library
*	link: https://github.com/nigels-com/glew
*	[3] OpenGL - cross-platform rendering API
*	link: https://www.opengl.org//
*	[4] imgui - graphical user interface library
*	link: https://github.com/ocornut/imgui
*	[5] entt - game programming library (used for the ECS)
*	link: https://github.com/skypjack/entt
*	[6] spdlog - logging library
*	link: https://github.com/gabime/spdlog
*	[7] glm - math library for OpenGL
*	link: https://github.com/g-truc/glm
*	[8] stb_image - image loader
*	link: https://github.com/nothings/stb
* 
* 
*   [9] Several parts of the project are simplified and adapted from
*       code in the following GitHub repository:
*   link: https://github.com/TheCherno/Hazel
*
*	citation:
*	 Chernikov, Yan. Hazel. Github, github.com/TheCherno/Hazel. Accessed 18 May 2021. 
*/
#include "xpch.h"
#include "Engine.h" 
#include "Engine/Entry.h"
#include "Engine/Debug/Profiler.h"

#include <AL\al.h>
#include <AL\alc.h>
Camera* myCam;
SpriteTexture* texture;
Texture* asteriodTexture;

#include "Engine/Audio/Audio.h"
#include "Engine/Audio/SoundSource.h"


#include "ShipScript.h"
#include "AsteriodScript.h"



void onStartup() { 
	xENGINE_PROFILE_FUNCTION();

	
	Renderer::SetClearColor({0.0f,0.0f,0.0f,0.0f});
	application::createScene("Chess Game");
	

	float m_aspectRatio = (float)application::getWindow()->getWidth() / (float)application::getWindow()->getHeight();
	
	float m_zoom = (float)application::getWindow()->getHeight();
	m_zoom = 1060;
	myCam = new Camera(-m_aspectRatio * m_zoom / 2.0f, m_aspectRatio * m_zoom / 2.0f, -m_zoom / 2.0f, m_zoom / 2.0f);


	Scene& myScene = application::getScene("Chess Game");


	myScene.useCamera(new CameraController(myCam));
	myScene.getCameraController()->setEventFunction([](xEngine::Event& event, CameraController& camController) {
		float padding = 0.1f;
		float x_len = application::getWindow()->getWidth() * (1 - 2 * padding);
		float y_len = application::getWindow()->getHeight() * (1 - 2 * padding);

		float length;

		camController.getAspectRatio() = (float)application::getWindow()->getWidth() / (float)application::getWindow()->getHeight();
		if (xEngine::Event::isEvent<xEngine::Events::WindowResizeEvent>(event) || xEngine::Event::isEvent<xEngine::Events::WindowMaximizeEvent>(event) || xEngine::Event::isEvent<xEngine::Events::WindowRestoreEvent>(event))
		{
			if (y_len < x_len) //height is smaller
			{
				length = y_len / application::getWindow()->getHeight();
				camController.getCamHeight() = 800.0f / length;
				camController.m_Camera->setCameraViewBounds(-camController.getCamHeight() * camController.getAspectRatio() / 2.0f, camController.getCamHeight() * camController.getAspectRatio() / 2.0f, -camController.getCamHeight() / 2.0f, camController.getCamHeight() / 2.0f);
			}
			else //width is smaller
			{
				length = x_len / application::getWindow()->getWidth();
				camController.getCamHeight() = 800.0f / length;
				camController.m_Camera->setCameraViewBounds(-camController.getCamHeight() / 2.0f, camController.getCamHeight() / 2.0f, -camController.getCamHeight() * (1 / camController.getAspectRatio()) / 2.0f, camController.getCamHeight() * (1 / camController.getAspectRatio()) / 2.0f);
				camController.getCamHeight() = camController.getCamHeight() * (1 / camController.getAspectRatio());
			}
		}


		//std::cout << camController.getCamHeight() << std::endl;
		});

	Audio::Load("res/shoot.wav");
	Audio::Load("res/explosion.wav");

	application::SetActiveScene("Chess Game");

	asteriodTexture = Texture::ImageTexture("res/textures/asteriod.png");
	myScene.createRenderLayer("", 1);
	//myScene.createRenderLayer("Pieces", 2);

	texture = Texture::ImageTexture("res/textures/Ship.png");
	Entity ship = myScene.createEntity();
	ship.addComponent<xEngine::Components::TransformComponent>(0,0,0,30,40);
	ship.addComponent<xEngine::Components::Sprite2DComponent>(true, "", 0, texture, Vec4<float>{1.0f,1.0f ,1.0f ,1.0f });

	//bool vis, const char* lay, int ord, SpriteTexture* tex, Vec4<float> col
	ship.addComponent<xEngine::Components::ScriptComponent>().Attach<ShipScript>(ship);
	((ShipScript*)(ship.getComponent<xEngine::Components::ScriptComponent>().m_Script))->asteriodTexture = asteriodTexture;
	ship.addComponent<xEngine::Components::ParticleSystem>(10);
	xEngine::Components::AudioSource& as = ship.addComponent<xEngine::Components::AudioSource>();
	as.AddSound(Audio::Get("res/shoot.wav"));
	as.getProperties(0).playOnAwake = false;
	as.getProperties(0).loop = false;
	as.AddSound(Audio::Get("res/explosion.wav"));
	as.getProperties(1).playOnAwake = false;
	as.getProperties(1).loop = false;
	
	

	Scene::LoadScene(&myScene);
}

void onShutdown() {
	
	Scene& myScene = application::getScene("Chess Game");
	myScene.deleteCamera();

	myScene.removeRenderLayer("");

	
	Scene::CloseScene(&myScene);
	application::deleteScene("Chess Game");

	delete myCam;
	delete texture;
	delete asteriodTexture;
}


ApplicationProperties createApplication() {
	return { "Asteriods",1000,1000 };
}