#include "Texture.h"

#include "vender/stb_image/stb_image.h"

Texture::Texture(const std::string& filepath) :
	m_RedererID(0),
	m_FilePath(filepath),
	m_LocalBuffer(nullptr),
	m_Width(0),
	m_Height(0),
	m_BPP(0)
{
	// flip texture upside down
	// OpenGL read a texture from bottom-left
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(filepath.c_str(),
		&m_Width,
		&m_Height,
		&m_BPP,
		4 // RGBA
	);
	CALLGL(glGenTextures(1, &m_RedererID));
	CALLGL(glBindTexture(GL_TEXTURE_2D, m_RedererID));

	CALLGL(glTexParameteri(
		GL_TEXTURE_2D,
		// how our texture will actually be resampled down if it needs
		// to be rendered smaller than it is kind of per pixel.
		// we're gonna have an in-depth episode on all of these kind of
		// texture parameters because like dealing with MIT maps and kind 
		// of minification and magnification filtering and like you
		// know edge clamping all that is it kind of a big topic then I don't want to just
		// squeeze it in here so we will talk about this stuff in more detail in the future
		GL_TEXTURE_MIN_FILTER,

		// set to geo linear which just going to linearly resample
		// everything so we'll kind of still look
		// ok it's not going to snap at the nearest
		// pixel which is what gel nearest would do
		GL_LINEAR
	));
	CALLGL(glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER,
		GL_LINEAR
	));
	CALLGL(glTexParameteri(
		GL_TEXTURE_2D,

		// the horizontal wrap so we wanted to clamp which means that we 
		// wanted to basically not extend the area
		GL_TEXTURE_WRAP_S,
		GL_CLAMP_TO_EDGE
	));
	CALLGL(glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T,
		GL_CLAMP_TO_EDGE
	));


	CALLGL(glTexImage2D(
		GL_TEXTURE_2D,

		// later
		0,
		GL_RGBA8,
		m_Width, m_Height,

		// border
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		m_LocalBuffer));

	CALLGL(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}
Texture::~Texture()
{
	CALLGL(glDeleteTextures(1, &m_RedererID));
}

void Texture::Bind(unsigned int slot) const
{
	CALLGL(glActiveTexture(GL_TEXTURE0 + slot));
	CALLGL(glBindTexture(GL_TEXTURE_2D, m_RedererID));
}
void Texture::Unbind() const
{
	CALLGL(glBindTexture(GL_TEXTURE_2D, 0));
}