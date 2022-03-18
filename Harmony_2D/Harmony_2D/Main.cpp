#include "Mesh.h"

static glm::ivec2 WindowSize{ 1080,1080 };
static double DeltaTime = 0.0, LastFrame = 0.0;
static bool IsMouseVisible = false, ExitProcess = false;

static Camera* SceneCamera = nullptr;
static GLFWwindow* RenderWindow = nullptr;
static std::map<int, bool> Keypresses;
static Mesh* HexagonMesh = nullptr;
static Mesh* CapGuyMesh = nullptr;

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
	// Set Window Clear Colour To Gray
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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
	TextureLoader::Init();

	// Create The Scene Camera
	SceneCamera = new Camera(WindowSize, Keypresses, {0,0,1});

	// Quad / Cap Guy Mesh
	CapGuyMesh = new Mesh(*SceneCamera, DeltaTime, 4, 8,
		{ 
			TextureLoader::LoadTexture("Resources/Textures/Capguy_Walk.png")
		});
	// Set Its Position To Bottom Middle
	CapGuyMesh->SetPosition({ 0.0f, -WindowSize.y / 4, 0 });
	// Start Animation
	CapGuyMesh->ToggleAnimating();
	// Set Starting Animation Frame
	CapGuyMesh->SetAnimationFrame(0);

	//  Hexagon Mesh
	HexagonMesh = new Mesh(*SceneCamera, DeltaTime, 6,
		{
			TextureLoader::LoadTexture("Resources/Textures/AwesomeFace.png"),
			TextureLoader::LoadTexture("Resources/Textures/Rayman.jpg")
		});
	

	// Scale Both Hexagons To Half There Size
	HexagonMesh->Scale({ 0.5f, 0.5f, 1.0f });
	HexagonMesh->SetTextureFadeSpeed(0.5f);
}

/// <summary>
/// Main Update Function That Gets Called Every Frame
/// </summary>
void Update()
{
	while (ExitProcess == false)
	{
		// Clear Default Frame Buffer Of Colour
		glClear(GL_COLOR_BUFFER_BIT);

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

/// <summary>
/// Main Render Function That Draws To The Screen At The End Of Every Frame
/// </summary>
void Render()
{
	// Draw Hexagon Meshes (2)
	// First
	HexagonMesh->SetPosition({ WindowSize.x / 4, WindowSize.y / 4, 0 });
	HexagonMesh->Draw();

	// Second
	HexagonMesh->SetPosition({ -WindowSize.x / 4, WindowSize.y / 4, 0 });
	HexagonMesh->Draw();

	// Draw Cap Guy
	CapGuyMesh->Draw();

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
	// Cleanup Hexagons
	if (HexagonMesh != nullptr)
		delete HexagonMesh;
	HexagonMesh = nullptr;

	// Cleanup Cap Guy
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






