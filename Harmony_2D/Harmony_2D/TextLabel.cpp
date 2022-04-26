#include "TextLabel.h"

TextLabel::TextLabel(glm::ivec2* _windowSize, std::string_view&& _text, std::string_view&& _font, double& _deltaTime, glm::vec2&& _position, glm::vec4&& _colour , glm::vec2&& _scale)
{
	SetText(_text);
	SetPosition(std::move(_position));
	SetColour(std::move(_colour));
	SetScale(std::move(_scale));
	m_WindowSize = _windowSize;
	m_DeltaTime = &_deltaTime;

	m_ProjectionMatrix = glm::ortho(0.0f, (float)m_WindowSize->x, 0.0f, (float)m_WindowSize->y, 0.0f, 100.0f);
	m_ProgramID = ShaderLoader::CreateShader("Resources/Shaders/TextLabel.vert", "Resources/Shaders/TextLabel.frag");

	FT_Library fontLibrary;
	FT_Face fontFace;

	if (FT_Init_FreeType(&fontLibrary) != 0)
	{
		Print("Failed To Inialize FreeType Library");
		return;
	}
	if (FT_New_Face(fontLibrary, _font.data(), 0, &fontFace) != 0)
	{
		std::string message = "Failed To Load Font: ";
		message += _text;
		Print(message);
		return;
	}

	FT_Set_Pixel_Sizes(fontFace, 0, 100);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte glyph = 0; glyph < m_CharacterLimit; glyph++)
	{
		if (FT_Load_Char(fontFace, glyph, FT_LOAD_RENDER))
		{
			std::string message = "Failed To Load Freetype Glyph: ";
			message += (unsigned char)glyph;
			Print(message);
			continue;
		}

		FontChar fontCharacter =
		{
			LoadFontTexture(std::move(fontFace)), // Texture
			{fontFace->glyph->bitmap.width,fontFace->glyph->bitmap.rows }, // Set Size
			{fontFace->glyph->bitmap_left,fontFace->glyph->bitmap_top }, // Set Bearing
			(GLuint)fontFace->glyph->advance.x / 64 // Set Advance
		};
		m_CharacterMap.emplace(std::make_pair(glyph, fontCharacter));
	}

	FT_Done_Face(fontFace);
	FT_Done_FreeType(fontLibrary);

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
	

	m_Initialized = true;
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
	if (m_Initialized)
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
		glm::vec2 origin = m_Position;
		origin.x += m_OriginOffset;
		if (!m_IsScrolling)
		{
			origin.x += 50.0f;
		}
		
		float textSize = 0.0f;
		for (auto& glyph : m_Text)
		{
			FontChar fontCharacter = m_CharacterMap[glyph];
			textSize += (fontCharacter.m_Advance);
		}
		for (auto& character : m_Text)
		{
			FontChar fontCharacter = m_CharacterMap[character];
			GLfloat posX = origin.x + fontCharacter.m_Bearing.x * m_Scale.x;
			GLfloat posY = origin.y - (fontCharacter.m_Size.y - fontCharacter.m_Bearing.y) * m_Scale.y;
			GLfloat width = fontCharacter.m_Size.x * m_Scale.x;
			GLfloat height = fontCharacter.m_Size.y * m_Scale.y;

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
			glBindTexture(GL_TEXTURE_2D, fontCharacter.m_TextureID);
			ShaderLoader::SetUniform1i(std::move(m_ProgramID), "Texture", 0);

			glDrawElements(GL_TRIANGLES, sizeof(GLuint) * 6, GL_UNSIGNED_INT, nullptr);

			origin.x += (fontCharacter.m_Advance  * m_Scale.x) ;
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
		averageLength += fontCharacter.m_Advance;
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

GLuint TextLabel::LoadFontTexture(FT_Face&& _fontFace)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RED,
		_fontFace->glyph->bitmap.width,
		_fontFace->glyph->bitmap.rows,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		_fontFace->glyph->bitmap.buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return textureID;
}
