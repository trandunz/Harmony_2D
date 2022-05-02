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
	// Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normals)));
	// TexCoords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoords)));
	

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
	// Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normals)));
	// TexCoords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoords)));
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

void NewMesh::Draw()
{
	glBindVertexArray(m_VertexArrayID);

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
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, 0.5f}, {0,0,1.0f}, {0.0f,1.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, 0.5f},{0,0,1.0f}, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, 0.5f}, {0,0,1.0f},{1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, 0.5f},  {0,0,1.0f}, {1.0f,1.0f} });

		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, -0.5f},  {0,0,-1.0f}, {0.0f,1.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, -0.5f}, {0,0,-1.0f},{0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, -0.5f},{0,0,-1.0f},{1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, -0.5f}, {0,0,-1.0f}, {1.0f,1.0f} });

		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, 0.5f}, {1.0f,0,0},{0.0f,1.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, 0.5f}, {1.0f,0,0},{0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, -0.5f},{1.0f,0,0}, {1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, -0.5f}, {1.0f,0,0},{1.0f,1.0f} });

		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, -0.5f},{-1.0f,0,0}, {0.0f,1.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, -0.5f},{-1.0f,0,0}, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, 0.5f},{-1.0f,0,0}, {1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, 0.5f}, {-1.0f,0,0},{1.0f,1.0f} });

		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, -0.5f}, {0,1.0f,0}, {0.0f,1.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, 0.5f}, {0,1.0f,0},{0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, 0.5f}, {0,1.0f,0},{1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, -0.5f}, {0,1.0f,0},{1.0f,1.0f} });

		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, 0.5f},{0,-1.0f,0}, {0.0f,1.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, -0.5f}, {0,-1.0f,0},{0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, -0.5f}, {0,-1.0f,0},{1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, 0.5f}, {0,-1.0f,0},{1.0f,1.0f} });
		break;
	}
	case SHAPE::PYRAMID:
	{
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, -0.5f},{}, {0.0f,1.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, 0.5f},{}, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, 0.5f}, {},{1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, -0.5f},{}, {1.0f,1.0f} });

		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, -0.5f},{},{0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, 0.5f}, {},{1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, 0.5f},{}, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, 0.5f}, {}, {1.0f,0.0f} });

		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, 0.5f}, {}, {0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, -0.5f},{}, {1.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.0f, -0.5f}, {},{0.0f,0.0f} });
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.0f, -0.5f},{},{1.0f,0.0f} });

		m_Vertices.emplace_back(Vertex{ {0.0f,  1.0f, 0.0f},{}, {0.5f,1.0f} });
		break;
	}
	case SHAPE::SPHERE:
	{
		// Angles to keep track of the sphere points 
		float Phi = 0.0f;
		float Theta = 0.0f;

		// Each cycle moves down on the vertical (Y axis) to start the next ring
		for (int i = 0; i < 36; i++)
		{
			// A new  horizontal ring starts at 0 degrees
			Theta = 0.0f;

			// Creates a horizontal ring and adds each new vertex point to the vertex array
			for (int j = 0; j < 36; j++)
			{
				// Calculate the new vertex position point with the new angles
				float x = cos(Phi) * sin(Theta);
				float y = cos(Theta);
				float z = sin(Phi) * sin(Theta);

				// Set the position of the current vertex point
				m_Vertices.emplace_back(Vertex{ 
					{ x * 0.5f, y * 0.5f, z * 0.5f },
					{ x,y,z },
					{ (float)i / (36 - 1), 1 - ((float)j / (36 - 1)) }
					});

				// Theta (Y axis) angle is incremented based on the angle created by number of sections
				// As the sphere is built ring by ring, the theta is only needed to do half the circumferance therefore using just PI
				Theta += ((float)PI / ((float)36 - 1.0f));
			}

			// Phi angle (X and Z axes) is incremented based on the angle created by the number of sections
			// Angle uses 2*PI to get the full circumference as this layer is built as a full ring
			Phi += (2.0f * (float)PI) / ((float)36 - 1.0f);
		}
		
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
	case SHAPE::SPHERE:
	{
		for (int i = 0; i < 36; i++)
		{
			for (int j = 0; j < 36; j++)
			{
				// First triangle of the quad
				m_Indices.emplace_back((((i + 1) % 36) * 36) + ((j + 1) % 36));
				m_Indices.emplace_back((i * 36) + (j));
				m_Indices.emplace_back((((i + 1) % 36) * 36) + (j));

				// Second triangle of the quad
				m_Indices.emplace_back((i * 36) + ((j + 1) % 36));
				m_Indices.emplace_back((i * 36) + (j));
				m_Indices.emplace_back((((i + 1) % 36) * 36) + ((j + 1) % 36));
			}
		}
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
		m_Vertices.emplace_back(Vertex{ {-0.5f,  0.5f, 0.0f},{}, {0.0f,1.0f} }); // Top Left
		m_Vertices.emplace_back(Vertex{ {-0.5f,  -0.5f, 0.0f},{}, {0.0f,0.0f} }); // Bottom Left
		m_Vertices.emplace_back(Vertex{ {0.5f,  -0.5f, 0.0f}, {},{1.0f,0.0f} }); // Bottom Right
		m_Vertices.emplace_back(Vertex{ {0.5f,  0.5f, 0.0f}, {},{1.0f,1.0f} }); // Top Right
		return;
	}

	float angle = 0.0f, increment = ((float)TWOPI / _numberOfSides);

	// Centre
	m_Vertices.emplace_back(Vertex{ {0.0f,  0.0f, 0.0f},{}, {0.5f,0.5f} });

	// Fan Around Centre
	float xPos, yPos;
	for (unsigned i = 0; i < _numberOfSides; i++)
	{
		xPos = cos(angle);
		yPos = sin(angle);
		m_Vertices.emplace_back(Vertex{ {xPos, yPos, 0 },{},{ToTexCoord(xPos),ToTexCoord(yPos)} });
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
	for (unsigned i = 0; i < _numberOfSides; i++)
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
