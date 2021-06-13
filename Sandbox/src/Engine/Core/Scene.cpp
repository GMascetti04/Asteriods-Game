#include "xpch.h"
#include "Scene.h"
#include "Engine/Renderer/Renderer.h"
#include <Engine/ECS/Components.h>
#include <Engine/Gui/GuiLayer.h>
#include "Engine/Debug/Profiler.h"


#include "imgui.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <Windows.h>


#if defined(DEBUG) || defined(RELEASE)
extern void engineGuiUpdate();
#endif

bool Scene::pushEvent(xEngine::Event& event)
{
	xENGINE_PROFILE_FUNCTION();
	m_cameraController->m_onEventFunction(event, *m_cameraController);
	auto script_entities = m_registry.View<xEngine::Components::ScriptComponent>();

	for (auto ent : script_entities)
	{
		script_entities.get<xEngine::Components::ScriptComponent>(ent).m_Script->onEvent(event);

	}

	return event.m_handled;
}

void Scene::onUpdate(DeltaTime& deltaTime)
{ 
	xENGINE_PROFILE_FUNCTION();

	Renderer::StartFrame();
	Renderer::Clear();
	auto ScriptEntities = m_registry.View<xEngine::Components::ScriptComponent>();
	//std::cout << "----" << std::endl;
	for (auto entity : ScriptEntities)
	{
		if (std::find(deleteQueue.begin(), deleteQueue.end(), entity) != deleteQueue.end()) //its in here
		{
			continue;
		}
		//std::cout << (int)entity << std::endl;
		ScriptEntities.get<xEngine::Components::ScriptComponent>(entity).m_Script->onUpdate(deltaTime);
	}
	
	m_cameraController->m_onUpdateFunction(deltaTime, *m_cameraController);


	
	
	auto renderable_entities = m_registry.Group<xEngine::Components::Sprite2DComponent, xEngine::Components::TransformComponent>();

	renderable_entities.sort<xEngine::Components::Sprite2DComponent>([this](const auto& lhs, const auto& rhs) {
		if (this->getRenderLayerPriority((lhs.layer)) != this->getRenderLayerPriority(rhs.layer))
			return this->getRenderLayerPriority(lhs.layer) < this->getRenderLayerPriority(rhs.layer);
		return lhs.order < rhs.order;
		});

	auto ParticleEntities = m_registry.View<xEngine::Components::ParticleSystem>();
	for (auto ent : ParticleEntities)
	{
		xEngine::Components::ParticleSystem& ps = ParticleEntities.get<xEngine::Components::ParticleSystem>(ent);
		ps.Update(deltaTime);

		Renderer::DrawParticleSystem(ps);

	}



	for (auto it = renderable_entities.begin(); it != renderable_entities.end(); it++)
	{
		xEngine::Components::TransformComponent& transform = renderable_entities.get<xEngine::Components::TransformComponent>(*it);
		xEngine::Components::Sprite2DComponent& sprite = renderable_entities.get<xEngine::Components::Sprite2DComponent>(*it);
		if (sprite.visible)
			Renderer::DrawEntity(sprite, transform);
	}

	
	//////////////////////////////   DRAW PARTICLES HERE


	Renderer::EndFrame();

	///////////////////////////////


	GuiLayer::Begin();
	
	for (auto entity : ScriptEntities)
		ScriptEntities.get<xEngine::Components::ScriptComponent>(entity).m_Script->onGuiUpdate();

#if defined(DEBUG) || defined(RELEASE)
	engineGuiUpdate();
#endif
	GuiLayer::End();


	//delete all the particles
	for (int i = 0; i < deleteQueue.size(); i++)
	{
		m_registry.destroyEntity({ deleteQueue[i],this });
	}
	deleteQueue.clear();
}


void Scene::useCamera(CameraController* cam)
{
	m_cameraController = cam;
}

void Scene::deleteCamera()
{
	if (m_cameraController)
	{
		delete m_cameraController;
		m_cameraController = nullptr;
	}
}

void Scene::LoadScene(Scene* scene)
{
	xENGINE_PROFILE_FUNCTION();
	//auto ScriptEntities = scene->m_registry.View<xEngine::Component::ScriptComponent>();
	//for (auto ent : ScriptEntities) //plan on removeing this
	//{
	//	ScriptEntities.get<xEngine::Component::ScriptComponent>(ent).Constructor(ScriptEntities.get<xEngine::Component::ScriptComponent>(ent), {ent,scene});
	//	ScriptEntities.get<xEngine::Component::ScriptComponent>(ent).m_Script->m_entity = Entity(ent, scene);
	//	ScriptEntities.get<xEngine::Component::ScriptComponent>(ent).m_Script->onCreate();
	//}

	//TEMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	auto AudioEntities = scene->m_registry.View<xEngine::Components::AudioSource>();
	for (auto ent : AudioEntities)
	{
		xEngine::Components::AudioSource& audioSource = AudioEntities.get<xEngine::Components::AudioSource>(ent);
		size_t count = audioSource.getSoundNumber();
		for (size_t i = 0; i < count; i++)
		{
			if (audioSource.getProperties(i).loop)
			{
				audioSource.setLoop(i, true);
			}
			if (audioSource.getProperties(i).playOnAwake)
			{
				audioSource.PlaySound(i);
			}
		}
		
	}

	auto ParticleEntities = scene->m_registry.View<xEngine::Components::ParticleSystem>();
	for (auto ent : ParticleEntities)
	{
		xEngine::Components::ParticleSystem& ps = ParticleEntities.get<xEngine::Components::ParticleSystem>(ent);
		ps.Init();
	}
}

void Scene::CloseScene(Scene* scene)
{
	xENGINE_PROFILE_FUNCTION();
	auto view = scene->m_registry.View<xEngine::Components::ScriptComponent>();
	for (auto entity : view)
	{
		
		view.get<xEngine::Components::ScriptComponent>(entity).m_Script->onDestroy();
		view.get<xEngine::Components::ScriptComponent>(entity).Destructor(view.get<xEngine::Components::ScriptComponent>(entity));
	}
	
	auto Audios = scene->m_registry.View<xEngine::Components::AudioSource>();
	for (auto ent : Audios)
	{
		xEngine::Components::AudioSource& source = Audios.get<xEngine::Components::AudioSource>(ent);
		for (int i = 0; i < source.m_Sounds.size(); i++)
		{
			delete source.m_Sounds[i];
		}
	}

	/*	for (int i = 0; i < m_Sounds.size(); i++)
		{
			delete m_Sounds[i];
		}*/

	auto ParticleEntities = scene->m_registry.View<xEngine::Components::ParticleSystem>();
	for (auto ent : ParticleEntities)
	{
		xEngine::Components::ParticleSystem& ps = ParticleEntities.get<xEngine::Components::ParticleSystem>(ent);
		ps.Destroy();
	}
}


Entity Scene::createEntity()
{
	return this->m_registry.createEntity();
}

void Scene::destroyEntity(Entity ent)
{
	//if it had a script, call the destrucor
	//if (m_registry.hasComponent<xEngine::Component::ScriptComponent>(ent))
	//{
	//	std::cout << "ew script" << std::endl;
	//	m_registry.getComponent<xEngine::Component::ScriptComponent>(ent).Destructor(m_registry.getComponent<xEngine::Component::ScriptComponent>(ent));
	//}
	//
	//m_registry.destroyEntity(ent);
	deleteQueue.push_back(ent.getID());
}



void Scene::createRenderLayer(const std::string& name, int priority)
{
	xENGINE_PROFILE_FUNCTION();
	for (int i = 0; i < m_RenderLayerNames.size(); i++)
	{
		if (m_RenderLayerNames[i] == name)
		{
		
			Log_EngineWarn("RenderLayer in Scene aleady has this name!");
			return;

		}
	}

	m_RenderLayerNames.push_back(name);
	m_RenderLayerNumber.push_back(priority);
}

void Scene::removeRenderLayer(const std::string& name)
{
	xENGINE_PROFILE_FUNCTION();
	for (int i = 0; i < m_RenderLayerNames.size(); i++)
	{
		if (m_RenderLayerNames[i] == name)
		{
			
			m_RenderLayerNames.erase(m_RenderLayerNames.begin() + i);
			m_RenderLayerNumber.erase(m_RenderLayerNumber.begin() + i);
			return;

		}
	}

	Log_EngineWarn("Specified Render Layer Does Not Exist! - ' {0} '",name);
	return;
}

void Scene::changeRenderLayer(const std::string& name, int priority)
{
	xENGINE_PROFILE_FUNCTION();
	for (int i = 0; i < m_RenderLayerNames.size(); i++)
	{
		if (m_RenderLayerNames[i] == name)
		{
			m_RenderLayerNumber[i] = priority;
			return;

		}
	}

	Log_EngineWarn("RenderLayer ' {0} ' does not exist", name);
}

int Scene::getRenderLayerPriority(const std::string& name)
{
	xENGINE_PROFILE_FUNCTION();
	for (int i = 0; i < m_RenderLayerNames.size(); i++)
	{
		if (m_RenderLayerNames[i] == name)
		{
			return m_RenderLayerNumber[i];
			
		}
	}

	Log_EngineWarn("RenderLayer does not exist - It cannot have a priority: -1 returned");
	return -1;
}
