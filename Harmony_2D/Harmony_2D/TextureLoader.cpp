#include "TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <STBI/stb_image.h>

TextureLoader::~TextureLoader()
{
    for (auto& item : m_Textures)
    {
        glDeleteTextures(1, &item.ID);
    }
}

void TextureLoader::Init()
{
    stbi_set_flip_vertically_on_load(true);
}

Texture TextureLoader::LoadTexture(const char* _filePath)
{
    for (auto& item : m_Textures)
    {
        if (item.FilePath == _filePath)
        {
            return item;
        }
    }

    GLint width, height, components;
    GLubyte* imageData = stbi_load(_filePath, &width, &height, &components, 0);
    
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    components = components == 4 ? GL_RGBA : components == 3 ? GL_RGB : components == 2 ? GL_RG : GL_R;
    
    glTexImage2D(GL_TEXTURE_2D, 0 ,components, width, height, 0, components, GL_UNSIGNED_BYTE, imageData);
    
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(imageData);
    imageData = nullptr;
    glBindTexture(GL_TEXTURE_2D, 0);

    m_Textures.emplace_back(Texture{ id , {width,height},_filePath });

    return m_Textures.back();
}
