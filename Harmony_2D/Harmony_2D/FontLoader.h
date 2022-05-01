#pragma once
#include "Helper.h"
#include <ft2build.h>
#include FT_FREETYPE_H

class FontLoader
{
public:
	static std::map<GLchar, FontChar> LoadFont(std::string_view _filePath, unsigned _characterLimit = 128);
private:
	/// <summary>
	/// Creates A Texture And Returns Its ID, FilePath And Dimentions In A The Struct Texture Using Cache Optimization.
	/// </summary>
	/// <param name="_filePath"></param>
	/// <returns></returns>
	static GLuint LoadFontTexture(FT_Face&& _fontFace);
};

