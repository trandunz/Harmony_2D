#include "FontLoader.h"

Font FontLoader::LoadFont(std::string&& _fileName, unsigned _characterLimit)
{
	FT_Library fontLibrary;
	FT_Face fontFace;
	std::map<GLchar, FontChar> characterMap{};
	_fileName = "Resources/Fonts/" + _fileName;
	if (FT_Init_FreeType(&fontLibrary) != 0)
	{
		Print("Failed To Inialize FreeType Library");
		return characterMap;
	}
	if (FT_New_Face(fontLibrary, _fileName.data(), 0, &fontFace) != 0)
	{
		Print("Failed To Load Font");
		return characterMap;
	}

	FT_Set_Pixel_Sizes(fontFace, 0, 100);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte glyph = 0; glyph < _characterLimit; glyph++)
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
		characterMap.emplace(std::make_pair(glyph, fontCharacter));
	}

	FT_Done_Face(fontFace);
	FT_Done_FreeType(fontLibrary);

	return characterMap;
}

GLuint FontLoader::LoadFontTexture(FT_Face&& _fontFace)
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
