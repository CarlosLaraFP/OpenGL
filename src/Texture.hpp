#pragma once

#include <string>

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP; // bits per pixel
	unsigned int m_Slot;

public:
	Texture(const std::string& path);
	~Texture();
	Texture(Texture&& source) noexcept; // move constructor
	Texture& operator=(Texture&& source) noexcept; // move assignment operator
	Texture(const Texture& source) = delete; // copy constructor
	Texture& operator=(const Texture& source) = delete; // copy assignment operator

	void Bind(unsigned int slot = 0);
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline unsigned int GetSlot() const { return m_Slot; }
};
