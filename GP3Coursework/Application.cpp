#include "pch.h"
#include "Application.h"
#include "Common.h"
#include "MeshRenderer.h"
#include "Quad.h"
#include "CameraComp.h"
#include "Input.h"
#include "Resources.h"
#include "Physics.h"
#include "BoxShape.h"
#include "RigidBody.h"

Application *Application::m_application = nullptr;

Application::Application()
{
}

void Application::Init()
{
	LOG_DEBUG("Init");
	//performing initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		LOG_DEBUG(SDL_GetError());
		exit(-1);
	}
		//seting openGL version to 4.2
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

		//setting openGL context to core, not compatibility
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		//creating window
		m_window = SDL_CreateWindow("GP3-GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight, SDL_WINDOW_OPENGL);

		SDL_CaptureMouse(SDL_TRUE);
		//SDL_SetRelativeMouseMode(SDL_TRUE);

		OpenGlInit();
		GameInit();
}

void Application::OpenGlInit()
{
	LOG_DEBUG("OpenGlInit");
	//creating context (our opengl statemachine in which all our GL calls will refer to)
	m_glContext = SDL_GL_CreateContext(m_window);
	CHECK_GL_ERROR();

	SDL_GL_SetSwapInterval(1);

	//initialising glew (do this after establishing context!)
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "GLEW Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}

	//Smooth shading
	GL_ATTEMPT(glShadeModel(GL_SMOOTH));

	//enable depth testing
	GL_ATTEMPT(glEnable(GL_DEPTH_TEST));

	//set less or equal func for depth testing
	GL_ATTEMPT(glDepthFunc(GL_LEQUAL));

	//enabling blending
	glEnable(GL_BLEND);
	GL_ATTEMPT(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	//turn on back face culling
	GL_ATTEMPT(glEnable(GL_CULL_FACE));
	GL_ATTEMPT(glCullFace(GL_BACK));

	glViewport(0, 0, (GLsizei)m_windowWidth, (GLsizei)m_windowHeight);
}

void Application::GameInit()
{
	LOG_DEBUG("GameInit");

	Resources::GetInstance()->AddModel("newcube.obj");
	Resources::GetInstance()->AddTexture("Wood.jpg");
	Resources::GetInstance()->AddShader(new ShaderProgram(ASSET_PATH + "simpleVert.glsl", ASSET_PATH + "simpleFrag.glsl"), "simple");
	Resources::GetInstance()->AddShader(new ShaderProgram(ASSET_PATH + "LitVert.glsl", ASSET_PATH + "LitFrag.glsl"), "lit");

	//First Entity - Plane
	Entity* a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("newcube.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Wood.jpg"))
	);
	MeshRenderer* m = a->GetComponent<MeshRenderer>();
	a->GetTransform()->SetPosition(glm::vec3(0, -10.f, -20.f));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(100.f, 1.f, 100.f)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	a->GetTransform()->SetScale(glm::vec3(100.f, 1.f, 100.f));

	//Second Entity - Camera
	a = new Entity();
	m_entities.push_back(a);
	CameraComp* cc = new CameraComp();
	a->AddComponent(cc);
	cc->Start();

	//Third Entity - Cubes
	for (int i = 0; i < 100; i++)
	{
		Entity* a = new Entity();
		m_entities.push_back(a);

		a->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("newcube.obj"),
				Resources::GetInstance()->GetShader("simple"),
				Resources::GetInstance()->GetTexture("Wood.jpg"))
		);
		a->GetTransform()->SetPosition(glm::vec3(0, 5.f * i, -20.f));
		a->AddComponent<RigidBody>();
		a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(1.f, 1.f, 1.f)));
		a->GetTransform()->SetScale(glm::vec3(1.f, 1.f, 1.f));
	}

	/*m_entities.push_back(new Entity());

	m_entities.at(0)->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("newcube.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Wood.jpg"))
	);
	m_entities.at(0)->GetTransform()->SetPosition(glm::vec3(0, 0, -10));

	m_entities.push_back(new Entity());
	CameraComp* cc = new CameraComp();
	m_entities.at(1)->AddComponent(cc);
	cc->Start();*/
}

void Application::Loop()
{
	m_appState = AppState::RUNNING;

	auto prevTicks = std::chrono::high_resolution_clock::now();
	SDL_Event event;

	while (m_appState != AppState::QUITTING)
	{
		//poll SDL events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				m_appState = AppState::QUITTING;
				break;
			case SDL_WINDOWEVENT_CLOSE:
				m_appState = AppState::QUITTING;
				break;
			case SDL_KEYDOWN:
				INPUT->SetKey(event.key.keysym.sym, true);

				//Quit app when user presses escape
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					m_appState = AppState::QUITTING;
					break;
				case SDLK_w:
					//move camera forward
					m_entities.at(1)->GetTransform()->AddPosition(m_entities.at(1)->GetTransform()->GetForward());
					break;
				case SDLK_a:
					//move camera left
					m_entities.at(1)->GetTransform()->AddPosition(-m_entities.at(1)->GetTransform()->GetRight());
					break;
				case SDLK_s:
					//move camera back
					m_entities.at(1)->GetTransform()->AddPosition(-m_entities.at(1)->GetTransform()->GetForward());
					break;
				case SDLK_d:
					//move camera right
					m_entities.at(1)->GetTransform()->AddPosition(m_entities.at(1)->GetTransform()->GetRight());
					break;
				}

				break;
			//record when the user releases a key
			case SDL_MOUSEMOTION:
				INPUT->MoveMouse(glm::ivec2(event.motion.xrel, event.motion.yrel));
				
				/*static int xpos = m_windowWidth / 2;
				static int ypos = m_windowHeight / 2;
				xpos += event.motion.xrel;
				ypos += event.motion.yrel;*/

				glm::ivec2 mouseDelta = INPUT->GetMouseDelta();

				m_entities.at(1)->GetTransform()->RotateEulerAxis(mouseDelta.x * 0.1f, glm::vec3(0, 1, 0));
				m_entities.at(1)->GetTransform()->RotateEulerAxis(mouseDelta.y * 0.1f, m_entities.at(1)->GetTransform()->GetRight());

				//LOG_DEBUG("x " + xpos);
				//LOG_DEBUG("y " + ypos);

				

				break;	
			}

			LOG_DEBUG(glm::to_string(m_entities.at(2)->GetTransform()->GetPosition()));
		}

		auto currentTicks = std::chrono::high_resolution_clock::now();

		float deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(currentTicks - prevTicks).count() / 100000;
		m_worldDeltaTime = deltaTime;
		prevTicks = currentTicks;

		Physics::GetInstance()->Update(deltaTime);
		//update and render all entities
		Update(deltaTime);
		Render();

		SDL_GL_SwapWindow(m_window);
	}
}

void Application::Update(float deltaTime)
{
	for (auto& a : m_entities)
	{
		a->OnUpdate(deltaTime);
	}

	INPUT->UpdatePreviousState();
}

void Application::Render()
{
	/*Clear context*/
	glClearColor(0.f,0.f,0.4,1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_mainCamera->Recalculate();
	for (auto& a : m_entities)
	{
		a->OnRender();
	}
}

void Application::SetCamera(Camera* camera)
{
	if (camera != nullptr)
	{
		m_mainCamera = camera;
	}
}

void Application::Quit()
{
	//Close SDL
	Physics::GetInstance()->Quit();
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
	SDL_Quit;
}

Application::~Application()
{
}

Application * Application::GetInstance()
{
	if (m_application == nullptr)
	{
		m_application = new Application();
	}
	return m_application;
}

void Application::Run()
{
	Init();
	Loop();
	Quit();
}