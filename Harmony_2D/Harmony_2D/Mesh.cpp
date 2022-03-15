#include "Mesh.h"

Mesh::Mesh(Camera& _camera, double& _deltaTime)
{
	m_Camera = &_camera;
	m_DeltaTime = &_deltaTime;
	Init();
}

Mesh::~Mesh()
{
	// Unbind
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}
	// Delete
	{
		glDeleteBuffers(1, &UniformBufferID);
		glDeleteVertexArrays(1, &VertexArrayID);
		glDeleteBuffers(1, &VertBufferID);
		glDeleteBuffers(1, &IndexBufferID);
		//glDeleteProgram(ShaderID);
	}
	m_Camera = nullptr;
	m_DeltaTime = nullptr;
}

void Mesh::Init()
{
	// Indices
	GenerateQuadIndices();

	// Vertices
	m_Vertices.push_back({ glm::vec3{-0.5f,   0.5f, 0.0f}, glm::vec2{0.0f,1.0f} }); // Top Left
	m_Vertices.push_back({ glm::vec3{-0.5f,  -0.5f, 0.0f}, glm::vec2{0.0f,0.0f} }); // Bottom Left
	m_Vertices.push_back({ glm::vec3{ 0.5f,  -0.5f, 0.0f}, glm::vec2{1.0f,0.0f} }); // Bottom Right
	m_Vertices.push_back({ glm::vec3{ 0.5f,   0.5f, 0.0f}, glm::vec2{1.0f,1.0f} }); // Top Right
	
	m_ActiveTextures.emplace_back(TextureLoader::LoadTexture("Resources/Textures/Capguy_Walk.png"));

	// Shader
	ShaderID = ShaderLoader::CreateShader("Resources/Shaders/basic.vert", "Resources/Shaders/basic.frag");
	glUseProgram(ShaderID);

	// Vertex Array
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Vertex Buffer
	glGenBuffers(1, &VertBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, VertBufferID);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);

	// Index Buffer
	glGenBuffers(1, &IndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);

	// Layouts
	glBindBuffer(GL_ARRAY_BUFFER, VertBufferID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoords)));

	// Uniform Buffer
	glGenBuffers(1, &UniformBufferID);
	unsigned matrixBlockIndex = glGetUniformBlockIndex(ShaderID, "Matrices");
	glUniformBlockBinding(ShaderID, matrixBlockIndex, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, UniformBufferID);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, matrixBlockIndex, UniformBufferID, 0, 2 * sizeof(glm::mat4));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

void Mesh::Draw()
{
	// Bind
	glUseProgram(ShaderID);
	glBindVertexArray(VertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);

	// If Not Frame Buffer
	if (m_Camera)
	{
		ProjectionMat = m_Camera->GetProjectionMatrix();
		ViewMat = m_Camera->GetViewMatrix();

		float time = glfwGetTime();
		//m_Transform.scale = { ((sin(time) / 2) + 0.5f) ,((sin(time) / 2) + 0.5f) ,((sin(time) / 2) + 0.5f) };
		//m_Transform.rotation_axis = { ((sin(time)) + 0.5f) ,((sin(time) / 2) + 0.5f) ,((sin(time) / 4) + 0.5f) };
		//m_Transform.rotation_value = ((sin(time * 5)) + 0.5f);

		ScaleToTexture();
		{
			// Bind
			glBindBuffer(GL_UNIFORM_BUFFER, UniformBufferID);
			// Stream In Proj Mat
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &ProjectionMat[0]);
			// Stream In View Mat
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &ViewMat[0]);
			// Unbind
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		if (m_Animated)
		{
			for (auto& item : m_Vertices)
			{
				if (item.texCoords.x > 0)
				{
					item.texCoords.x = 0.25f;
				}
			}

			glBindBuffer(GL_ARRAY_BUFFER, VertBufferID);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex), m_Vertices.data());
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		ShaderLoader::SetUniformMatrix4fv(ShaderID, "Model", m_Transform.tranform);
		ShaderLoader::SetUniform1f(ShaderID, "Time", time);
		ShaderLoader::SetUniform1i(ShaderID, "Id", m_ObjectID);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ActiveTextures[0].ID);
		ShaderLoader::SetUniform1i(ShaderID, "Diffuse", 0);
	}

	// Draw
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, nullptr);

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Mesh::GenerateQuadIndices(int _numberOfQuads)
{
	for (int i = 0; i < _numberOfQuads; i++)
	{
		m_Indices.push_back(0 + (3 * i));
		m_Indices.push_back(1 + (3 * i));
		m_Indices.push_back(2 + (3 * i));

		m_Indices.push_back(0 + (3 * i));
		m_Indices.push_back(2 + (3 * i));
		m_Indices.push_back(3 + (3 * i));
	}
}

void Mesh::ScaleToTexture()
{
	m_Transform.scale = { m_ActiveTextures[0].Dimensions.x / 2,m_ActiveTextures[0].Dimensions.y/2,0};
	UpdateModelValueOfTransform(m_Transform);
}
