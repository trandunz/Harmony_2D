#include "Mesh.h"
#include "FrameBuffer.h"

static double deltaTime = 0.0;	// Time between current frame and last frame
static double lastFrame = 0.0; // Time of last frame
static unsigned int frameCounter = 0;
static bool firstMouse = true;
static bool activeMouse = false;
static double lastX = 0, lastY = 0;

static Camera* m_Camera;

static unsigned RenderBufferID;
static GLFWwindow* m_RenderWindow = nullptr;
static std::map<int, bool> m_Keypresses;


static void InitGLFW();
static void InitGLEW();

void Start();
void Update();
int Cleanup();

Mesh* m_FrameBufferMesh = nullptr;
std::vector<Mesh*> m_Meshes;

static void CalculateDeltaTime()
{
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	frameCounter++;
}

static inline void ErrorCallback(int _error, const char* _description)
{
	Print("Error: %s\n");
	Print(_description);
}

static inline void CursorPositionCallback(GLFWwindow* _renderWindow, double _xPos, double _yPos)
{
	if (firstMouse)
	{
		lastX = _xPos;
		lastY = _yPos;
		firstMouse = false;
	}
	auto xoffset = _xPos - lastX;
	lastX = _xPos;

	auto yoffset = lastY - _yPos;
	lastY = _yPos;



	if (m_Camera && !activeMouse)
		m_Camera->ProcessMouse(xoffset, yoffset);
}

static inline void MouseButtonCallback(GLFWwindow* _renderWindow, int _button, int _action, int _mods)
{
	FrameBuffer::GrabIDUnderMouse(lastX, lastY);
}

static inline void KeyCallback(GLFWwindow* _renderWindow, int _key, int _scanCode, int _action, int _mods)
{
	// Collect Input
	if (_action == GLFW_PRESS)
		m_Keypresses[_key] = true;
	else if (_action == GLFW_RELEASE)
		m_Keypresses[_key] = false;

	if (m_Camera)
		m_Camera->Input();
}

static inline void ScrollCallback(GLFWwindow* _renderWindow, double _xOffset, double _yOffset)
{
	if (m_Camera)
		m_Camera->ProcessScroll(_yOffset);
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
	m_RenderWindow = glfwCreateWindow(1080, 1080, "Harmony2D v0.01", NULL, NULL);

	// Set Context To New Window
	glfwMakeContextCurrent(m_RenderWindow);

	glfwSetKeyCallback(m_RenderWindow, KeyCallback);
	glfwSetCursorPosCallback(m_RenderWindow, CursorPositionCallback);
	glfwSetMouseButtonCallback(m_RenderWindow, MouseButtonCallback);
	glfwSetScrollCallback(m_RenderWindow, ScrollCallback);

	if (activeMouse)
		glfwSetInputMode(m_RenderWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode(m_RenderWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

	FrameBuffer::InitFrameBufferDSA();

	// Set Clear Color / Background
	glClearColor(FrameBuffer::BackgroundColor[0], FrameBuffer::BackgroundColor[1], FrameBuffer::BackgroundColor[2], FrameBuffer::BackgroundColor[3]);

	m_FrameBufferMesh = new Mesh(FrameBuffer::FrameBufferTexture);
	
	m_Camera = new Camera(m_Keypresses);

	for (int i = 0; i < 5; i++)
	{
		m_Meshes.push_back(new Mesh(*m_Camera));
	}

	m_Meshes[0]->GetTransform().translation = { 0,5,-4 };
}

void Update()
{
	while (!glfwWindowShouldClose(m_RenderWindow))
	{
		FrameBuffer::Bind();

		// Clear Frame Buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		FrameBuffer::ClearTexturesCustom();

		// Update
		for (auto& item : m_Keypresses)
		{
			if (item.second == true)
			{
				switch (item.first)
				{
				case GLFW_KEY_TAB:
				{
					activeMouse = !activeMouse;

					if (activeMouse)
						glfwSetInputMode(m_RenderWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					else
						glfwSetInputMode(m_RenderWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
						
					item.second = false;
					break;
				}
				case GLFW_KEY_ESCAPE:
				{
					glfwSetWindowShouldClose(m_RenderWindow, GLFW_TRUE);

					item.second = false;
					break;
				}
				case GLFW_KEY_K:
				{
					if (m_Meshes[0])
					{
						delete m_Meshes[0];
						m_Meshes[0] = nullptr;
					}
					else
					{
						m_Meshes[0] = new Mesh(*m_Camera);
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

		if (m_Camera)
		{
			m_Camera->Movement(deltaTime);
		}

		// Draw Items To Frame Buffer
		[]() -> void
		{
			for (auto& item : m_Meshes)
			{
				item->Draw();
			}
		}();
		
		// Draw Frame Buffer To Screen
		[]() -> void
		{
			glDisable(GL_DEPTH_TEST);

			FrameBuffer::UnBind();

			if (m_FrameBufferMesh != nullptr)
				m_FrameBufferMesh->Draw();

			glEnable(GL_DEPTH_TEST);
		}();

		// Swap Buffers
		glfwSwapBuffers(m_RenderWindow);

		// Poll Events
		glfwPollEvents();
	}
}

int Cleanup()
{
	FrameBuffer::Cleanup();

	if (m_FrameBufferMesh != nullptr)
		delete m_FrameBufferMesh;
	m_FrameBufferMesh = nullptr;

	for (auto& item : m_Meshes)
	{
		if (item != nullptr)
		{
			delete item;
		}
		item = nullptr;
	}
	m_Meshes.clear();

	if (m_Camera != nullptr)
		delete m_Camera;
	m_Camera = nullptr;

	// Cleanup GLFW
	glfwDestroyWindow(m_RenderWindow);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}






