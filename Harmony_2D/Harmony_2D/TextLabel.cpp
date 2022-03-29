#include "TextLabel.h"

TextLabel::TextLabel(glm::ivec2* _windowSize, std::string_view&& _text, glm::vec2&& _position = { 0.0f,0.0f }, glm::vec4&& _colour = { 1.0f, 1.0f, 1.0f ,1.0f }, glm::vec2&& _scale = { 1.0f,1.0f })
{
	SetText(std::move(_text));
	SetPosition(std::move(_position));
	SetColour(std::move(_colour));
	SetScale(std::move(_scale));
	m_WindowSize = _windowSize;

	m_ProjectionMatrix = glm::ortho(0.0f, (float)m_WindowSize->x, 0.0f, (float)m_WindowSize->y, 0.0f, 100.0f);
	m_ProgramID = ShaderLoader::CreateShader("Resources/Shaders/TextLabel.vert", "Resources/Shaders/TextLabel.frag");

	FT_Library fontLibrary;
	FT_Face fontFace;

	if (FT_Init_FreeType(&fontLibrary) != 0)
	{
		Print("Failed To Inialize FreeType Library");
		return;
	}
	if (FT_New_Face(fontLibrary, _text.data(), 0, &fontFace) != 0)
	{
		std::string message = "Failed To Load Font: ";
		message += _text;
		Print(message);
		return;
	}

	FT_Set_Pixel_Sizes(fontFace, 0, 10.0f);
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
			1, // Texture
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4), nullptr, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

}

TextLabel::~TextLabel()
{
	m_WindowSize = nullptr;
}

void TextLabel::Draw()
{
}

void TextLabel::SetText(std::string_view&& _newText)
{
	m_Text = _newText;
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

GLuint TextLabel::GenerateTexture(FT_Face _face)
{
	return GLuint();
}
