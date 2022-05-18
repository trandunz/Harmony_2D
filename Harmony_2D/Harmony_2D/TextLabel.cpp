#include "TextLabel.h"

TextLabel::TextLabel(glm::ivec2* _windowSize, std::string_view&& _text, std::map<GLchar, FontChar> _loadedFont, double& _deltaTime, glm::vec2&& _position, glm::vec4&& _colour , glm::vec2&& _scale)
{
	SetText(_text);
	SetPosition(std::move(_position));
	SetColour(std::move(_colour));
	SetScale(std::move(_scale));
	m_WindowSize = _windowSize;
	m_DeltaTime = &_deltaTime;
	m_CharacterMap = _loadedFont;
	if (_loadedFont.size() > 0)
		m_FontLoaded = true;

	m_ProjectionMatrix = glm::ortho(0.0f, (float)m_WindowSize->x, 0.0f, (float)m_WindowSize->y, 0.0f, 10.0f);
	m_ProgramID = ShaderLoader::CreateShader("TextLabel.vert", "TextLabel.frag");

	glGenVertexArrays(1, &m_VertexArrayID);
	glBindVertexArray(m_VertexArrayID);

	glGenBuffers(1, &m_VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 4, nullptr, GL_DYNAMIC_DRAW);

	GLuint indices[6]
	{
		0,1,2,
		0,2,3
	};

	glGenBuffers(1, &m_IndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

TextLabel::~TextLabel()
{
	m_WindowSize = nullptr;
	m_DeltaTime = nullptr;
}

void TextLabel::Draw()
{
	if (m_FontLoaded)
	{
		glUseProgram(m_ProgramID);
		ShaderLoader::SetUniform4fv(std::move(m_ProgramID), "Colour", std::move(m_Colour));
		ShaderLoader::SetUniformMatrix4fv(std::move(m_ProgramID), "PMatrix", std::move(m_ProjectionMatrix));
		ShaderLoader::SetUniform1f(std::move(m_ProgramID), "LeftClip", std::move(m_LeftClip));
		ShaderLoader::SetUniform1f(std::move(m_ProgramID), "RightClip", std::move(m_RightClip));
		ShaderLoader::SetUniform1f(std::move(m_ProgramID), "ElapsedTime", (float)glfwGetTime());
		ShaderLoader::SetUniform1f(std::move(m_ProgramID), "ScrollSpeed", std::move(m_ScrollSpeed));
		ShaderLoader::SetUniform1i(std::move(m_ProgramID), "IsScrollingRight", std::move(m_ScrollRight));
		ShaderLoader::SetUniform1i(std::move(m_ProgramID), "IsScrolling", std::move(m_IsScrolling));

		glBindVertexArray(m_VertexArrayID);

		FontChar fontCharacter;
		GLfloat posX;
		GLfloat posY;
		GLfloat width;
		GLfloat height;
		glm::vec2 origin = m_Position;
		for (auto& character : m_Label)
		{
			fontCharacter = m_CharacterMap[character];
			width = fontCharacter.size.x * m_Scale.x;
			height = fontCharacter.size.y * m_Scale.y;
			ApplyAlignment(posX, posY, origin, fontCharacter);

			GLfloat vertices[4][4]
			{
				{posX, posY + height, 0.0f, 0.0f}, // 0
				{posX, posY, 0.0f, 1.0f}, // 1
				{posX + width, posY , 1.0f, 1.0f}, // 2
				{posX + width, posY + height, 1.0f, 0.0f} // 3
			};

			glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, fontCharacter.textureID);
			ShaderLoader::SetUniform1i(std::move(m_ProgramID), "Texture", 0);

			glDrawElements(GL_TRIANGLES, sizeof(GLuint) * 6, GL_UNSIGNED_INT, nullptr);

			origin.x += fontCharacter.advance * m_Scale.x;
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

glm::vec2 TextLabel::GetScale()
{
	return m_Scale;
}

glm::vec2 TextLabel::GetPosition()
{
	return m_Position;
}

float TextLabel::GetRightClip()
{
	return m_RightClip;
}

glm::vec4 TextLabel::GetBounds()
{
	glm::vec2 textSize = GetTextSize();
	switch (m_Alignment)
	{
	case TEXTALIGNMENT::LEFT:
	{
		return { m_Position.x, m_Position.x + textSize.x , m_WindowSize->y - m_Position.y,  m_WindowSize->y - (m_Position.y + textSize.y) };
		break;
	}
	case TEXTALIGNMENT::MIDDLE:
	{
		return { m_Position.x - textSize.x / 2.0, m_Position.x + textSize.x / 2.0f , m_WindowSize->y - (m_Position.y - textSize.y / 2),  m_WindowSize->y - (m_Position.y + textSize.y / 2) };
		break;
	}
	case TEXTALIGNMENT::RIGHT:
	{
		return { m_Position.x - textSize.x, m_Position.x , m_WindowSize->y - m_Position.y,  m_WindowSize->y - (m_Position.y + textSize.y) };
		break;
	}
	default:
		return glm::vec4(0);
		break;
	}
}

glm::vec2 TextLabel::GetTextSize()
{
	float textSizeX = 0.0f;
	float largestSizeY = 0.0f;

	for (auto& character : m_Label)
	{
		FontChar fontCharacter = m_CharacterMap[character];
		textSizeX += fontCharacter.advance * m_Scale.x;
		float sizeY = fontCharacter.size.y * m_Scale.y;
		if (sizeY > largestSizeY && sizeY > 0)
		{
			largestSizeY = sizeY;
		}
	}

	return { textSizeX , largestSizeY};
}

bool TextLabel::BoundsContain(glm::vec2 _position)
{
	if (_position.x > GetBounds().x
		&& _position.x < GetBounds().y
		&& _position.y < GetBounds().z
		&& _position.y > GetBounds().w)
	{
		return true;
	}
	else
		return false;
}

void TextLabel::SetText(std::string_view _newText)
{
	m_Label = _newText;
}

void TextLabel::SetColour(glm::vec4&& _newColour)
{
	m_Colour = _newColour;
}

void TextLabel::SetScale(glm::vec2&& _newScale)
{
	m_Scale = _newScale;
}

void TextLabel::SetPosition(glm::vec2&& _newPosition)
{
	m_Position = _newPosition;
}

void TextLabel::SetScrollSpeed(float&& _newSpeed)
{
	m_ScrollSpeed = 100.0f * _newSpeed;
}

void TextLabel::SetScrollingRight(bool&& _isScrollingRight)
{
	m_ScrollRight = _isScrollingRight;
}

void TextLabel::SetScrolling(bool&& _isScrolling)
{
	m_IsScrolling = _isScrolling;
}

void TextLabel::SetClip(float&& _leftClip, float&& _rightClip)
{
	m_LeftClip = _leftClip;
	m_RightClip = _rightClip;
}

void TextLabel::SetAlignment(TEXTALIGNMENT _newAlignment)
{
	m_Alignment = _newAlignment;
}

TEXTALIGNMENT TextLabel::GetAlignment()
{
	return m_Alignment;
}

void TextLabel::ApplyAlignment(GLfloat& _xPos, GLfloat& _yPos, glm::vec2& _origin, FontChar& _fontCharacter)
{
	glm::vec2 textSize = GetTextSize();
	switch (m_Alignment)
	{
	case TEXTALIGNMENT::LEFT:
	{
		_xPos = _origin.x + _fontCharacter.bearing.x * m_Scale.x;
		_yPos = _origin.y - (_fontCharacter.size.y - _fontCharacter.bearing.y) * m_Scale.y;
		break;
	}
	case TEXTALIGNMENT::MIDDLE:
	{
		_xPos = (_origin.x - (textSize.x / 2)) + _fontCharacter.bearing.x * m_Scale.x;
		_yPos = (_origin.y - (textSize.y / 2)) - (_fontCharacter.size.y - _fontCharacter.bearing.y) * m_Scale.y;
		break;
	}
	case TEXTALIGNMENT::RIGHT:
	{
		_xPos = (_origin.x - textSize.x) + _fontCharacter.bearing.x * m_Scale.x;
		_yPos = _origin.y - (_fontCharacter.size.y - _fontCharacter.bearing.y) * m_Scale.y;
		break;
	}
	default:
		_xPos = _origin.x + _fontCharacter.bearing.x * m_Scale.x;
		_yPos = _origin.y - (_fontCharacter.size.y - _fontCharacter.bearing.y) * m_Scale.y;
		break;
	}
}


