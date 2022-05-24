// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Main.cpp 
// Description : Main Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "GameObject.h"
#include "TextLabel.h"
#include "FontLoader.h"

glm::ivec2 WindowSize{ 1000,1000 };

float DeltaTime = 0.0, LastFrame = 0.0;

bool IsMouseVisible = false, IsWireframe = false;

GameObject* CubeObject = nullptr;

Camera* SceneCamera = nullptr;

Mesh* CubeMesh = nullptr;
Mesh* PyramidMesh = nullptr;

Font ArialFont;
std::vector<TextLabel*> TextLabels{};

KEYMAP Keypresses;
GLFWwindow* RenderWindow = nullptr;

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
	float currentFrame = (float)glfwGetTime();
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
/// GLFW Mouse Position Callback Function.
/// Activates when the mouse moves
/// </summary>
/// <param name="_renderWindow"></param>
/// <param name="_xpos"></param>
/// <param name="_ypos"></param>
static inline void CursorPositionCallback(GLFWwindow* _renderWindow, double _xpos, double _ypos)
{
	if (SceneCamera)
		SceneCamera->MouseLook(DeltaTime, {(float)_xpos , (float) _ypos});
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

	if (SceneCamera)
		SceneCamera->Movement_Capture(Keypresses);
}

/// <summary>
/// GLFW Character Callback Function.
/// Returns uniCode / Asci Input Codes
/// </summary>
/// <param name="_renderWindow"></param>
/// <param name="_uniCode"></param>
static inline void CharacterCallback(GLFWwindow* _renderWindow, unsigned _uniCode)
{
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
	// MSAA x 4
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create Window
	RenderWindow = glfwCreateWindow(WindowSize.x, WindowSize.y, "GD1P04 - Assessment 3", NULL, NULL);

	// Set Context To New Window
	glfwMakeContextCurrent(RenderWindow);

	// Set Callback Functions
	glfwSetKeyCallback(RenderWindow, KeyCallback);
	glfwSetWindowSizeCallback(RenderWindow, WindowResizeCallback);
	glfwSetCursorPosCallback(RenderWindow, CursorPositionCallback);
	glfwSetCharCallback(RenderWindow, CharacterCallback);
}

/// <summary>
/// Initalizes GLEW, Enables Culling, Enables 1-Alpha Blending, Sets Clear Color For Default FrameBuffer (Grey)
/// </summary>
void InitGLEW()
{
	// Init Glew
	if (glewInit() != GLEW_OK)
		Print("Failed to Initalise GLEW");

	// Enable MSAA
	glEnable(GL_MULTISAMPLE);
	// Enable Culling
	glEnable(GL_CULL_FACE);
	// Enable Blending
	glEnable(GL_BLEND);
	// Set Blending To Handle Alpha On Texture
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Enable Depth Buffer
	glEnable(GL_DEPTH_TEST);
	// Set Window Clear Colour To Sky Blue
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
	TextureLoader::Init({ 
		"Capguy_Walk.png", 
		"path.jpg",
		"AwesomeFace.png",
		"Rayman.jpg" ,
		"Raven.png",
		"Gull.jpg"
		});

	ArialFont = FontLoader::LoadFont("ARIAL.TTF");

	// Create The Scene Camera
	SceneCamera = new Camera(WindowSize, {0,0,10});

	CubeMesh = new Mesh(SHAPE::CUBE);
	PyramidMesh = new Mesh(SHAPE::PYRAMID);

	CubeObject = new GameObject(*SceneCamera, { 0,-2,0 });
	CubeObject->SetShader("SingleTexture.vert", "SingleTexture.frag");
	CubeObject->SetMesh(CubeMesh);
	CubeObject->SetActiveTextures({ TextureLoader::LoadTexture("Rayman.jpg") });
	CubeObject->SetScale({100,1,100});
}

/// <summary>
/// Main Update Function That Gets Called Every Frame
/// </summary>
void Update()
{
	while (glfwWindowShouldClose(RenderWindow) == false)
	{
		// Clear Default Frame Buffer Of Colour
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calculate DeltaTime
		CalculateDeltaTime();

		// Poll Events
		glfwPollEvents();

		// Input
		HandleKeymapActions();

		if (SceneCamera)
			SceneCamera->Movement(DeltaTime);

		if (CubeObject)
			CubeObject->Update(DeltaTime);

		// Main Render
		Render();
	}
}

/// <summary>
/// Main Render Function That Draws To The Screen At The End Of Every Frame
/// </summary>
void Render()
{
	// Enable Wireframe if bool
	if (IsWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	CubeObject->Draw();

	// Disable Wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Draw Any TextLabels Without Wireframe
	for (auto& textLabel : TextLabels)
	{
		textLabel->Draw();
	}

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
	if (CubeMesh != nullptr)
		delete CubeMesh;
	CubeMesh = nullptr;

	if (PyramidMesh != nullptr)
		delete PyramidMesh;
	PyramidMesh = nullptr;

	if (CubeObject != nullptr)
		delete CubeObject;
	CubeObject = nullptr;

	// Clean up TextLabels
	for (auto& textLabel : TextLabels)
	{
		if (textLabel)
			delete textLabel;
		textLabel = nullptr;
	}
	TextLabels.clear();

	// Clean up Arial Font
	for (auto& fontCharacter : ArialFont)
	{
		glDeleteTextures(1, &fontCharacter.second.textureID);
	}
	ArialFont.clear();

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
				IsMouseVisible = !IsMouseVisible;
				HandleMouseVisible();

				// On Button Down Instead Of Hold
				item.second = false;
				break;
			}
			case GLFW_KEY_ESCAPE:
			{
				// Set Window To Close At End Of Frame
				glfwSetWindowShouldClose(RenderWindow, true);

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






