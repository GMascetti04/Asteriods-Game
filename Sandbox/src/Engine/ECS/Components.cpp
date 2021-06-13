#include "xpch.h"
#include "Components.h"   
#include "Engine/Core/Application.h"

namespace xEngine {
    std::tuple<float, float> Script::getMousePosition()
    {
        CameraController* camController = m_entity.getScene()->getCameraController();
        float xPos = Input::percentMouseX() * camController->getAspectRatio() * camController->getCamHeight() + camController->getCamera()->getPosition().x - camController->getAspectRatio() * camController->getCamHeight() / 2.0f;
        float yPos = Input::percentMouseY() * camController->getCamHeight() + camController->getCamera()->getPosition().y - camController->getCamHeight() / 2.0f;
        return std::tuple<float, float>(xPos, yPos);
    }

    std::tuple<int, int> Script::worldToScreenSpace(float x, float y)
    {
        CameraController* camController = m_entity.getScene()->getCameraController();
        float xPos = (x + ((camController->getAspectRatio() * camController->getCamHeight()) / 2.0f) - camController->getCamera()->getPosition().x) / (float)(camController->getAspectRatio() * camController->getCamHeight()) * application::getWindow()->getWidth();
        float yPos = application::getWindow()->getHeight() - ((y + (camController->getCamHeight() / 2.0f) - camController->getCamera()->getPosition().y) / (float)(camController->getCamHeight()) * application::getWindow()->getHeight());
        return std::tuple<int, int>((int)xPos, (int)yPos);
    }
}

void xEngine::Components::ParticleSystem::Update(DeltaTime& dt)
{
    //make make this concurrent somehow
    for (int i = 0; i < maxParticles; i++)
    {
        Particle& particle = particles[i];
        if (!particle.alive)
            continue;

        float timeChange = dt.getSeconds<float>();

        //update position
        particle.position += particle.velocity * timeChange;

        //update velocity
        particle.velocity += particle.acceleration * timeChange;
        
        //update color
        particle.currentColor += particle.colorChange * timeChange;

        //update size
        particle.currentSize += particle.sizeChange * timeChange;

        //update lifespan
        particle.timeAlive += timeChange;
        if (particle.timeAlive >= particle.lifeSpan)
            particle.alive = false;

    }
}


