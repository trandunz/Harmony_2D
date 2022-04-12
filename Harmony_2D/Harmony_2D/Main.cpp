// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Main.cpp 
// Description : Main Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "Mesh.h"
#include "TextLabel.h"

static glm::ivec2 WindowSize{ 1080,1080 };
static double DeltaTime = 0.0, LastFrame = 0.0;
static bool IsMouseVisible = false, ExitProcess = false;

static Camera* SceneCamera = nullptr;
static GLFWwindow* RenderWindow = nullptr;
static std::map<int, bool> Keypresses;
static Mesh* CubeMesh = nullptr;
static Mesh* CubeMesh2 = nullptr;
static TextLabel* m_TextLabelTest = nullptr;

void InitGLFW();
void InitGLEW();

void Start();
void Update();
void Render();
int Cleanup();

void HandleKeymapActions();
void HandleMouseVisible();

/// <summary>
/// Calculates The Time Taken Between The Last Frame And The Current One
/// </summary>
static inline void CalculateDeltaTime()
{
	double currentFrame = glfwGetTime();
	DeltaTime = currentFrame - LastFrame;
	LastFrame = currentFrame;
}

/// <summary>
/// Handles Window Resizing.
/// Calls GlViewPort(0,0,newWidth,newHeight).
/// Sets WindowSize = {newWidth,newHeight}.
/// </summary>
/// <param name="_renderWindow"></param>
/// <param name="_width"></param>
/// <param name="_height"></param>
static inline void WindowResizeCallback(GLFWwindow* _renderWindow, int _width, int _height)
{
	glViewport(0, 0, _width, _height);
	WindowSize = { _width , _height};
}

/// <summary>
/// GLFW Key Callback Function
/// </summary>
/// <param name="_renderWindow"></param>
/// <param name="_key"></param>
/// <param name="_scanCode"></param>
/// <param name="_action"></param>
/// <param name="_mods"></param>
static inline void KeyCallback(GLFWwindow* _renderWindow, int _key, int _scanCode, int _action, int _mods)
{
	// Collect Input
	if (_action == GLFW_PRESS)
		Keypresses[_key] = true;
	else if (_action == GLFW_RELEASE)
		Keypresses[_key] = false;

	// Handle Input
	if (SceneCamera)
		SceneCamera->Input();
}

/// <summary>
/// Main Thread Function
/// </summary>
/// <returns></returns>
int main()
{
	Start();
	Update();
	return Cleanup();
}

/// <summary>
/// Initalizes GLFW With Window Hints, Callback Functions
/// </summary>
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
	RenderWindow = glfwCreateWindow(WindowSize.x, WindowSize.y, "GD1P04 - Assessment 1", NULL, NULL);

	// Set Context To New Window
	glfwMakeContextCurrent(RenderWindow);

	// Set Callback Functions
	glfwSetKeyCallback(RenderWindow, KeyCallback);
	glfwSetWindowSizeCallback(RenderWindow, WindowResizeCallback);
}

/// <summary>
/// Initalizes GLEW, Enables Culling, Enables 1-Alpha Blending, Sets Clear Color For Default FrameBuffer (Grey)
/// </summary>
void InitGLEW()
{
	// Init Glew
	if (glewInit() != GLEW_OK)
		Print("Failed to Initalise GLEW");

	// Enable Culling
	glEnable(GL_CULL_FACE);
	// Enable Blending
	glEnable(GL_BLEND);
	// Set Blending To Handle Alpha On Texture
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Set Window Clear Colour To Sky Blue
	glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
}

/// <summary>
/// Main Initalization Function That Gets Called When The Proccess Open's
/// </summary>
void Start()
{
	InitGLFW();
	InitGLEW();

	// Update Mouse Visible Based On Default Value
	HandleMouseVisible();
	
	// Initialize Texture Loader
	TextureLoader::Init({ 
		"Resources/Textures/Capguy_Walk.png", 
		"Resources/Textures/path.jpg",
		"Resources/Textures/AwesomeFace.png",
		"Resources/Textures/Rayman.jpg" ,
		"Resources/Textures/Raven.png",
		"Resources/Textures/Gull.jpg"
		});

	// Create The Scene Camera
	SceneCamera = new Camera(WindowSize, Keypresses, {0,0,1});

	CubeMesh = new Mesh(*SceneCamera, DeltaTime, SHAPE::CUBE, 
		{ 
			TextureLoader::LoadTexture("Resources/Textures/DiceSix.png"),
			TextureLoader::LoadTexture("Resources/Textures/DiceOne.png"),
			TextureLoader::LoadTexture("Resources/Textures/DiceFive.png"),
			TextureLoader::LoadTexture("Resources/Textures/DiceTwo.png"),
			TextureLoader::LoadTexture("Resources/Textures/DiceFour.png"),
			TextureLoader::LoadTexture("Resources/Textures/DiceThree.png")
		});
	CubeMesh->SetPosition({ 0,0,-5 });

	CubeMesh2 = new Mesh(std::move(CubeMesh->GetVertexArrayID()), std::move(CubeMesh->GetIndexBufferID()),*SceneCamera, DeltaTime, SHAPE::CUBE,
		{
			TextureLoader::LoadTexture("Resources/Textures/Rayman.jpg"),
			TextureLoader::LoadTexture("Resources/Textures/Gull.jpg"),
			TextureLoader::LoadTexture("Resources/Textures/path.jpg")
		});
	CubeMesh2->SetPosition({ 0,0,-5 });

	m_TextLabelTest = new TextLabel(&WindowSize, "Yay!", "Resources/Fonts/ARIAL.TTF", DeltaTime, { WindowSize.x / 2, WindowSize.y / 1.1f }, {0,0,0,1});
}

/// <summary>
/// Main Update Function That Gets Called Every Frame
/// </summary>
void Update()
{
	while (ExitProcess == false)
	{
		// Clear Default Frame Buffer Of Colour
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

		CubeMesh->RotateAround({0,0,0}, {0,1,0}, DeltaTime);
		CubeMesh2->RotateAround({ 0,0,0 }, { 1,0,0 }, DeltaTime);

		m_TextLabelTest->Update();

		// Main Render
		Render();
	}
}

/// <summary>
/// Main Render Function That Draws To The Screen At The End Of Every Frame
/// </summary>
void Render()
{
	// Cube 1
	CubeMesh->Draw();

	// Cube 2
	CubeMesh2->Draw();

	m_TextLabelTest->Draw();

	// Swap Front Buffer And Back Buffer
	glfwSwapBuffers(RenderWindow);
}

/// <summary>
/// Returns Main Thread Exit Code.
/// Handles Cleaning Up Any Pointers Or Vectors Created In The Main Implementation File.
/// </summary>
/// <returns></returns>
int Cleanup()
{
	// Cleanup Cube 1
	if (CubeMesh != nullptr)
		delete CubeMesh;
	CubeMesh = nullptr;

	// Cleanup Cube 2
	if (CubeMesh2 != nullptr)
		delete CubeMesh2;
	CubeMesh2 = nullptr;

	if (m_TextLabelTest != nullptr)
		delete m_TextLabelTest;
	m_TextLabelTest = nullptr;

	// Cleanup Scene Camera
	if (SceneCamera != nullptr)
		delete SceneCamera;
	SceneCamera = nullptr;

	// Cleanup GLFW
	glfwDestroyWindow(RenderWindow);
	glfwTerminate();
	exit(EXIT_SUCCESS);
	
	// Return Main Thread Exit Code (0)
	return EXIT_SUCCESS;
}

/// <summary>
/// Handles All Window Related Input Actions
/// </summary>
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
				// Toggle Mouse Visible
				IsMouseVisible = !IsMouseVisible;
				// Update Based On Bool
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

/// <summary>
/// Disable Or Enables The Mouse Curson Depending On Bool IsMouseVisible
/// </summary>
void HandleMouseVisible()
{
	if (IsMouseVisible)
		glfwSetInputMode(RenderWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode(RenderWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}






