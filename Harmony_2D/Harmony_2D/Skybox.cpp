#include "Skybox.h"

Skybox::Skybox(Camera* _activeCamera, Texture _cubemapTexture)
{
	m_ActiveCamera = _activeCamera;
	m_CubemapTexture = _cubemapTexture;
	SetScale({1000,1000,1000});

	m_ShaderID = ShaderLoader::CreateShader("Skybox.vert","Skybox.frag");

	CreateCubeVAO();
}

Skybox::~Skybox()
{
	if (m_ActiveCamera)
		m_ActiveCamera = nullptr;
}

void Skybox::Update(float& _dt)
{
	
}

void Skybox::Draw()
{
	glUseProgram(m_ShaderID);

	// Set Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture.ID);
	ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Texture0", 0);

	// Set PVM Matrix
	if (m_ActiveCamera)
		ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "PVMMatrix", m_ActiveCamera->GetPVMatrix() * m_Transform.transform);

	glBindVertexArray(m_VertexArrayID);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glUseProgram(0);
}

void Skybox::SetTexture(Texture _cubemapTexture)
{
	m_CubemapTexture = _cubemapTexture;
}

void Skybox::SetActiveCamera(Camera* _camera)
{
	m_ActiveCamera = _camera;
}

Texture Skybox::GetTextureID()
{
	return m_CubemapTexture;
}

void Skybox::SetTranslation(glm::vec3 _newPosition)
{
	m_Transform.translation = _newPosition;
	UpdateModelValueOfTransform(m_Transform);
}

void Skybox::Translate(glm::vec3 _translation)
{
	m_Transform.translation += _translation;
	UpdateModelValueOfTransform(m_Transform);
}

void Skybox::SetRotation(glm::vec3 _axis, float _degrees)
{
	m_Transform.rotation_axis = _axis;
	m_Transform.rotation_value = glm::radians(_degrees);
	UpdateModelValueOfTransform(m_Transform);
}

void Skybox::Rotate(glm::vec3 _axis, float _degrees)
{
	m_Transform.rotation_axis = _axis;
	m_Transform.rotation_value += glm::radians(_degrees);
	UpdateModelValueOfTransform(m_Transform);
}

void Skybox::SetScale(glm::vec3 _newScale)
{
	m_Transform.scale = _newScale;
	UpdateModelValueOfTransform(m_Transform);
}

void Skybox::Scale(glm::vec3 _scaleFactor)
{
	m_Transform.scale *= _scaleFactor;
	UpdateModelValueOfTransform(m_Transform);
}

void Skybox::CreateCubeVAO()
{
	GLuint vertexBufferID;
	GLuint indexBufferID;

	glm::vec3 vertexPositions[]
	{
		{ -0.5f,  0.5f, 0.5f },
		{-0.5f,  -0.5f, 0.5f},
		{0.5f,  -0.5f, 0.5f},

		{0.5f,  0.5f, 0.5f},
		{0.5f,  0.5f, -0.5f},
		{0.5f,  -0.5f, -0.5f},

		{-0.5f,  -0.5f, -0.5f},
		{-0.5f,  0.5f, -0.5f},
		{0.5f,  0.5f, 0.5f},

		{0.5f,  -0.5f, 0.5f},
		{0.5f,  -0.5f, -0.5f},
		{0.5f,  0.5f, -0.5f},

		{-0.5f,  0.5f, -0.5f},
		{-0.5f,  -0.5f, -0.5f},
		{-0.5f,  -0.5f, 0.5f},

		{-0.5f,  0.5f, 0.5f},
		{-0.5f,  0.5f, -0.5f},
		{-0.5f,  0.5f, 0.5f},

		{0.5f,  0.5f, 0.5f},
		{0.5f,  0.5f, -0.5f},
		{-0.5f,  -0.5f, 0.5f},

		{-0.5f,  -0.5f, -0.5f},
		{0.5f,  -0.5f, -0.5f},
		{0.5f,  -0.5f, 0.5f} 
	};

	unsigned indicesValues[36];
	unsigned element{ 0 };
	for (int i = 0; i < 6; i++)
	{
		indicesValues[element++] = (4 * i);
		indicesValues[element++] = ((4 * i) + 2);
		indicesValues[element++] = ((4 * i) + 1);

		indicesValues[element++] = (4 * i);
		indicesValues[element++] = ((4 * i) + 3);
		indicesValues[element++] = ((4 * i) + 2);
	}
	

	// Vertex Array
	glGenVertexArrays(1, &m_VertexArrayID);
	glBindVertexArray(m_VertexArrayID);

	// Vertex Buffer
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(GLfloat), &vertexPositions[0], GL_STATIC_DRAW);

	// Index Buffer
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned), &indicesValues[0], GL_STATIC_DRAW);

	// Layouts
	// Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
