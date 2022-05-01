#include "NewMesh.h"

NewMesh::NewMesh(SHAPE _shape)
{
	CreateShapeVertices(_shape);
	CreateShapeIndices(_shape);

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

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

NewMesh::NewMesh(unsigned int _numberOfSides)
{
	CreatePolygonVertices(_numberOfSides);
	CreatePolygonIndices(_numberOfSides);

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

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

NewMesh::~NewMesh()
{
	m_Indices.clear();
	m_Vertices.clear();

	// Unbind
	{
		glUseProgram(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// Delete
	{
		glDeleteVertexArrays(1, &m_VertexArrayID);
		glDeleteBuffers(1, &m_VertexBufferID);
		glDeleteBuffers(1, &m_IndexBufferID);
	}
}

void NewMesh::Draw(Transform& _transform, GLuint _shaderID, glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix, std::vector<Texture> _textures)
{
	// Bind
	glUseProgram(_shaderID);
	glBindVertexArray(m_VertexArrayID);

	// Textures
	ShaderLoader::SetUniform1i(std::move(_shaderID), "TextureCount", (GLint)_textures.size());
	for (unsigned i = 0; i < _textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, _textures[i].ID);
		ShaderLoader::SetUniform1i(std::move(_shaderID), "Texture" + std::to_string(i), std::move(i));
	}

	// Model Matrix
	ShaderLoader::SetUniformMatrix4fv(std::move(_shaderID), "Model", std::move(_transform.transform));

	// Projection * View Matrix
	ShaderLoader::SetUniformMatrix4fv(std::move(_shaderID), "PVMatrix", _projectionMatrix * _viewMatrix);

	// Elapsed Time
	ShaderLoader::SetUniform1f(std::move(_shaderID), "Time", (float)glfwGetTime());

	glDrawElements(GL_TRIANGLES, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
	glUseProgram(0);
}

void NewMesh::CreateShapeVertices(SHAPE _shape)
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
	case SHAPE::PYRAMID:
	{
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, -0.5f}, {0.0f,1.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, 0.5f}, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, 0.5f}, {1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, -0.5f}, {1.0f,1.0f} });

		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, -0.5f},{0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, 0.5f}, {1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, 0.5f}, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, 0.5f},  {1.0f,0.0f} });

		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, 0.5f},  {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, -0.5f}, {1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, -0.5f}, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, -0.5f},{1.0f,0.0f} });

		m_Vertices.emplace_back(Vertex{ {0.0f,  1.0f, 0.0f}, {0.5f,1.0f} });
		break;
	}
	default:
	{
		break;
	}
	}
}

void NewMesh::CreateShapeIndices(SHAPE _shape)
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
	case SHAPE::PYRAMID:
	{
		m_Indices.emplace_back(1);
		m_Indices.emplace_back(0);
		m_Indices.emplace_back(3);
		m_Indices.emplace_back(1);
		m_Indices.emplace_back(3);
		m_Indices.emplace_back(2);

		m_Indices.emplace_back(4);
		m_Indices.emplace_back(5);
		m_Indices.emplace_back(12);
		m_Indices.emplace_back(6);
		m_Indices.emplace_back(7);
		m_Indices.emplace_back(12);
		m_Indices.emplace_back(8);
		m_Indices.emplace_back(9);
		m_Indices.emplace_back(12);
		m_Indices.emplace_back(10);
		m_Indices.emplace_back(11);
		m_Indices.emplace_back(12);
		break;
	}
	default:
	{
		break;
	}
	}
}

void NewMesh::CreatePolygonVertices(unsigned int _numberOfSides)
{
	// If Its Got 4 Sides, Create Generic Quad
	if (_numberOfSides == 4)
	{
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, 0.0f}, {0.0f,1.0f} }); // Top Left
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, 0.0f}, {0.0f,0.0f} }); // Bottom Left
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, 0.0f}, {1.0f,0.0f} }); // Bottom Right
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, 0.0f}, {1.0f,1.0f} }); // Top Right
		return;
	}

	float angle = 0.0f, increment = ((float)TWOPI / _numberOfSides);

	// Centre
	m_Vertices.emplace_back(Vertex{ {0.0f,  0.0f, 0.0f}, {0.5f,0.5f} });

	// Fan Around Centre
	float xPos, yPos;
	for (int i = 0; i < _numberOfSides; i++)
	{
		xPos = cos(angle);
		yPos = sin(angle);
		m_Vertices.emplace_back(Vertex{ {xPos, yPos, 0 },{ToTexCoord(xPos),ToTexCoord(yPos)} });
		angle += increment;
	}
}

void NewMesh::CreatePolygonIndices(unsigned int _numberOfSides)
{
	// If Its Got 4 Sides, Create Generic Quad Indices
	if (_numberOfSides == 4)
	{
		m_Indices.emplace_back(0);	// Top Left
		m_Indices.emplace_back(1);	// Bottom Left
		m_Indices.emplace_back(2);	// Bottom Right
		m_Indices.emplace_back(0);	// Top Left
		m_Indices.emplace_back(2);	// Bottom Right
		m_Indices.emplace_back(3);	// Top Right
		m_Indices.emplace_back(0);	// Top Left
		return;
	}

	// Generate Generalised Regular Polygon Indices
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

float NewMesh::ToTexCoord(float& _position)
{
	return (_position + 1) * 0.5f;
}
