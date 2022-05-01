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
	m_ProgramID = ShaderLoader::CreateShader("Resources/Shaders/TextLabel.vert", "Resources/Shaders/TextLabel.frag");

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

void TextLabel::Update()
{
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

		origin.x += m_OriginOffset;

		float textSize = 0.0f;
		for (auto& character : m_Text)
		{
			FontChar fontCharacter = m_CharacterMap[character];
			textSize += fontCharacter.advance * m_Scale.x;
		}

		for (auto& character : m_Text)
		{
			fontCharacter = m_CharacterMap[character];
			width = fontCharacter.size.x * m_Scale.x;
			height = fontCharacter.size.y * m_Scale.y;
			posX = (origin.x - (textSize / 2)) + fontCharacter.bearing.x * m_Scale.x;
			posY = origin.y - (fontCharacter.size.y - fontCharacter.bearing.y) * m_Scale.y;

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

		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
		glBindVertexArray(0);
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
	float textSizeX = 0.0f;
	float largestSizeY = 0.0f;
	
	for (auto& character : m_Text)
	{
		FontChar fontCharacter = m_CharacterMap[character];
		textSizeX += fontCharacter.advance * m_Scale.x;
		float sizeY = fontCharacter.size.y * m_Scale.y;
		if (sizeY > largestSizeY && sizeY > 0)
		{
			largestSizeY = sizeY;
		}
	}

	return {m_Position.x - textSizeX /2.0, m_Position.x + textSizeX / 2.0f , m_WindowSize->y - m_Position.y,  m_WindowSize->y - (m_Position.y + largestSizeY) };
}

float TextLabel::GetAverageCharacterAdvance()
{
	if (m_Text.size() <= 0)
	{
		return 0;
	}
	float averageLength = 0.0f;
	for (auto& character : m_Text)
	{
		FontChar fontCharacter = m_CharacterMap[character];
		averageLength += fontCharacter.advance;
	}
	averageLength /= m_Text.size();
	return averageLength;
}

void TextLabel::SetText(std::string_view _newText)
{
	m_Text = _newText;
}

void TextLabel::SetOriginOffset(float&& _offset)
{
	m_OriginOffset = _offset;
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


