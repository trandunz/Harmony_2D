// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Mesh.cpp 
// Description : Mesh Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "Mesh.h"

Mesh::Mesh(Camera& _camera, double& _deltaTime, unsigned&& _numberOfSides, std::vector<Texture>&& _textures)
{
	m_Camera = &_camera;
	m_DeltaTime = &_deltaTime;
	m_NumberOfSides = _numberOfSides;

	// Take A Copy Of The Texture Ids And Store Them In Active Textures Array
	for (unsigned i = 0; i < _textures.size(); i++)
	{
		m_ActiveTextures.emplace_back(_textures[i]);
	}

	// Create and initalize the mesh ready for drawing
	Init();
}

Mesh::Mesh(Camera& _camera, double& _deltaTime, SHAPE&& _shape, std::vector<Texture>&& _textures)
{
	m_Camera = &_camera;
	m_DeltaTime = &_deltaTime;
	m_IsShape = true;

	// Take A Copy Of The Texture Ids And Store Them In Active Textures Array
	for (unsigned i = 0; i < _textures.size(); i++)
	{
		m_ActiveTextures.emplace_back(_textures[i]);
	}

	// Create and initalize the mesh ready for drawing
	Init(std::move(_shape));
}

Mesh::Mesh(GLuint&& _vertexArrayID, GLuint&& _indexBufferID, Camera& _camera, double& _deltaTime, SHAPE&& _shape, std::vector<Texture>&& _textures)
{
	m_Camera = &_camera;
	m_DeltaTime = &_deltaTime;
	m_IsShape = true;
	m_Shape = _shape;
	m_VertexArrayID = _vertexArrayID;
	m_IndexBufferID = _indexBufferID;

	// Take A Copy Of The Texture Ids And Store Them In Active Textures Array
	for (unsigned i = 0; i < _textures.size(); i++)
	{
		m_ActiveTextures.emplace_back(_textures[i]);
	}

	// Create and initalize the mesh ready for drawing
	Init(_vertexArrayID);
}

Mesh::Mesh(Camera& _camera, double& _deltaTime, unsigned&& _numberOfSides, unsigned&& _numberOfAnimationFrames, std::vector<Texture>&& _textures)
{
	m_Camera = &_camera;
	m_DeltaTime = &_deltaTime;
	m_NumberOfSides = _numberOfSides;
	m_NumberOfAnimationFrames = _numberOfAnimationFrames;
	m_Animated = true;

	// Take A Copy Of The Texture Ids And Store Them In Active Textures Array
	for (unsigned i = 0; i < _textures.size(); i++)
	{
		m_ActiveTextures.emplace_back(_textures[i]);
	}
	
	// Create and initalize the mesh ready for drawing
	Init();
}

Mesh::Mesh(GLuint&& _vertexArrayID, Camera& _camera, double& _deltaTime, unsigned&& _numberOfSides, std::vector<Texture>&& _textures)
{
	m_Camera = &_camera;
	m_DeltaTime = &_deltaTime;
	m_NumberOfSides = _numberOfSides;
	m_VertexArrayID = _vertexArrayID;

	// Take A Copy Of The Texture Ids And Store Them In Active Textures Array
	for (unsigned i = 0; i < _textures.size(); i++)
	{
		m_ActiveTextures.emplace_back(_textures[i]);
	}

	// Create and initalize the mesh ready for drawing
	Init(_vertexArrayID);
}

Mesh::Mesh(GLuint&& _vertexArrayID, Camera& _camera, double& _deltaTime, unsigned&& _numberOfSides, unsigned&& _numberOfAnimationFrames, std::vector<Texture>&& _textures)
{
	m_Camera = &_camera;
	m_DeltaTime = &_deltaTime;
	m_NumberOfSides = _numberOfSides;
	m_NumberOfAnimationFrames = _numberOfAnimationFrames;
	m_Animated = true;
	m_VertexArrayID = _vertexArrayID;

	// Take A Copy Of The Texture Ids And Store Them In Active Textures Array
	for (unsigned i = 0; i < _textures.size(); i++)
	{
		m_ActiveTextures.emplace_back(_textures[i]);
	}

	// Create and initalize the mesh ready for drawing
	Init(_vertexArrayID);
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
		glDeleteBuffers(1, &m_UniformBufferID);
		glDeleteVertexArrays(1, &m_VertexArrayID);
		glDeleteBuffers(1, &m_VertexBufferID);
		glDeleteBuffers(1, &m_IndexBufferID);
	}
	m_Vertices.clear();
	m_Indices.clear();
	m_ActiveTextures.clear();
	m_Camera = nullptr;
	m_DeltaTime = nullptr;
}

void Mesh::SetAnimationFrame(unsigned&& _frame)
{
	if (m_Animated)
	{
		if (_frame <= m_NumberOfAnimationFrames)
			m_CurrentAnimationFrame = _frame;
		else
			m_CurrentAnimationFrame = m_NumberOfAnimationFrames;
	}
}

void Mesh::SetAnimationFrameTime(float&& _frameTime_s)
{
	m_FrameTime_s = _frameTime_s;
}

void Mesh::SetTextureFadeSpeed(float&& _newSpeed)
{
	m_TextureFadeSpeed = _newSpeed;
}

void Mesh::Init()
{
	// Generate N Sided Polygon
	GeneratePolygonVertices(m_NumberOfSides);
	GeneratePolygonIndices(m_NumberOfSides);

	// Create A Shader And Copy The ID
	m_ShaderID = ShaderLoader::CreateShader("Resources/Shaders/basic.vert", "Resources/Shaders/basic.frag");

	// Vertex Array
	glGenVertexArrays(1, &m_VertexArrayID);
	glBindVertexArray(m_VertexArrayID);

	// Vertex Buffer
	glGenBuffers(1, &m_VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);

	// Index Buffer
	glGenBuffers(1, &m_IndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);

	// Layouts
	// Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// TexCoords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoords)));

	// Uniform Buffer
	// Generate Uniform Buffer
	glGenBuffers(1, &m_UniformBufferID);
	// Get Block Binding Index (Similar To Get Location)
	unsigned matrixBlockIndex = glGetUniformBlockIndex(m_ShaderID, "Matrices");
	// Assign Binding Point To Uniform Block
	glUniformBlockBinding(m_ShaderID, matrixBlockIndex, 0);
	// Bind Uniform Buffer
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferID);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, matrixBlockIndex, m_UniformBufferID, 0, sizeof(glm::mat4));

	// Scale The Mesh To The Texture OR Animation Frame Size
	if (m_Animated)
	{
		SetScaleToAnimationFrameSize();
	}
	else
	{
		ScaleToTexture();
	}

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::Init(SHAPE&& _shape)
{
	// Generate N Sided Polygon
	GenerateShapeVertices(_shape);
	GenerateShapeIndices(_shape);

	// Create A Shader And Copy The ID
	m_ShaderID = ShaderLoader::CreateShader("Resources/Shaders/basic.vert", "Resources/Shaders/basic.frag");

	// Vertex Array
	glGenVertexArrays(1, &m_VertexArrayID);
	glBindVertexArray(m_VertexArrayID);

	// Vertex Buffer
	glGenBuffers(1, &m_VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);

	// Index Buffer
	glGenBuffers(1, &m_IndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_DYNAMIC_DRAW);

	// Layouts
	// Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// TexCoords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoords)));

	// Uniform Buffer
	// Generate Uniform Buffer
	glGenBuffers(1, &m_UniformBufferID);
	// Get Block Binding Index (Similar To Get Location)
	unsigned matrixBlockIndex = glGetUniformBlockIndex(m_ShaderID, "Matrices");
	// Assign Binding Point To Uniform Block
	glUniformBlockBinding(m_ShaderID, matrixBlockIndex, 0);
	// Bind Uniform Buffer
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferID);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, matrixBlockIndex, m_UniformBufferID, 0, sizeof(glm::mat4));

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::Init(GLuint& _vertexArray)
{
	if (m_IsShape)
	{
		// Generate Shape Vertices And Indices
		GenerateShapeVertices(m_Shape);
		GenerateShapeIndices(m_Shape);
	}
	else
	{
		// Generate N Sided Polygon
		GeneratePolygonVertices(m_NumberOfSides);
		GeneratePolygonIndices(m_NumberOfSides);
	}

	m_ShaderID = ShaderLoader::CreateShader("Resources/Shaders/basic.vert", "Resources/Shaders/basic.frag");

	glBindVertexArray(m_VertexArrayID);

	// Uniform Buffer
	// Generate Uniform Buffer
	glGenBuffers(1, &m_UniformBufferID);
	// Get Block Binding Index (Similar To Get Location)
	unsigned matrixBlockIndex = glGetUniformBlockIndex(m_ShaderID, "Matrices");
	// Assign Binding Point To Uniform Block
	glUniformBlockBinding(m_ShaderID, matrixBlockIndex, 0);
	// Bind Uniform Buffer
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferID);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, matrixBlockIndex, m_UniformBufferID, 0, sizeof(glm::mat4));

	if (!m_IsShape)
	{
		// Scale The Mesh To The Texture OR Animation Frame Size
		if (m_Animated)
		{
			SetScaleToAnimationFrameSize();
		}
		else
		{
			ScaleToTexture();
		}
	}

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Mesh::Draw()
{
	// Bind
	glUseProgram(m_ShaderID);
	glBindVertexArray(m_VertexArrayID);

	// Get PVMatrix From Camera
	m_PVMatrix = m_Camera->GetPVMatrix();

	// Bind Uniform Buffer
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferID);
	// Stream In PVMat Data
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &m_PVMatrix[0]);
	// Unbind Uniform Buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//
	// Uniforms
	//
	// Model Matrix
	ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "Model", std::move(m_Transform.tranform));
	// Elapsed Time
	ShaderLoader::SetUniform1f(std::move(m_ShaderID), "Time", (float)glfwGetTime() * m_TextureFadeSpeed);

	if (m_IsShape)
	{
		switch (m_Shape)
		{
		case SHAPE::CUBE:
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);

			std::vector<unsigned> indicesCache(m_Indices);
			glActiveTexture(GL_TEXTURE0);
			ShaderLoader::SetUniform1i(std::move(m_ShaderID), "TextureCount", 1);
			for (int i = 0; i < 6; i++)
			{
				glBindTexture(GL_TEXTURE_2D, m_ActiveTextures[(unsigned)glm::mod((float)i, (float)(m_ActiveTextures.size()))].ID);

				ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Texture0", 0);

				// Draw
				m_Indices = { indicesCache[i * 6], indicesCache[(i * 6) + 1] ,indicesCache[(i * 6) + 2] ,indicesCache[(i * 6) + 3] ,indicesCache[(i * 6) + 4] ,indicesCache[(i * 6) + 5] };
				// Stream In PVMat Data
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned) * m_Indices.size(), m_Indices.data());

				glDrawElements(GL_TRIANGLES, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT, nullptr);
			}
			m_Indices = indicesCache;
			break;
		}
		default:
			break;
		}
	}
	else
	{

		// Textures
		ShaderLoader::SetUniform1i(std::move(m_ShaderID), "TextureCount", (GLint)m_ActiveTextures.size());
		for (unsigned i = 0; i < m_ActiveTextures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_ActiveTextures[i].ID);
			ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Texture" + std::to_string(i), std::move(i));
		}

		// Animated Texture / Sprite Sheet
		if (m_Animated)
		{
			ShaderLoader::SetUniform1i(std::move(m_ShaderID), "IsAnimation", 1);
			ShaderLoader::SetUniform1i(std::move(m_ShaderID), "NumberOfAnimationFrames", m_NumberOfAnimationFrames);
			ShaderLoader::SetUniform1i(std::move(m_ShaderID), "CurrentAnimationFrame", m_CurrentAnimationFrame);

			// If animating then change current frame to next with some delay m_FrameTime_s
			if (m_Animating)
			{
				if (m_AnimationTimer > 0)
				{
					m_AnimationTimer -= (float)*m_DeltaTime;
				}
				else
				{
					m_AnimationTimer = m_FrameTime_s;
					m_CurrentAnimationFrame++;
				}
			}
		}
		else
		{
			ShaderLoader::SetUniform1i(std::move(m_ShaderID), "IsAnimation", 0);
		}

		// Draw
		glDrawElements(GL_TRIANGLES, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT, nullptr);
	}

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

void Mesh::SetPosition(glm::vec3&& _newPos)
{
	m_Transform.translation = _newPos;
	UpdateModelValueOfTransform(m_Transform);
}

void Mesh::SetScale(glm::vec3&& _newScale)
{
	m_Transform.scale = _newScale;
	UpdateModelValueOfTransform(m_Transform);
}

void Mesh::Scale(glm::vec3&& _scaleFactor)
{
	m_Transform.scale *= _scaleFactor;
	UpdateModelValueOfTransform(m_Transform);
}

void Mesh::SetRotation(glm::vec3&& _axis, float&& _degrees)
{
	m_Transform.rotation_axis = _axis;
	m_Transform.rotation_value = glm::radians(_degrees);
	UpdateModelValueOfTransform(m_Transform);
}

void Mesh::Rotate(glm::vec3&& _axis, float&& _degrees)
{
	m_Transform.rotation_axis = _axis;
	m_Transform.rotation_value += glm::radians(_degrees);
	UpdateModelValueOfTransform(m_Transform);
}

void Mesh::RotateAround(glm::vec3&& _position, glm::vec3&& _axis, float&& _degrees)
{
	glm::vec3 direction = glm::abs(_position - m_Transform.translation);
	m_Transform.tranform = glm::translate(m_Transform.tranform, direction);
	m_Transform.tranform = glm::rotate(m_Transform.tranform, _degrees, _axis);
	m_Transform.tranform = glm::translate(m_Transform.tranform, -direction);
	
}

void Mesh::SetTranslation(glm::vec3&& _newPosition)
{
	m_Transform.translation = _newPosition;
	UpdateModelValueOfTransform(m_Transform);
}

void Mesh::Translate(glm::vec3&& _direction)
{
	m_Transform.translation += _direction;
	UpdateModelValueOfTransform(m_Transform);
}

void Mesh::SetScaleToAnimationFrameSize()
{
	SetScale({ m_ActiveTextures[0].Dimensions.x / m_NumberOfAnimationFrames ,m_ActiveTextures[0].Dimensions.y,0 });
}

void Mesh::ToggleAnimating()
{
	m_Animated ? m_Animating = !m_Animating : false;
}

bool Mesh::IsAnimating()
{
	return m_Animated ? m_Animating : false;
}

void Mesh::ScaleToTexture()
{
	SetScale({ m_ActiveTextures[0].Dimensions.x ,m_ActiveTextures[0].Dimensions.y ,0 });
	UpdateModelValueOfTransform(m_Transform);
}

void Mesh::GeneratePolygonVertices(int&& _numberOfSides)
{
	float angle = 0.0f, increment = ((float)TWOPI / _numberOfSides);

	// If Its A Sqaure, Turn It 45% Degrees
	if (_numberOfSides == 4)
	{
		GenerateGenericQuadVertices();
		return;
	}

	// Centre
	m_Vertices.emplace_back(Vertex{{0.0f,  0.0f, 0.0f}, {0.5f,0.5f}});

	// Fan Around Centre
	float xPos, yPos;
	for (int i = 0; i < _numberOfSides; i++)
	{
		xPos = cos(angle);
		yPos = sin(angle);
		m_Vertices.emplace_back(Vertex{{xPos, yPos, 0 },{ToTexCoord(xPos),ToTexCoord(yPos)}});
		angle += increment;
	}
}

void Mesh::GenerateShapeIndices(SHAPE _shape)
{
	switch (_shape)
	{
	case SHAPE::CUBE:
	{
		m_Indices.emplace_back(0);
		m_Indices.emplace_back(1);
		m_Indices.emplace_back(2);
		m_Indices.emplace_back(0);
		m_Indices.emplace_back(2);
		m_Indices.emplace_back(3);

		m_Indices.emplace_back(4);
		m_Indices.emplace_back(5);
		m_Indices.emplace_back(6);
		m_Indices.emplace_back(4);
		m_Indices.emplace_back(6);
		m_Indices.emplace_back(7);

		m_Indices.emplace_back(8);
		m_Indices.emplace_back(9);
		m_Indices.emplace_back(10);
		m_Indices.emplace_back(8);
		m_Indices.emplace_back(10);
		m_Indices.emplace_back(11);

		m_Indices.emplace_back(12);
		m_Indices.emplace_back(13);
		m_Indices.emplace_back(14);
		m_Indices.emplace_back(12);
		m_Indices.emplace_back(14);
		m_Indices.emplace_back(15);

		m_Indices.emplace_back(16);
		m_Indices.emplace_back(17);
		m_Indices.emplace_back(18);
		m_Indices.emplace_back(16);
		m_Indices.emplace_back(18);
		m_Indices.emplace_back(19);

		m_Indices.emplace_back(20);
		m_Indices.emplace_back(21);
		m_Indices.emplace_back(22);
		m_Indices.emplace_back(20);
		m_Indices.emplace_back(22);
		m_Indices.emplace_back(23);

		break;
	}
	default:
	{
		break;
	}
	}
}

void Mesh::GenerateShapeVertices(SHAPE _shape)
{
	switch (_shape)
	{
	case SHAPE::CUBE:
	{
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, 0.5f}, {0.0f,1.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, 0.5f}, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, 0.5f}, {1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, 0.5f}, {1.0f,1.0f} });

		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, -0.5f}, {0.0f,1.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, -0.5f}, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, -0.5f}, {1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, -0.5f}, {1.0f,1.0f} });

		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, 0.5f}, {0.0f,1.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, 0.5f}, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, -0.5f}, {1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, -0.5f}, {1.0f,1.0f} });

		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, -0.5f}, {0.0f,1.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, -0.5f}, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, 0.5f}, {1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, 0.5f}, {1.0f,1.0f} });

		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, -0.5f}, {0.0f,1.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, 0.5f}, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, 0.5f}, {1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, -0.5f}, {1.0f,1.0f} });

		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, 0.5f}, {0.0f,1.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, -0.5f}, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, -0.5f}, {1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, 0.5f}, {1.0f,1.0f} });
		break;
	}
	default:
	{
		break;
	}
	}
	
}

void Mesh::GenerateGenericQuadVertices()
{
	m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, 0.0f}, {0.0f,1.0f} }); // Top Left
	m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, 0.0f}, {0.0f,0.0f} }); // Bottom Left
	m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, 0.0f}, {1.0f,0.0f} }); // Bottom Right
	m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, 0.0f}, {1.0f,1.0f} }); // Top Right
}

void Mesh::GenerateGenericQuadIndices()
{
	m_Indices.emplace_back(0);	// Top Left
	m_Indices.emplace_back(1);	// Bottom Left
	m_Indices.emplace_back(2);	// Bottom Right
	m_Indices.emplace_back(0);	// Top Left
	m_Indices.emplace_back(2);	// Bottom Right
	m_Indices.emplace_back(3);	// Top Right
	m_Indices.emplace_back(0);	// Top Left
}

float Mesh::ToTexCoord(float& _position)
{
	return (_position + 1) * 0.5f;
}

void Mesh::GeneratePolygonIndices(int&& _numberOfSides)
{
	if (_numberOfSides == 4)
	{
		GenerateGenericQuadIndices();
		return;
	}
	for (int i = 0; i < _numberOfSides; i++)
	{
		// Centre
		m_Indices.push_back(0);

		// Back At Beginning?
		if (i + 2 > _numberOfSides)
		{
			m_Indices.push_back(i + 1);
			m_Indices.push_back(1);
		}
		else
		{
			m_Indices.push_back(i + 1);
			m_Indices.push_back(i + 2);
		}
	}
}
