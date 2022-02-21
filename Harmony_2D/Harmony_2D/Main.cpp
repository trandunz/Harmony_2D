#include "Square.h"

static GLFWwindow* m_RenderWindow = nullptr;
static std::map<int, bool> m_Keypresses;

static inline void ErrorCallback(int _error, const char* _description)
{
	Print("Error: %s\n");
	Print(_description);
}

static inline void CursorPositionCallback(GLFWwindow* _renderWindow, double _xPos, double _yPos)
{
}

static inline void MouseButtonCallback(GLFWwindow* _renderWindow, int _button, int _action, int _mods)
{
}

static inline void KeyCallback(GLFWwindow* _renderWindow, int _key, int _scanCode, int _action, int _mods)
{
	// Collect Input
	if (_action == GLFW_PRESS)
		m_Keypresses[_key] = true;
	else if (_action == GLFW_RELEASE)
		m_Keypresses[_key] = false;
}

static inline void ScrollCallback(GLFWwindow* _renderWindow, double _xOffset, double _yOffset)
{
}

static void InitGLFW();
static void InitGLEW();
void Start();
void Update();
int Cleanup();

Square* m_Square = nullptr;

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
	m_RenderWindow = glfwCreateWindow(1920, 1080, "Harmony2D v0.01", NULL, NULL);

	// Set Context To New Window
	glfwMakeContextCurrent(m_RenderWindow);

	glfwSetKeyCallback(m_RenderWindow, KeyCallback);
	glfwSetCursorPosCallback(m_RenderWindow, CursorPositionCallback);
	glfwSetMouseButtonCallback(m_RenderWindow, MouseButtonCallback);
	glfwSetScrollCallback(m_RenderWindow, ScrollCallback);
}

void InitGLEW()
{
	// Init Glew
	if (glewInit() != GLEW_OK)
		Print("Failed to Initalise GLEW");

	// Set Clear Color / Background
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void Start()
{
	InitGLFW();
	InitGLEW();


	m_Square = new Square();
}

void Update()
{
	while (!glfwWindowShouldClose(m_RenderWindow))
	{
		// Clear Frame Buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update
		for (auto& item : m_Keypresses)
		{
			if (item.second == true)
			{
				switch (item.first)
				{
				case GLFW_KEY_TAB:
				{
					if (m_Square == nullptr)
					{
						m_Square = new Square();
					}
					else 
					{
						delete m_Square;
						m_Square = nullptr;
					}
						
					item.second = false;
					break;
				}
				case GLFW_KEY_ESCAPE:
				{
					glfwSetWindowShouldClose(m_RenderWindow, GLFW_TRUE);

					item.second = false;
					break;
				}
				default:
					break;
				}
			}
		}

		// Draw
		if (m_Square != nullptr)
			m_Square->Draw();

		// Swap Buffers
		glfwSwapBuffers(m_RenderWindow);

		// Poll Events
		glfwPollEvents();
	}
}

int Cleanup()
{
	if (m_Square != nullptr)
		delete m_Square;
	m_Square = nullptr;

	// Cleanup GLFW
	glfwDestroyWindow(m_RenderWindow);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}


