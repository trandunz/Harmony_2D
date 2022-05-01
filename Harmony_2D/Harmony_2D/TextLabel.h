#pragma once
#include "Helper.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"



class TextLabel
{
public:
	TextLabel(glm::ivec2* _windowSize, std::string_view&& _text, std::map<GLchar, FontChar> _loadedFont, double& _deltaTime, glm::vec2&& _position = { 0.0f,0.0f }, glm::vec4&& _colour = { 0.0f, 0.0f, 0.0f ,1.0f }, glm::vec2&& _scale = {1.0f,1.0f});
	~TextLabel();

	void Update();

	void Draw();

	glm::vec2 GetScale();
	glm::vec2 GetPosition();
	float GetRightClip();
	glm::vec4 GetBounds();

	float GetAverageCharacterAdvance();
	
	void SetText(std::string_view _newText);
	void SetOriginOffset(float&& _offset);
	void SetColour(glm::vec4&& _newColour);
	void SetScale(glm::vec2&& _newScale);
	void SetPosition(glm::vec2&& _newPosition);
	void SetScrollSpeed(float&& _newSpeed);
	void SetScrollingRight(bool&& _isScrollingRight);
	void SetScrolling(bool&& _isScrolling);
	void SetClip(float&& _leftClip, float&& _rightClip);

private:
	bool m_FontLoaded = false;
	bool m_IsScrolling = false;
	bool m_ScrollRight = true;
	double* m_DeltaTime = nullptr;
	float m_ScrollSpeed = 100.0f;
	float m_LargestGlyphWidth = 0.0f;
	float m_RightClip = 800.0f;
	float m_LeftClip = 200.0f;
	float m_OriginOffset = 0.0f;

	std::string m_Text{ "" };
	glm::vec2 m_Scale{ 1.0f, 1.0f };
	glm::vec2 m_Position{ 0.0f, 0.0f };
	glm::vec4 m_Colour{ 1.0f,1.0f,1.0f,1.0f };

	GLuint m_VertexArrayID;
	GLuint m_VertexBufferID;
	GLuint m_IndexBufferID;
	GLuint m_ProgramID;
	glm::mat4 m_ProjectionMatrix;
	glm::ivec2* m_WindowSize = nullptr;
	std::map<GLchar, FontChar> m_CharacterMap;
};

