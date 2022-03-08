#pragma once
#include "Helper.h"
static class TextureLoader
{
public:
	~TextureLoader();
	static void Init();
	static Texture LoadTexture(const char* _filePath);

	inline static std::vector<Texture> m_Textures;
};

