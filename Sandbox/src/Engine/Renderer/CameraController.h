#pragma once
#include "xpch.h"
#include "Camera.h"
#include "Engine/DeltaTime.h"
#include "Engine/Core/Event.h"
#include "Engine/Debug/Profiler.h"


class CameraController {
public:
	CameraController(Camera* cam);
	~CameraController() {}

	Camera* getCamera() { return m_Camera; }
	float& getCamHeight() { return m_CamHeight; }

	float& getAspectRatio() { return m_aspectRatio; }

	template<typename T>
	void setUpdateFunction(T& lambda)
	{
		m_onUpdateFunction = lambda;
	}

	template<typename T>
	void setEventFunction(T& lambda)
	{
		m_onEventFunction = lambda;
	}

	Camera* m_Camera;

	
	

private:
	friend class Scene;
	float m_CamHeight;
	float m_aspectRatio;
	std::function<void(DeltaTime&, CameraController&)> m_onUpdateFunction = [](DeltaTime& dt, CameraController& camController) {};
	std::function<void(xEngine::Event&, CameraController&)> m_onEventFunction = [](xEngine::Event& event, CameraController& camController) {};
};