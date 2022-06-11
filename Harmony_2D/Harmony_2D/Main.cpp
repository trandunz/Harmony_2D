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
#include "Skybox.h"

glm::ivec2 WindowSize{ 1000,1000 };

float DeltaTime = 0.0, LastFrame = 0.0;

bool IsMouseVisible = false;

std::vector <GameObject*> GameObjects{};

Camera* SceneCamera = nullptr;
LightManager* LightManagerObject = nullptr;

Skybox* SceneSkybox = nullptr;

Mesh* CubeMesh = nullptr;
Mesh* SphereMesh = nullptr;

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

void CreateTextLabels();
void CreateLights();
void CreateObjects();

void HandleKeymapActions();
void HandleMouseVisible();

/// <summary>
/// Calculates The Time Taken Between The Last Frame And The Current One
/// </summary>
static inline void CalculateDeltaTime()
{
	// Calculate DeltaTime
	float currentFrame = (float)glfwGetTime();
	DeltaTime = currentFrame - LastFrame;
	LastFrame = currentFrame;

	// Update Window Title With FPS And FrameTime
	glfwSetWindowTitle(RenderWindow, ("GD1P04 - Assessment 3 | " + std::to_string(1/DeltaTime) + " fps / " + std::to_string(DeltaTime * 1000) + " ms").c_str());
}

/// <summary>
/// GLFW Mouse Position Callback Function.
/// Activates when the mouse moves.
/// </summary>
/// <param name="_renderWindow"></param>
/// <param name="_xpos"></param>
/// <param name="_ypos"></param>
static inline void CursorPositionCallback(GLFWwindow* _renderWindow, double _xpos, double _ypos)
{
	if (SceneCamera && !IsMouseVisible)
		SceneCamera->MouseLook(DeltaTime, {(float)_xpos , (float) _ypos});
}

/// <summary>
/// GLFW Key Callback Function.
/// Actives when key is pressed.
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
/// Initalizes GLFW With Window Hints, 
/// Set Number of MSAA Samples To 4,
/// Create GLFW Window,
/// Set Callback Functions
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
	glfwSetCursorPosCallback(RenderWindow, CursorPositionCallback);
}

/// <summary>
/// Initalizes GLEW, 
/// Enable MultiSample Anti-Aliasing,
/// Enable Backface Culling,
/// Enable Blending,
/// Enable One Minus Alpha Blending, 
/// Enable Depth Buffer,
/// Enable Seamless Cubemap Textures,
/// Sets Clear Color For Default FrameBuffer (Grey)
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
	// Enable Seemless Cubemaps
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
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

	CreateTextLabels();

	// Create The Scene Camera
	SceneCamera = new Camera(WindowSize, {0,0,10});

	CubeMesh = new Mesh(SHAPE::CUBE, GL_CCW);
	SphereMesh = new Mesh(SHAPE::SPHERE, GL_CCW);

	LightManagerObject = new LightManager(*SceneCamera, 2, 1, 1);
	LightManagerObject->SetLightMesh(SphereMesh);

	CreateLights();

	SceneSkybox = &Skybox::GetInstance(SceneCamera, TextureLoader::LoadCubemap(
		{
			"MountainOutpost/Right.jpg",
			"MountainOutpost/Left.jpg",
			"MountainOutpost/Up.jpg",
			"MountainOutpost/Down.jpg",
			"MountainOutpost/Back.jpg",
			"MountainOutpost/Front.jpg"
		}));

	CreateObjects();

	for (auto& gameObject : GameObjects)
	{
		gameObject->SetLightManager(*LightManagerObject);
		gameObject->SetSkyboxTexture(SceneSkybox->GetTextureID());
	}
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

		for (auto& gameObject : GameObjects)
			gameObject->Update(DeltaTime);

		// Main Render
		Render();
	}
}

/// <summary>
/// Main Render Function That Draws To The Screen At The End Of Every Frame
/// </summary>
void Render()
{
	// Draw Skybox
	if (SceneSkybox)
		SceneSkybox->Draw();

	// Draw All PointLights
	if (LightManagerObject)
		LightManagerObject->Draw();

	// Draw All Objects
	for (auto& gameObject : GameObjects)
		gameObject->Draw();

	// Draw All TextLabels
	for (auto& textLabel : TextLabels)
		textLabel->Draw();

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
	// Cleanup Light Manahger
	if (LightManagerObject)
		delete LightManagerObject;
	LightManagerObject = nullptr;

	// Null Skybox Pointer
	SceneSkybox = nullptr;

	// Cleanup Cube Mesh
	if (CubeMesh)
		delete CubeMesh;
	CubeMesh = nullptr;

	// Cleanup Sphere Mesh
	if (SphereMesh)
		delete SphereMesh;
	SphereMesh = nullptr;

	// Cleanup All Objects
	for (auto& gameObject : GameObjects)
	{
		if (gameObject)
			delete gameObject;
		gameObject = nullptr;
	}
	GameObjects.clear();

	// Cleanup All TextLabels
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
/// Handles Creation Of Text Labels
/// </summary>
void CreateTextLabels()
{
	TextLabels.emplace_back(new TextLabel(WindowSize, "ESC To Close Window", ArialFont, DeltaTime, { 120,WindowSize.y - 30 }, { 1,1,1,1 }, { 0.2f, 0.2f }));
	TextLabels.emplace_back(new TextLabel(WindowSize, "TAB To Toggle Cursor", ArialFont, DeltaTime, { 120,WindowSize.y - 60 }, { 1,1,1,1 }, { 0.2f, 0.2f }));
}

/// <summary>
/// Handles Creation Of Lights
/// </summary>
void CreateLights()
{
	LightManagerObject->CreatePointLight({
		{ -5.0f, 0.0f, -2.0f }, // Starting Position
		{1.0f, 0.0f, 0.0f}, // Color
		1.0f // Specular Strength
		});

	LightManagerObject->CreatePointLight({
		{ 5.0f, 0.0f, -2.0f }, // Starting Position
		{0.0f, 0.0f, 1.0f}, // Color
		1.0f // Specular Strength
		});

	LightManagerObject->CreateDirectionalLight({
		{ 0.0f, 10.0f, -2.0f }, // Starting Position
		{1.0f, 1.0f, 1.0f}, // Color
		1.0f // Specular Strength
		});

	LightManagerObject->CreateSpotLight({
		true, // Is attachted to Camera
		{ 0.0f, 10.0f, -2.0f }, // Starting Position
		{1.0f, 1.0f, 1.0f}, // Color
		1.0f // Specular Strength
		});
}

/// <summary>
/// Handles Creation Of Objects
/// </summary>
void CreateObjects()
{
	float x = 0;
	float y = 0;
	float z = 0;
	// Create 10 Lit Spheres
	for (int i = 0; i < 10; i++)
	{
		x = cos(glm::radians((float)(rand() % 360))) * sin(glm::radians((float)(rand() % 360)));
		y = cos(glm::radians((float)(rand() % 360)));
		z = sin(glm::radians((float)(rand() % 360))) * sin(glm::radians((float)(rand() % 360)));

		GameObjects.push_back(new GameObject(*SceneCamera, { x * 4.0f,y * 4.0f,z * 4.0f }));
		GameObjects.back()->SetShader("Normals3D.vert", "BlinnFong3D.frag");
		GameObjects.back()->SetMesh(SphereMesh);
	}

	// Create Rim Lighting Sphere
	GameObjects.push_back(new GameObject(*SceneCamera, { 0,0,0 }));
	GameObjects.back()->SetShader("Normals3D.vert", "BlinnFong3D.frag");
	GameObjects.back()->SetMesh(SphereMesh);
	GameObjects.back()->SetRimLighting(true);
	GameObjects.back()->SetActiveTextures({
		TextureLoader::LoadTexture("Crate.jpg"),
		});

	// Create Reflection Sphere
	GameObjects.push_back(new GameObject(*SceneCamera, { -1.0f,0,0 }));
	GameObjects.back()->SetShader("Normals3D.vert", "Reflection.frag");
	GameObjects.back()->SetMesh(SphereMesh);

	// Creation Reflection Map Cube
	GameObjects.push_back(new GameObject(*SceneCamera, { 1.0f,0,0 }));
	GameObjects.back()->SetShader("Normals3D.vert", "ReflectionMap.frag");
	GameObjects.back()->SetMesh(CubeMesh);
	GameObjects.back()->SetActiveTextures({
		TextureLoader::LoadTexture("Crate.jpg"),
		TextureLoader::LoadTexture("Crate-Reflection.png")
		});

	// Create Reflection Map Cube With Combined Lighting
	GameObjects.push_back(new GameObject(*SceneCamera, { 2.0f,0,0 }));
	GameObjects.back()->SetShader("Normals3D.vert", "BlinnFong3D_Reflection.frag");
	GameObjects.back()->SetMesh(CubeMesh);
	GameObjects.back()->SetActiveTextures({
		TextureLoader::LoadTexture("Crate.jpg"),
		TextureLoader::LoadTexture("Crate-Reflection.png")
		});
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
/// Disable Or Enables The Mouse Cursor Depending On Bool IsMouseVisible
/// </summary>
void HandleMouseVisible()
{
	if (IsMouseVisible)
		glfwSetInputMode(RenderWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode(RenderWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}






