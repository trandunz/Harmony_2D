#include "Mesh.h"

static double deltaTime = 0.0;	// Time between current frame and last frame
static double lastFrame = 0.0; // Time of last frame
static unsigned int frameCounter = 0;
static bool firstMouse = true;
static bool activeMouse = false;
static double lastX = 0, lastY = 0;

static Camera* m_Camera;
static unsigned FrameBufferTexture;
static unsigned FrameBufferDepthTexture;
static unsigned FrameBufferIDTexture;
static unsigned FrameBufferID;
static unsigned RenderBufferID;
static GLFWwindow* m_RenderWindow = nullptr;
static std::map<int, bool> m_Keypresses;
static GLfloat BackgroundColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };

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
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	GLfloat* pixels = new GLfloat[4];
	glReadPixels(1080 - lastX, 1080 - lastY, 1, 1, GL_RGBA, GL_FLOAT, pixels);
	std::string output = "";
	output += std::to_string(pixels[0]);
	output += "|";
	output += std::to_string(pixels[1]);
	output += "|";
	output += std::to_string(pixels[2]);
	output += "|";
	output += std::to_string(pixels[3]);
	Print(output);
	delete[] pixels;
	pixels = nullptr;

	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

static void InitGLFW();
static void InitGLEW();
void InitFrameBufferNDSA();
void InitFrameBufferDSA();

void Start();
void Update();
int Cleanup();

Mesh* m_FrameBufferMesh = nullptr;
std::vector<Mesh*> m_Meshes;

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

void InitFrameBufferNDSA()
{
	glGenFramebuffers(1, &FrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);

	glGenTextures(1, &FrameBufferTexture);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, FrameBufferTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1080, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FrameBufferTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		Print("FrameBuffer Error ");
		std::cout << status << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void InitFrameBufferDSA()
{
	glCreateFramebuffers(1, &FrameBufferID);

	glCreateTextures(GL_TEXTURE_2D, 1, &FrameBufferTexture);
	glCreateTextures(GL_TEXTURE_2D, 1, &FrameBufferIDTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTextureStorage2D(FrameBufferTexture, 1, GL_RGBA8, 1080, 1080);
	glNamedFramebufferTexture(FrameBufferID, GL_COLOR_ATTACHMENT0, FrameBufferTexture, 0);

	// ID's
	glTextureStorage2D(FrameBufferIDTexture, 1, GL_RGBA8, 1080, 1080);
	glNamedFramebufferTexture(FrameBufferID, GL_COLOR_ATTACHMENT1, FrameBufferIDTexture, 0);

	// ID's
	glTextureStorage2D(FrameBufferDepthTexture, 1, GL_DEPTH24_STENCIL8, 1080, 1080);
	glNamedFramebufferTexture(FrameBufferID, GL_DEPTH_STENCIL_ATTACHMENT, FrameBufferDepthTexture, 0);

	auto status = glCheckNamedFramebufferStatus(FrameBufferID, GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		Print("FrameBuffer Error ");
		std::cout << status << std::endl;
	}
}

void Start()
{
	InitGLFW();
	InitGLEW();

	InitFrameBufferDSA();

	// Set Clear Color / Background
	glClearColor(BackgroundColor[0], BackgroundColor[1], BackgroundColor[2], BackgroundColor[3]);

	m_FrameBufferMesh = new Mesh(FrameBufferTexture);
	
	m_Camera = new Camera(m_Keypresses);

	for (int i = 0; i < 5; i++)
	{
		m_Meshes.push_back(new Mesh(*m_Camera));
	}

	m_Meshes[4]->GetTransform().translation = { 0,1,-1 };
}

void Update()
{
	while (!glfwWindowShouldClose(m_RenderWindow))
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);

		glClearNamedFramebufferfv(FrameBufferID, GL_COLOR, 0, BackgroundColor);

		// Clear Frame Buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

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

		// Draw
		for (auto& item : m_Meshes)
		{
			item->Draw();
		}




		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glBindTextureUnit(0, FrameBufferTexture);

		if (m_FrameBufferMesh != nullptr)
			m_FrameBufferMesh->Draw();

		// Swap Buffers
		glfwSwapBuffers(m_RenderWindow);

		// Poll Events
		glfwPollEvents();


	}
}

int Cleanup()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &FrameBufferID);
	glDeleteTextures(1, &FrameBufferTexture);

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


