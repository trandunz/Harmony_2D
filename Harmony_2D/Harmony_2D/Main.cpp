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

static glm::ivec2 WindowSize{ 1000,1000 };
static double DeltaTime = 0.0, LastFrame = 0.0;
static bool IsMouseVisible = false, IsWireframe = false, EnterUsername = false ,ExitProcess = false;
static std::string Username = "";
static std::map<int, bool> Keypresses;

void InitGLFW();
void InitGLEW();
void InitTextLabels();

void Start();
void Update();
void Render();
int Cleanup();

void HandleKeymapActions();
void HandleMouseVisible();

GameObject* CubeObject = nullptr;
GameObject* CubeObject2 = nullptr;
GameObject* PyramidObject = nullptr;
NewMesh* CubeMesh = nullptr;
NewMesh* PyramidMesh = nullptr;
NewMesh* QuadMesh = nullptr;
NewMesh* SphereMesh = nullptr;

std::vector<TextLabel*> TextLabels{};
std::map<GLchar, FontChar> m_Arial;
Camera* SceneCamera = nullptr;
GLFWwindow* RenderWindow = nullptr;


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

		if ((float)_xpos > TextLabels[5]->GetBounds().x
			&& (float)_xpos < TextLabels[5]->GetBounds().y
			&& (float)_ypos < TextLabels[5]->GetBounds().z
			&& (float)_ypos > TextLabels[5]->GetBounds().w)
		{
			TextLabels[5]->SetColour({ 0.0f,1.0f,0.0f,1.0f });
		}
		else
		{
			TextLabels[5]->SetColour({ 0.0f,0.0f,0.0f,1.0f });
		}
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

	if (PyramidObject)
		PyramidObject->KeyboardInput(Keypresses);
}

static inline void CharacterCallback(GLFWwindow* _renderWindow, unsigned _uniCode)
{
	if (EnterUsername)
	{
		// If Name Does Not Exceed Bounds
		if (TextLabels[0]->GetBounds().y < TextLabels[0]->GetRightClip())
		{
			// Grab Entered Character
			Username += (char)_uniCode;

			// Update Username Text
			TextLabels[0]->SetText(Username);
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

void InitTextLabels()
{
	TextLabels.emplace_back(new TextLabel(&WindowSize, "Username", m_Arial, DeltaTime, { (float)WindowSize.x / 2.0f, 50.0f }));
	TextLabels.back()->SetClip(350.0f, 650.0f);
	TextLabels.back()->SetScale({ 0.25f, 0.25f });

	TextLabels.emplace_back(new TextLabel(&WindowSize, "Enter To Type UserName ", m_Arial, DeltaTime, { 0.0f, 15.0f }, { 0.8f, 0.2f , 1.0f, 1.0f }));
	TextLabels.back()->SetClip(0.0f, 500.0f);
	TextLabels.back()->SetScale({ 0.25f, 0.25f });
	TextLabels.back()->SetScrolling(true);

	TextLabels.emplace_back(new TextLabel(&WindowSize, "BackSpace To Delete Last Character ", m_Arial, DeltaTime, { (float)WindowSize.x / 2.0f, 15.0f }, { 0.8f, 0.1f , 0.1f, 1.0f }));
	TextLabels.back()->SetClip(500.0f, 1000.0f);
	TextLabels.back()->SetScale({ 0.25f, 0.25f });
	TextLabels.back()->SetScrolling(true);
	TextLabels.back()->SetScrollingRight(false);
	TextLabels.back()->SetScrollSpeed(0.5f);

	TextLabels.emplace_back(new TextLabel(&WindowSize, "TAB To Toggle Mouse", m_Arial, DeltaTime, { 250.0f, (float)WindowSize.y - 35.0f }));
	TextLabels.back()->SetClip(0.0f, 450.0f);
	TextLabels.back()->SetScale({ 0.25f, 0.25f });

	TextLabels.emplace_back(new TextLabel(&WindowSize, "CAPSLOCK To Toggle Wireframe", m_Arial, DeltaTime, { 250.0f, (float)WindowSize.y - 65.0f }));
	TextLabels.back()->SetClip(0.0f, 500.0f);
	TextLabels.back()->SetScale({ 0.25f, 0.25f });

	TextLabels.emplace_back(new TextLabel(&WindowSize, "Hover Over Me", m_Arial, DeltaTime, { 800.0f, (float)WindowSize.y - 105.0f }));
	TextLabels.back()->SetClip(540.0f, 1100.0f);
	TextLabels.back()->SetScale({ 0.25f, 0.25f });

	TextLabels.emplace_back(new TextLabel(&WindowSize, "ESC To Close Window", m_Arial, DeltaTime, { 800.0f, (float)WindowSize.y - 55.0f }));
	TextLabels.back()->SetClip(540.0f, 1100.0f);
	TextLabels.back()->SetScale({ 0.25f, 0.25f });
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

	m_Arial = FontLoader::LoadFont("Resources/Fonts/ARIAL.TTF");

	// Create The Scene Camera
	SceneCamera = new Camera(WindowSize, Keypresses, {0,0,10});

	CubeMesh = new NewMesh(SHAPE::CUBE);
	PyramidMesh = new NewMesh(SHAPE::PYRAMID);
	QuadMesh = new NewMesh(4);
	SphereMesh = new NewMesh(SHAPE::SPHERE);

	CubeObject = new GameObject(*SceneCamera, DeltaTime, { 0,0,-1 });
	CubeObject->SetShader(ShaderLoader::CreateShader("Resources/Shaders/basic.vert", "Resources/Shaders/basic.frag"));
	CubeObject->SetMesh(CubeMesh);
	CubeObject->SetActiveTextures({ TextureLoader::LoadTexture("Resources/Textures/Rayman.jpg") });

	CubeObject2 = new GameObject(*SceneCamera, DeltaTime, { 0,0,-2 });
	CubeObject2->SetShader(ShaderLoader::CreateShader("Resources/Shaders/basic.vert", "Resources/Shaders/basic.frag"));
	CubeObject2->SetMesh(CubeMesh);
	CubeObject2->SetActiveTextures({ TextureLoader::LoadTexture("Resources/Textures/path.jpg") });

	PyramidObject = new GameObject(*SceneCamera, DeltaTime, { 0,0,0 });
	PyramidObject->SetShader(ShaderLoader::CreateShader("Resources/Shaders/basic.vert", "Resources/Shaders/basic.frag"));
	PyramidObject->SetMesh(SphereMesh);
	PyramidObject->SetActiveTextures({ TextureLoader::LoadTexture("Resources/Textures/Rayman.jpg")});

	InitTextLabels();
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

		// Handle Camera Vectors
		if (SceneCamera)
		{
			SceneCamera->Movement(DeltaTime);
		}

		float xScale = ClampedSin((float)glfwGetTime(), 20, 2, 0.3f);
		float yScale = ClampedSin((float)glfwGetTime(), 20, 2, 0.3f);
		TextLabels.back()->SetScale({ xScale,yScale });

		for (auto& textLabel : TextLabels)
		{
			textLabel->Update();
		}

		PyramidObject->Update();
		CubeObject->Update();
		CubeObject2->Update();

		CubeObject->RotateAround({ 0,0,0 }, { 0,1,0 }, (float)DeltaTime);
		CubeObject2->RotateAround({ 0,0,0 }, { 1,0,0 }, (float)DeltaTime);

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

	CubeObject->Draw();
	CubeObject2->Draw();
	PyramidObject->Draw();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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

	if (QuadMesh != nullptr)
		delete QuadMesh;
	QuadMesh = nullptr;

	if (PyramidMesh != nullptr)
		delete PyramidMesh;
	PyramidMesh = nullptr;

	if (SphereMesh != nullptr)
		delete SphereMesh;
	SphereMesh = nullptr;

	if (PyramidObject != nullptr)
		delete PyramidObject;
	PyramidObject = nullptr;

	if (CubeObject != nullptr)
		delete CubeObject;
	CubeObject = nullptr;

	if (CubeObject2 != nullptr)
		delete CubeObject2;
	CubeObject2 = nullptr;

	for (auto& textLabel : TextLabels)
	{
		if (textLabel)
			delete textLabel;
		textLabel = nullptr;
	}
	TextLabels.clear();
	m_Arial.clear();

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

				TextLabels[0]->SetText(Username);

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
					TextLabels[0]->SetText(Username);
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






