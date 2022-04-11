#version 460 core

// Input locations from vertex buffer
layout (location = 0) in vec4 l_vertex;

// Output data for next Shader
out vec2 TexCoords;
out vec2 Position;
out float LeftClipPass;
out float RightClipPass;

// Outside Variables Passed In As 'Uniforms'
uniform mat4 ProjectionMatrix;
uniform float LeftClip;
uniform float RightClip;
uniform float DeltaTime;
uniform float ScrollSpeed;
uniform bool IsScrollingRight;

vec2 alteredPosition;

// Main function that gets called per vertex.
void main()
{
	LeftClipPass = LeftClip;
	RightClipPass = RightClip;
	TexCoords = l_vertex.zw;
	alteredPosition = l_vertex.xy;
	if (IsScrollingRight)
	{
		alteredPosition.x = mod((l_vertex.x + DeltaTime * ScrollSpeed) - LeftClip, RightClip - LeftClip) + LeftClip;
	}
	else
	{
		alteredPosition.x = mod((l_vertex.x - DeltaTime * ScrollSpeed) - LeftClip, RightClip - LeftClip) + LeftClip;
	}
		
	Position = alteredPosition;

	gl_Position = ProjectionMatrix * vec4(alteredPosition, 0.0f, 1.0f);
}