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
	/// Initializes The Texture Loader With Any Custom Setups (e.g Flip_on_Load)
	/// </summary>
	static void Init();

	/// <summary>
	/// Creates A Texture And Returns Its ID, FilePath And Dimentions In A The Struct Texture Using Cache Optimization.
	/// </summary>
	/// <param name="_filePath"></param>
	/// <returns></returns>
	static Texture LoadTexture(const char* _filePath);

private:
	inline static std::vector<Texture> m_Textures;
};

