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

static glm::ivec2 WindowSize{ 1000,1000 };
static double DeltaTime = 0.0, LastFrame = 0.0;
static bool IsMouseVisible = false, IsWireframe = false, EnterUsername = false ,ExitProcess = false;
static std::string Username = "";

static Camera* SceneCamera = nullptr;
static GLFWwindow* RenderWindow = nullptr;
static std::map<int, bool> Keypresses;
static Mesh* CubeMesh = nullptr;
static Mesh* CubeMesh2 = nullptr;
static TextLabel* EnterToTypePromptText = nullptr;
static TextLabel* BackSpaceToDeletePromptText = nullptr;
static TextLabel* TabToToggleMousePromptText = nullptr;
static TextLabel* EscapeToCloseWindowPromptText = nullptr;
static TextLabel* CapsToToggleWireframeText = nullptr;
static TextLabel* UsernameText = nullptr;

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

static inline void CursorPositionCallback(GLFWwindow* _renderWindow, double _xpos, double _ypos)
{
	if (IsMouseVisible)
	{
		std::string cursorPosition = "Cursor Position | X: ";
		cursorPosition += std::to_string((int)_xpos);
		cursorPosition += " Y: ";
		cursorPosition += std::to_string((int)_ypos);
		Print(cursorPosition);
	}
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

static inline void CharacterCallback(GLFWwindow* _renderWindow, unsigned _uniCode)
{
	if (EnterUsername)
	{
		// If Name Does Not Exceed Bounds
		if ((UsernameText->GetPosition().x + (Username.size() * UsernameText->GetAverageCharacterAdvance() * UsernameText->GetScale().x)) < UsernameText->GetRightClip() - 110)
		{
			// Grab Entered Character
			Username += (char)_uniCode;

			// Update Username Text
			UsernameText->SetText(Username);
		}
	}
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

	// Enable Culling
	glEnable(GL_CULL_FACE);
	// Enable Blending
	glEnable(GL_BLEND);
	// Set Blending To Handle Alpha On Texture
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
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
	SceneCamera = new Camera(WindowSize, Keypresses, {0,0,10});

	CubeMesh = new Mesh(*SceneCamera, DeltaTime, SHAPE::CUBE, 
		{ 
			TextureLoader::LoadTexture("Resources/Textures/DiceSix.png"),
			TextureLoader::LoadTexture("Resources/Textures/DiceOne.png"),
			TextureLoader::LoadTexture("Resources/Textures/DiceFive.png"),
			TextureLoader::LoadTexture("Resources/Textures/DiceTwo.png"),
			TextureLoader::LoadTexture("Resources/Textures/DiceFour.png"),
			TextureLoader::LoadTexture("Resources/Textures/DiceThree.png")
		});
	CubeMesh->SetPosition({ 0,0,-2 });

	CubeMesh2 = new Mesh(std::move(CubeMesh->GetVertexArrayID()), std::move(CubeMesh->GetIndexBufferID()),*SceneCamera, DeltaTime, SHAPE::CUBE,
		{
			TextureLoader::LoadTexture("Resources/Textures/Rayman.jpg"),
			TextureLoader::LoadTexture("Resources/Textures/Gull.jpg"),
			TextureLoader::LoadTexture("Resources/Textures/path.jpg")
		});
	CubeMesh2->SetPosition({ 0,0,-1.8f });

	EnterToTypePromptText = new TextLabel(&WindowSize, "Enter To Type UserName ", "Resources/Fonts/ARIAL.TTF", DeltaTime, {0.0f, 15.0f }, { 0.8f, 0.2f , 1.0f, 1.0f });
	EnterToTypePromptText->SetClip(0.0f, 500.0f);
	EnterToTypePromptText->SetScale({ 0.25f, 0.25f });
	EnterToTypePromptText->SetScrolling(true);

	BackSpaceToDeletePromptText = new TextLabel(&WindowSize, "BackSpace To Delete Last Character ", "Resources/Fonts/ARIAL.TTF", DeltaTime, { (float)WindowSize.x / 2.0f, 15.0f }, { 0.8f, 0.1f , 0.1f, 1.0f });
	BackSpaceToDeletePromptText->SetClip(500.0f, 1000.0f);
	BackSpaceToDeletePromptText->SetScale({ 0.25f, 0.25f });
	BackSpaceToDeletePromptText->SetScrolling(true);
	BackSpaceToDeletePromptText->SetScrollingRight(false);
	BackSpaceToDeletePromptText->SetScrollSpeed(0.5f);

	UsernameText = new TextLabel(&WindowSize, "Username", "Resources/Fonts/ARIAL.TTF", DeltaTime, { 350.0f, 50.0f });
	UsernameText->SetClip(350.0f, 650.0f);
	UsernameText->SetScale({ 0.25f, 0.25f });

	TabToToggleMousePromptText = new TextLabel(&WindowSize, "TAB To Toggle Mouse", "Resources/Fonts/ARIAL.TTF", DeltaTime, { 0.0f, (float)WindowSize.y - 35.0f });
	TabToToggleMousePromptText->SetClip(0.0f, 400.0f);
	TabToToggleMousePromptText->SetScale({ 0.25f, 0.25f });

	CapsToToggleWireframeText = new TextLabel(&WindowSize, "CAPSLOCK To Toggle Wireframe", "Resources/Fonts/ARIAL.TTF", DeltaTime, { 0.0f, (float)WindowSize.y - 65.0f });
	CapsToToggleWireframeText->SetClip(0.0f, 500.0f);
	CapsToToggleWireframeText->SetScale({ 0.25f, 0.25f });

	EscapeToCloseWindowPromptText = new TextLabel(&WindowSize, "ESC To Close Window", "Resources/Fonts/ARIAL.TTF", DeltaTime, { 640.0f, (float)WindowSize.y - 35.0f });
	EscapeToCloseWindowPromptText->SetClip(640.0f, 1000.0f);
	EscapeToCloseWindowPromptText->SetScale({ 0.25f, 0.25f });
	
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

		CubeMesh->RotateAround({0,0,0 }, {0,1,0}, (float)DeltaTime);
		CubeMesh2->RotateAround({ 0,0,0 }, { 1,0,0 }, (float)DeltaTime);

		//m_TextLabelTest->SetScale({ ClampedSin((float)glfwGetTime()) + 0.5f, ClampedSin((float)glfwGetTime()) + 0.5f });
		EnterToTypePromptText->Update();

		UsernameText->Update();

		BackSpaceToDeletePromptText->Update();

		TabToToggleMousePromptText->Update();

		EscapeToCloseWindowPromptText->Update();

		CapsToToggleWireframeText->Update();
		

		// Main Render
		Render();
	}
}

/// <summary>
/// Main Render Function That Draws To The Screen At The End Of Every Frame
/// </summary>
void Render()
{
	// Update PolygonMode Based On Bool
	if (IsWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Cube 1
	CubeMesh->Draw();

	// Cube 2
	CubeMesh2->Draw();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	EnterToTypePromptText->Draw();

	UsernameText->Draw();

	BackSpaceToDeletePromptText->Draw();

	TabToToggleMousePromptText->Draw();

	EscapeToCloseWindowPromptText->Draw();

	CapsToToggleWireframeText->Draw();

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

	if (EnterToTypePromptText != nullptr)
		delete EnterToTypePromptText;
	EnterToTypePromptText = nullptr;

	if (BackSpaceToDeletePromptText != nullptr)
		delete BackSpaceToDeletePromptText;
	BackSpaceToDeletePromptText = nullptr;

	if (TabToToggleMousePromptText != nullptr)
		delete TabToToggleMousePromptText;
	TabToToggleMousePromptText = nullptr;

	if (CapsToToggleWireframeText != nullptr)
		delete CapsToToggleWireframeText;
	CapsToToggleWireframeText = nullptr;

	if (UsernameText != nullptr)
		delete UsernameText;
	UsernameText = nullptr;

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
				if (!EnterUsername)
				{
					// Toggle Mouse Visible
					IsMouseVisible = !IsMouseVisible;
					// Update Based On Bool
					HandleMouseVisible();
				}

				// On Button Down Instead Of Hold
				item.second = false;
				break;
			}
			case GLFW_KEY_CAPS_LOCK:
			{
				if (!EnterUsername)
				{
					// Toggle Wireframe
					IsWireframe = !IsWireframe;
				}

				// On Button Down Instead Of Hold
				item.second = false;
				break;
			}
			case GLFW_KEY_ENTER:
			{
				// Toggle Enter Username
				EnterUsername = !EnterUsername;

				if (EnterUsername)
				{
					Username = "";

				}
				else
					if (Username.size() <= 0)
						Username = "Username";

				UsernameText->SetText(Username);

				// On Button Down Instead Of Hold
				item.second = false;
				break;
			}
			case GLFW_KEY_BACKSPACE:
			{
				// Backspace Username
				if (EnterUsername && Username.size() > 0)
				{
					Username.pop_back();
					UsernameText->SetText(Username);
				}

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






