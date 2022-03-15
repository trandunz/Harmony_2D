#include "Mesh.h"

static int WindowHeight = 1080, WindowWidth = 1080;
static double DeltaTime = 0.0, LastFrame = 0.0, MouseX = 0.0, MouseY = 0.0;
static bool IsMouseVisible = false, ExitProcess = false;

static Camera* SceneCamera = nullptr;
static GLFWwindow* RenderWindow = nullptr;
static std::map<int, bool> Keypresses;
static std::vector<Mesh*> HexagonMeshes;
static Mesh* CapGuyMesh = nullptr;

void InitGLFW();
void InitGLEW();

void Start();
void Update();
void Render();
int Cleanup();

void HandleKeymapActions();
void HandleMouseVisible();

static void CalculateDeltaTime()
{
	float currentFrame = (float)glfwGetTime();
	DeltaTime = currentFrame - LastFrame;
	LastFrame = currentFrame;
}

static inline void CursorPositionCallback(GLFWwindow* _renderWindow, double _xPos, double _yPos)
{
	MouseX = _xPos;
	MouseY = _yPos;
}

static inline void MouseButtonCallback(GLFWwindow* _renderWindow, int _button, int _action, int _mods)
{
	if (_action == GLFW_PRESS)
	{
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
		SceneCamera->ProcessScroll((const float)_yOffset);
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
	RenderWindow = glfwCreateWindow(WindowWidth, WindowHeight, "GD1P04 - Assessment 1", NULL, NULL);

	// Set Context To New Window
	glfwMakeContextCurrent(RenderWindow);

	// Set Callback Functions
	glfwSetKeyCallback(RenderWindow, KeyCallback);
	glfwSetCursorPosCallback(RenderWindow, CursorPositionCallback);
	glfwSetMouseButtonCallback(RenderWindow, MouseButtonCallback);
	glfwSetScrollCallback(RenderWindow, ScrollCallback);

	// Check For Mouse Visible
	HandleMouseVisible();
}

void InitGLEW()
{
	// Init Glew
	if (glewInit() != GLEW_OK)
		Print("Failed to Initalise GLEW");

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
}

void Start()
{
	InitGLFW();
	InitGLEW();

	TextureLoader::Init();

	SceneCamera = new Camera(Keypresses);

	CapGuyMesh = new Mesh(*SceneCamera, DeltaTime, 4,
		{ TextureLoader::LoadTexture("Resources/Textures/Capguy_Walk.png")}
		, true);
	CapGuyMesh->SetPosition({ 0.0f, -1080 / 4, 0 });

	for (int i = 0; i < 1; i++)
	{
		HexagonMeshes.push_back(new Mesh(*SceneCamera, DeltaTime, 6, 
			{ TextureLoader::LoadTexture("Resources/Textures/AwesomeFace.png"),TextureLoader::LoadTexture("Resources/Textures/Rayman.jpg")}));
	}
}

void Update()
{
	while (ExitProcess == false)
	{
		// Clear Default Frame Buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calculate DeltaTime
		CalculateDeltaTime();

		// Poll Events
		glfwPollEvents();

		// Input
		HandleKeymapActions();

		// Handle Camera Movement
		if (SceneCamera)
		{
			SceneCamera->Movement(DeltaTime);
		}

		// Main Render
		Render();
	}
}

void Render()
{
	// Draw Hexagon Meshes
	for (auto& item : HexagonMeshes)
	{
		item->SetPosition({ 1080 / 4, 1080 / 4, 0 });
		item->Draw();
		item->SetPosition({ -1080 / 4, 1080 / 4, 0 });
		item->Draw();
	}

	// Draw Cap Guy
	CapGuyMesh->Draw();

	// Swap Buffers
	glfwSwapBuffers(RenderWindow);
}

int Cleanup()
{
	// Cleanup All Meshes
	for (auto& item : HexagonMeshes)
	{
		if (item != nullptr)
		{
			delete item;
		}
		item = nullptr;
	}
	HexagonMeshes.clear();

	if (CapGuyMesh != nullptr)
		delete CapGuyMesh;
	CapGuyMesh = nullptr;

	// Cleanup Scene Camera
	if (SceneCamera != nullptr)
		delete SceneCamera;
	SceneCamera = nullptr;

	// Cleanup GLFW
	glfwDestroyWindow(RenderWindow);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	// Return Main Thread Exit Code
	return EXIT_SUCCESS;
}

void HandleKeymapActions()
{
	for (auto& item : Keypresses)
	{
		if (item.second == true)
		{
			switch (item.first)
			{
			case GLFW_KEY_TAB:
			{
				IsMouseVisible = !IsMouseVisible;
				HandleMouseVisible();

				// On Button Down Instead Of Hold
				item.second = false;
				break;
			}
			case GLFW_KEY_ESCAPE:
			{
				// Set Window To Close At End Of Frame
				ExitProcess = true;

				// On Button Down Instead Of Hold
				item.second = false;
				break;
			}
			default:
				break;
			}
		}
	}
}

void HandleMouseVisible()
{
	if (IsMouseVisible)
		glfwSetInputMode(RenderWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode(RenderWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}






