#version 460 core

// Input locations from vertex buffer
layout (location = 0) in vec4 l_vertex;

// Output data for next Shader
out vec2 TexCoords;
out vec2 Position;
out float LeftClipPass;
out float RightClipPass;

// Outside Variables Passed In As 'Uniforms'
uniform mat4 PMatrix;
uniform float LeftClip;
uniform float RightClip;
uniform float ElapsedTime;
uniform float ScrollSpeed;
uniform bool IsScrollingRight;
uniform bool IsScrolling;

// Variables
vec2 alteredPosition;

// Main function that gets called per vertex.
void main()
{
	LeftClipPass = LeftClip;
	RightClipPass = RightClip;
	TexCoords = l_vertex.zw;
	alteredPosition = l_vertex.xy;
	if (IsScrolling)
	{
		if (IsScrollingRight)
		{
			alteredPosition.x = mod((l_vertex.x + ElapsedTime * ScrollSpeed) - LeftClip, RightClip - LeftClip) + LeftClip;
		}
		else
		{
			alteredPosition.x = mod((l_vertex.x - ElapsedTime * ScrollSpeed) - LeftClip, RightClip - LeftClip) + LeftClip;
		}
	}
		
	Position = alteredPosition;

	gl_Position = PMatrix * vec4(alteredPosition, 0.0f, 1.0f);
}