#pragma once
#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_filePath;
	unsigned char* m_localBuffer;
	int m_width, m_height, m_BPP;

public:
	Texture(const std::string& filePath);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int getWidth() const { return m_width; }
	inline int getHeight() const { return m_height; }

};

