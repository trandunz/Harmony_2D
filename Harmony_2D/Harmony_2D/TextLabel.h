#pragma once
#include "Helper.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"
#include <FREETYPE/ft2build.h>
#include FT_FREETYPE_H
class TextLabel
{
public:
	TextLabel(glm::ivec2* _windowSize, std::string_view&& _text, glm::vec2&& _position = { 0.0f,0.0f }, glm::vec4&& _colour = { 1.0f, 1.0f, 1.0f ,1.0f }, glm::vec2&& _scale = {1.0f,1.0f});
	~TextLabel();

	void Draw();
	
	void SetText(std::string_view&& _newText);
	void SetColour(glm::vec4&& _newColour);
	void SetScale(glm::vec2&& _newScale);
	void SetPosition(glm::vec2&& _newPosition);

private:
	struct FontChar
	{
		GLuint m_TextureID = 0; // Texture ID
		glm::ivec2 m_Size{ 0,0 }; // Size of 'glyph'
		glm::ivec2 m_Bearing{ 0,0 }; // Offset of 'glyph' from baseline
		GLuint m_Advance = 0; // Distance To Next Character
	};

	GLuint GenerateTexture(FT_Face _face);
	const int m_CharacterLimit = 128;
	bool m_Initialized = false;

	std::string m_Text{ "" };
	glm::vec2 m_Scale{ 1.0f, 1.0f };
	glm::vec2 m_Position{ 0.0f, 0.0f };
	glm::vec4 m_Colour{ 1.0f,1.0f,1.0f,1.0f };

	GLuint m_VertexArrayID;
	GLuint m_VertexBufferID;
	GLuint m_ProgramID;
	glm::mat4 m_ProjectionMatrix;
	glm::ivec2* m_WindowSize = nullptr;
	std::map<GLchar, FontChar> m_CharacterMap;
};

