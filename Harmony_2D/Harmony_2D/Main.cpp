#include "Mesh.h"
#include "FrameBuffer.h"

static double DeltaTime = 0.0;
static double LastFrame = 0.0;
static unsigned int FrameCounter = 0;
static bool IsFirstMouse = true;
static bool IsMouseActive = false;
static double MouseX = 0.0, MouseY = 0.0;
static float Depth = 1;

static Camera* SceneCamera = nullptr;

static GLFWwindow* RenderWindow = nullptr;
static std::map<int, bool> Keypresses;

static void InitGLFW();
static void InitGLEW();

void Start();
void Update();
int Cleanup();

static Mesh* FrameBufferMesh = nullptr;
static std::vector<Mesh*> Meshes;

static void CalculateDeltaTime()
{
	float currentFrame = (float)glfwGetTime();
	DeltaTime = currentFrame - LastFrame;
	LastFrame = currentFrame;
	FrameCounter++;
}

static inline void ErrorCallback(int _error, const char* _description)
{
	Print("Error: %s\n");
	Print(_description);
}

static inline void CursorPositionCallback(GLFWwindow* _renderWindow, double _xPos, double _yPos)
{
	if (IsFirstMouse)
	{
		MouseX = _xPos;
		MouseY = _yPos;
		IsFirstMouse = false;
	}
	double xoffset = _xPos - MouseX;
	MouseX = _xPos;

	double yoffset = MouseY - _yPos;
	MouseY = _yPos;

	if (SceneCamera && !IsMouseActive)
		SceneCamera->ProcessMouse(xoffset, yoffset);
}

static inline void MouseButtonCallback(GLFWwindow* _renderWindow, int _button, int _action, int _mods)
{
	if (_action == GLFW_PRESS)
	{
		Depth = FrameBuffer::GrabDepthUnderMouse(std::move(MouseX), std::move(MouseY));
		FrameBuffer::GrabMousePositionIn3D(std::move(MouseX), std::move(MouseY));
	}
	else if (_action == GLFW_RELEASE)
	{

	}
}

static inline void KeyCallback(GLFWwindow* _renderWindow, int _key, int _scanCode, int _action, int _mods)
{
	// Collect Input
	if (_action == GLFW_PRESS)
		Keypresses[_key] = true;
	else if (_action == GLFW_RELEASE)
		Keypresses[_key] = false;

	if (SceneCamera)
		SceneCamera->Input();
}

static inline void ScrollCallback(GLFWwindow* _renderWindow, double _xOffset, double _yOffset)
{
	if (SceneCamera)
		SceneCamera->ProcessScroll(_yOffset);
}

int main()
{
	Start();
	Update();
	return Cleanup();
}

void InitGLFW()
{
	// Init GLFW
	if (glfwInit() == GLFW_FALSE)
		Print("Failed to Initalise GLFW");

	// Setting Version And Profile ; 460 : CORE
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create Window
	RenderWindow = glfwCreateWindow(1080, 1080, "Harmony2D v0.01", NULL, NULL);

	// Set Context To New Window
	glfwMakeContextCurrent(RenderWindow);

	glfwSetKeyCallback(RenderWindow, KeyCallback);
	glfwSetCursorPosCallback(RenderWindow, CursorPositionCallback);
	glfwSetMouseButtonCallback(RenderWindow, MouseButtonCallback);
	glfwSetScrollCallback(RenderWindow, ScrollCallback);

	if (IsMouseActive)
		glfwSetInputMode(RenderWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode(RenderWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InitGLEW()
{
	// Init Glew
	if (glewInit() != GLEW_OK)
		Print("Failed to Initalise GLEW");
}



void Start()
{
	InitGLFW();
	InitGLEW();

	glEnable(GL_CULL_FACE);

	FrameBuffer::InitFrameBufferDSA();

	TextureLoader::Init();

	// Set Clear Color / Background
	glClearColor(FrameBuffer::BackgroundColor[0], FrameBuffer::BackgroundColor[1], FrameBuffer::BackgroundColor[2], FrameBuffer::BackgroundColor[3]);

	FrameBufferMesh = new Mesh(FrameBuffer::FrameBufferTexture);
	
	SceneCamera = new Camera(Keypresses);

	for (int i = 0; i < 1; i++)
	{
		Meshes.push_back(new Mesh(*SceneCamera));
	}

	// Plane
	{
		//Meshes[0]->GetTransform().translation = { 0,0,0 };
		//Meshes[0]->GetTransform().scale = { 100, 100, 1};
		//Meshes[0]->GetTransform().rotation_axis = { 1,0,0 };
		//Meshes[0]->GetTransform().rotation_value = 3.14 / 2;
	}
}

void Update()
{
	while (!glfwWindowShouldClose(RenderWindow))
	{
		FrameBuffer::Bind();

		// Clear Frame Buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		FrameBuffer::ClearTexturesCustom();

		// Update
		for (auto& item : Keypresses)
		{
			if (item.second == true)
			{
				switch (item.first)
				{
				case GLFW_KEY_TAB:
				{
					IsMouseActive = !IsMouseActive;

					if (IsMouseActive)
						glfwSetInputMode(RenderWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					else
						glfwSetInputMode(RenderWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
						
					item.second = false;
					break;
				}
				case GLFW_KEY_ESCAPE:
				{
					glfwSetWindowShouldClose(RenderWindow, GLFW_TRUE);

					item.second = false;
					break;
				}
				case GLFW_KEY_K:
				{
					if (Meshes[0])
					{
						delete Meshes[0];
						Meshes[0] = nullptr;
					}
					else
					{
						Meshes[0] = new Mesh(*SceneCamera);
					}
						

					item.second = false;
					break;
				}
				default:
					break;
				}
			}
		}

		CalculateDeltaTime();

		if (SceneCamera)
		{
			SceneCamera->Movement(DeltaTime);
		}



		// Draw Items To Frame Buffer
		for (auto& item : Meshes)
		{
			item->Draw(Depth);
		}
		
		// Draw Frame Buffer To Screen
		glDisable(GL_DEPTH_TEST);

		FrameBuffer::UnBind();

		if (FrameBufferMesh != nullptr)
			FrameBufferMesh->Draw();

		glEnable(GL_DEPTH_TEST);

		// Swap Buffers
		glfwSwapBuffers(RenderWindow);

		// Poll Events
		glfwPollEvents();
	}
}

int Cleanup()
{
	FrameBuffer::Cleanup();

	if (FrameBufferMesh != nullptr)
		delete FrameBufferMesh;
	FrameBufferMesh = nullptr;

	for (auto& item : Meshes)
	{
		if (item != nullptr)
		{
			delete item;
		}
		item = nullptr;
	}
	Meshes.clear();

	if (SceneCamera != nullptr)
		delete SceneCamera;
	SceneCamera = nullptr;

	// Cleanup GLFW
	glfwDestroyWindow(RenderWindow);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}






