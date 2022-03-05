#pragma once
#include "Helper.h"
static class FrameBuffer
{
public:
	inline static void InitFrameBufferDSA()
	{
		BackgroundColor[0] = 0.1f;
		BackgroundColor[1] = 0.1f;
		BackgroundColor[2] = 0.1f;
		BackgroundColor[3] = 1.0f;

		glCreateFramebuffers(1, &FrameBufferID);

		glCreateTextures(GL_TEXTURE_2D, 1, &FrameBufferTexture);
		glCreateTextures(GL_TEXTURE_2D, 1, &FrameBufferIDTexture);
		glCreateTextures(GL_TEXTURE_2D, 1, &FrameBufferHitPosTexture);
		glCreateTextures(GL_TEXTURE_2D, 1, &FrameBufferDepthTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTextureStorage2D(FrameBufferTexture, 1, GL_RGBA8, 1080, 1080);
		glNamedFramebufferTexture(FrameBufferID, GL_COLOR_ATTACHMENT0, FrameBufferTexture, 0);

		// ID's
		glTextureStorage2D(FrameBufferIDTexture, 1, GL_R32I, 1080, 1080);
		glNamedFramebufferTexture(FrameBufferID, GL_COLOR_ATTACHMENT1, FrameBufferIDTexture, 0);


		glTextureStorage2D(FrameBufferHitPosTexture, 1, GL_RGBA32F, 1080, 1080);
		glNamedFramebufferTexture(FrameBufferID, GL_COLOR_ATTACHMENT2, FrameBufferHitPosTexture, 0);


		glTextureStorage2D(FrameBufferDepthTexture, 1, GL_DEPTH_COMPONENT32F, 1080, 1080);
		glNamedFramebufferTexture(FrameBufferID, GL_DEPTH_ATTACHMENT, FrameBufferDepthTexture, 0);

		auto status = glCheckNamedFramebufferStatus(FrameBufferID, GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			Print("FrameBuffer Error ");
			std::cout << status << std::endl;
		}
	}

	inline static void ClearTexturesCustom()
	{
		// Color
		glClearNamedFramebufferfv(FrameBufferID, GL_COLOR, 0, BackgroundColor);

		// Id's
		const GLint clearID = -1;
		glClearTexImage(FrameBufferIDTexture, 0, GL_RED_INTEGER, GL_INT, &clearID);

		// Anything else that needs default values
	}

	inline static void Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
	}

	inline static void UnBind()
	{

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTextureUnit(0, FrameBufferTexture);
	}

	inline static void Cleanup()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &FrameBufferID);
		glDeleteTextures(1, &FrameBufferTexture);
		glDeleteTextures(1, &FrameBufferIDTexture);
		glDeleteTextures(1, &FrameBufferDepthTexture);
		glDeleteTextures(1, &FrameBufferHitPosTexture);
	}

	inline static int GrabIDUnderMouse(float _mouseX, float _mouseY)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer::FrameBufferID);
		GLenum buffers[] = { GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_DEPTH_ATTACHMENT };
		glDrawBuffers(2, buffers);
		glReadBuffer(GL_COLOR_ATTACHMENT1);

		int* pixels = new int;
		glReadPixels(_mouseX, 1080 - _mouseY, 1, 1, GL_RED_INTEGER, GL_INT, pixels);
		int returnValue = *pixels;
		Print(*pixels);
		delete pixels;
		pixels = nullptr;

		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return returnValue;
	}

	inline static glm::vec3 GrabColourUnderMouse(float _mouseX, float _mouseY)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer::FrameBufferID);

		GLenum buffers[] = { GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_DEPTH_ATTACHMENT };
		glDrawBuffers(1, buffers);
		glReadBuffer(GL_COLOR_ATTACHMENT0);

		GLfloat* pixels = new GLfloat[4];
		glReadPixels(_mouseX, 1080 - _mouseY, 1, 1, GL_RGBA, GL_FLOAT, pixels);
		glm::vec3 returnValue = { pixels[0], pixels[1], pixels[2] };
		{
			std::string output = "";
			output += std::to_string(pixels[0]);
			output += "|";
			output += std::to_string(pixels[1]);
			output += "|";
			output += std::to_string(pixels[2]);
			Print(output);
		}
		delete[] pixels;
		pixels = nullptr;

		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return returnValue;
	}

	inline static glm::vec3 GrabMousePositionIn3D(float _mouseX, float _mouseY)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer::FrameBufferID);

		GLenum buffers[] = { GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_DEPTH_ATTACHMENT };
		glDrawBuffers(3, buffers);
		glReadBuffer(GL_COLOR_ATTACHMENT2);

		GLfloat* pixels = new GLfloat[4];
		glReadPixels(_mouseX, 1080 - _mouseY, 1, 1, GL_RGBA, GL_FLOAT, pixels);
		glm::vec3 returnValue = { pixels[0], pixels[1], pixels[2] };
		{
			std::string output = "";
			output += std::to_string(pixels[0]);
			output += "|";
			output += std::to_string(pixels[1]);
			output += "|";
			output += std::to_string(pixels[2]);
			Print(output);
		}
		delete[] pixels;
		pixels = nullptr;

		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return returnValue;
	}

	inline static unsigned FrameBufferTexture;
	inline static unsigned FrameBufferDepthTexture;
	inline static unsigned FrameBufferIDTexture;
	inline static unsigned FrameBufferHitPosTexture;
	inline static unsigned FrameBufferID;

	inline static GLfloat BackgroundColor[4];
};

