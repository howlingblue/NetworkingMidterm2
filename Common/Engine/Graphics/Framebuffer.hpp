#ifndef INCLUDED_FRAME_BUFFER_HPP
#define INCLUDED_FRAME_BUFFER_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include "Texture.hpp"

//-----------------------------------------------------------------------------------------------
class Framebuffer
{
	friend class Renderer;
	friend class OpenGLRenderer;

public:
	static const unsigned int MAXIMUM_COLOR_OUTPUT_TEXTURES = 4;

	enum Target
	{
		TARGET_FOR_WRITING = 1,
		TARGET_FOR_READING = 2,
		TARGET_FOR_READING_AND_WRITING = 3
	};

	Framebuffer()
		: m_framebufferID( 0 )
		, m_targetForReadingOrWriting( TARGET_FOR_READING_AND_WRITING )
	{ }

	Framebuffer( int framebufferID, Target targetForReadingOrWriting )
		: m_framebufferID( framebufferID )
		, m_targetForReadingOrWriting( targetForReadingOrWriting )
	{ }

	unsigned int GetID() const { return m_framebufferID; }
	Target GetOutputTarget() const { return m_targetForReadingOrWriting; }
	const Texture* GetAttachedColorTexture( unsigned int colorSlot ) const
	{
		assert( colorSlot < MAXIMUM_COLOR_OUTPUT_TEXTURES );
		return m_attachedColorTextures[ colorSlot ];
	}
	const Texture* GetAttachedDepthTexture() const { return m_attachedDepthTexture; }
	const Texture* GetAttachedStencilTexture() const { return m_attachedStencilTexture; }

private:
	unsigned int m_framebufferID;
	Target m_targetForReadingOrWriting;
	Texture* m_attachedColorTextures[ MAXIMUM_COLOR_OUTPUT_TEXTURES ];
	Texture* m_attachedDepthTexture;
	Texture* m_attachedStencilTexture;
};

#endif //INCLUDED_FRAME_BUFFER_HPP
