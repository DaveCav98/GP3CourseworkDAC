#include "pch.h"
#include "FPSCamera.h"
#include "Camera.h"
#include "Input.h"

void FPSCamera::OnUpdate(float deltaTime)
{
	FPSCamera::Movement();
	FPSCamera::Rotation();
}

void FPSCamera::OnRender()
{

}

void FPSCamera::Movement()
{
	//using WASD move camera

}

void FPSCamera::Rotation()
{
	SDL_Event event;

	/*while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_MOUSEMOTION:

			

			if (INPUT->MoveMouse.GetInstance.)
			{
				LOG_DEBUG("Moving Right!!");
			}

			if (INPUT->MoveMouse.event.motion.xrel < 0)
			{
				LOG_DEBUG("Moving Left!!");
			}
			break;
		}
	}*/

	//using mouse rotate camera
	//if (INPUT->MoveMouse().motion.xrel > 0)
	//{
	//	LOG_DEBUG("Moving Right!");
	//}

	//if (INPUT->MoveMouse.MoveMouse.event.motion.xrel < 0)
	//{
		//LOG_DEBUG("Moving Left!");
	//}
}