#pragma once

#include "Engine/Renderer/Texture.h"
#include "Engine/Utilities/Vec.h"
#include "Engine/DeltaTime.h"
#include "Entity.h"
#include "Engine/Input.h"
#include "Engine/Renderer/CameraController.h"
#include "Engine/Debug/Profiler.h"
#include "Engine/Debug/Logger.h"
#include "Engine/Audio/SoundBuffer.h"
#include "Engine/Audio/SoundSource.h"
/* The component structs in this file are modified 
*  and adapted from code in the Hazel Github repository 
*  (see Game.cpp citation [9])
*/

namespace xEngine {

	namespace Components
	{
		struct ScriptComponent;
	}

	class Script {

	public:
		Script() {}
		virtual ~Script() {}

		virtual void onCreate() {}
		virtual void onDestroy() {}

		virtual void onUpdate(DeltaTime& ts) {}
		virtual void onEvent(Event& event) {}
		virtual void onGuiUpdate() {}

	protected:

		template<typename Component>
		Component& getComponent()
		{
			return m_entity.getComponent<Component>();
		}

		template<typename Component>
		bool hasComponent()
		{
			return m_entity.hasComponent<Component>();
		}


		Entity& getEntity() { return m_entity; }

		//will get where in the world space the mouse currently is
		std::tuple<float, float> getMousePosition();

		//convert from world cordinates to actual screen cordinates
		std::tuple<int, int> worldToScreenSpace(float x, float y);

	private:
		friend class Scene;
		friend struct xEngine::Components::ScriptComponent;
		Entity m_entity;
	};
	

	namespace Components {

		struct TagComponent {
			TagComponent(const char* str) : m_tag(str) { }
			TagComponent() {}
			const char* m_tag;
		};


		struct TransformComponent {
			TransformComponent() {}
			TransformComponent(float x_, float y_, float rot_, float x_scal, float y_scal) : x(x_), y(y_), rot(rot_), x_scale(x_scal), y_scale(y_scal) {}
			void SetPosition(float x_, float y_) { x = x_; y = y_; }
			float x, y;
			float rot;
			float x_scale, y_scale;
		};

		struct Sprite2DComponent {
			Sprite2DComponent() {  }
			Sprite2DComponent(bool vis, const char* lay, int ord, SpriteTexture* tex, Vec4<float> col) : visible(vis), layer(lay), order(ord), texture(tex), color(col) {}
			bool visible;
			const char* layer;
			int order;
			//Texture* texture; 
			SpriteTexture* texture;
			Vec4<float> color;

		};


		
		struct ScriptComponent {
			ScriptComponent() {}

			::xEngine::Script* m_Script = nullptr;

			std::function<void(ScriptComponent&, Entity)> Constructor;
			std::function<void(ScriptComponent&)> Destructor;

			template<typename ScriptClass>
			void Attach(Entity entity) {
				Constructor = [](ScriptComponent& sc, Entity entity) { sc.m_Script = (Script*)new ScriptClass(); sc.m_Script->m_entity = entity; };
				Destructor = [](ScriptComponent& sc) {delete sc.m_Script; sc.m_Script = nullptr; };

				Constructor(*this, entity);
				m_Script->m_entity = entity;
				//m_Script->onCreate();
			}

			template<typename ScriptClass>
			ScriptClass* getScript()
			{
				return (ScriptClass*)(m_Script);
			}

		};


		/*
		* 	for (auto ent : ScriptEntities)
	{
		ScriptEntities.get<xEngine::Component::ScriptComponent>(ent).Constructor(ScriptEntities.get<xEngine::Component::ScriptComponent>(ent), {ent,scene});
		ScriptEntities.get<xEngine::Component::ScriptComponent>(ent).m_Script->m_entity = Entity(ent, scene);
		ScriptEntities.get<xEngine::Component::ScriptComponent>(ent).m_Script->onCreate();
	}
		*/

		


		struct SoundProperties {
			//loop, and stuff like that

			SoundProperties()
			{
				loop = false;
				playOnAwake = true; //change this to false later
			}
			bool loop;
			bool playOnAwake;
		};


		struct AudioSource
		{
			//the position will come from the trasnform if needed
			AudioSource() {  }
			~AudioSource() {

			}

			//the position in which sound was added
			size_t AddSound(SoundBuffer* sound)
			{
				for (int i = 0; i < m_Sounds.size(); i++)
				{
					if (m_Sounds[i]->getSoundBuffer() == sound)
					{
						Log_EngineWarn("Tried to add a sound that already exists");
						return -1;
					}
				}
				SoundSource* soundSource = new SoundSource();
				soundSource->AttachBuffer(sound);
				m_Sounds.push_back(soundSource);
				m_SoundProperties.push_back(SoundProperties());
				return m_SoundProperties.size() - 1;
			}

			SoundProperties& getProperties(size_t position) //position must be a valid index position
			{
				return m_SoundProperties[position];
			}

			size_t getSoundNumber()
			{
				return m_Sounds.size();
			}

			void PlaySound(size_t indexPosition)
			{
				if (m_Sounds.size() <= indexPosition)
					return;
				m_Sounds[indexPosition]->playSound();
			}

			void PauseSound(size_t indexPosition)
			{
				if (m_Sounds.size() <= indexPosition)
					return;
				m_Sounds[indexPosition]->pauseSound();
			}

			void StopSound(size_t indexPosition)
			{
				if (m_Sounds.size() <= indexPosition)
					return;
				m_Sounds[indexPosition]->stopSound();
			}

			void setLoop(size_t indexPosition, bool loop)
			{
				if (m_Sounds.size() <= indexPosition)
					return;
				m_Sounds[indexPosition]->setLoop(loop);
			}

			void removeSound(SoundBuffer* sound)
			{
				for (int i = 0; i < m_Sounds.size(); i++)
				{
					if (m_Sounds[i]->getSoundBuffer() == sound)
					{
						delete m_Sounds[i];
						m_Sounds.erase(m_Sounds.begin() + i);
						m_SoundProperties.erase(m_SoundProperties.begin() + i);
						return;
					}
				}
				Log_EngineWarn("Tried to remove a sound that doesn't exist!");
			}

			void removeSound(size_t indexPosition)
			{
				if (m_Sounds.size() <= indexPosition)
					return;
				m_SoundProperties.erase(m_SoundProperties.begin() + indexPosition);

			}




		private:
			friend class Scene;
			std::vector<SoundSource*> m_Sounds;
			std::vector<SoundProperties> m_SoundProperties;

		};



		//for now, all particles will be squares
		//add in later: particles can collide with other things
		struct ParticleSystem
		{
		public:
			ParticleSystem() 
			{
				
			}
			ParticleSystem(int maxNumber)
			{
				maxParticles = maxNumber;
				currentParticle = 0;
			}

			void EmitParticle(const glm::vec2& pos, const glm::vec2& vel, const glm::vec2& acc, const glm::vec4& startingColor,
				const glm::vec4& endingColor, const glm::vec2& startingSize, const glm::vec2& endingSize, float angle, float lifeSpan)
			{
				
				particles[currentParticle].position = pos;
				particles[currentParticle].velocity = vel;
				particles[currentParticle].acceleration = acc;
				particles[currentParticle].currentColor = startingColor;
				particles[currentParticle].colorChange = (endingColor - startingColor) / lifeSpan;
				particles[currentParticle].currentSize = startingSize;
				particles[currentParticle].sizeChange = (endingSize-startingSize) / lifeSpan;
				particles[currentParticle].angle = angle;
				particles[currentParticle].lifeSpan = lifeSpan;
				particles[currentParticle].timeAlive = 0.0f;
				particles[currentParticle].alive = true;

				currentParticle++;
				if (currentParticle >= maxParticles)
					currentParticle = 0;
			}

			   int maxParticles; //might make unsigned int OR const
			   void Init() //change this later
			   {
				   particles = new Particle[maxParticles];

			   }
			   void Destroy()
			   {
				   delete[] particles;
				   particles = nullptr;
			   }

			
		private:

			friend class Scene;
			friend class Renderer;

			

		
			void Update(DeltaTime& dt);

			//add in rotation later
			struct Particle {


				Particle()
				{
					alive = false;
				}
				glm::vec2 position;
				glm::vec2 velocity;
				glm::vec2 acceleration;

				glm::vec4 colorChange;
				glm::vec4 currentColor;

				glm::vec2 sizeChange;
				glm::vec2 currentSize;

				float angle;

				float lifeSpan;
				float timeAlive; //how long the particle has been alive
				bool alive;
			};

		private:

			Particle* particles; //call this in the scene start function or something like that
			int currentParticle;


		};

	}

	
}