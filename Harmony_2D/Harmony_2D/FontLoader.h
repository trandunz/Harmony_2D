#pragma once
#include "Helper.h"
#include <ft2build.h>
#include FT_FREETYPE_H

using Font = std::map<GLchar, FontChar>;

class FontLoader
{
public:
	/// <summary>
	/// Loads the given font and returns it.
	/// </summary>
	/// <param name="_filePath"></param>
	/// <param name="_characterLimit"></param>
	/// <returns></returns>
	static Font LoadFont(std::string&& _fileName, unsigned _characterLimit = 128);
private:
	/// <summary>
	/// Creates A Texture And Returns Its ID, FilePath And Dimentions In A The Struct Texture Using Cache Optimization.
	/// </summary>
	/// <param name="_filePath"></param>
	/// <returns></returns>
	static GLuint LoadFontTexture(FT_Face&& _fontFace);
};

