#include "xpch.h"
#include "CameraController.h"
#include "Engine/Core/Application.h"
#include "Engine/Debug/Profiler.h"


CameraController::CameraController(Camera* cam) {
	m_aspectRatio = (float)application::getWindow()->getWidth() / (float)application::getWindow()->getHeight();
	m_CamHeight = (float)application::getWindow()->getHeight();
	m_Camera = cam;
}



