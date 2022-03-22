#pragma once
#include "Helper.h"
class TextureLoader
{
public:
	/// <summary>
	/// Cleans Up All Loaded Textures
	/// </summary>
	~TextureLoader();

	/// <summary>
	/// Initializes The Texture Loader With Any Initial Textures And Sets Flip_On_Load True
	/// </summary>
	static void Init(std::vector<const char*>&& _textures = {});

	/// <summary>
	/// Creates A Texture And Returns Its ID, FilePath And Dimentions In A The Struct Texture Using Cache Optimization.
	/// </summary>
	/// <param name="_filePath"></param>
	/// <returns></returns>
	static Texture LoadTexture(const char* _filePath);

private:
	inline static std::vector<Texture> m_Textures;
};

