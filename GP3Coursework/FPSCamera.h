#pragma once
#include "Component.h"
#include "Camera.h"

class FPSCamera : public Component
{
private:
	Camera* m_camera;

public:
	//Inherited via Component
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;

	void Movement();
	void Rotation();
};