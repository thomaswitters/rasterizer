#include "Texture.h"
#include "Vector2.h"
#include <SDL_image.h>
#include <iostream>

namespace dae
{
	Texture::Texture(SDL_Surface* pSurface) :
		m_pSurface{ pSurface },
		m_pSurfacePixels{ (uint32_t*)pSurface->pixels }
	{
		m_pSurface = SDL_CreateRGBSurface(0, m_pSurface->w, m_pSurface->h, 32, 0, 0, 0, 0);
	}

	Texture::~Texture()
	{
		if (m_pSurface)
		{
			SDL_FreeSurface(m_pSurface);
			m_pSurface = nullptr;
		}
	}

	Texture* Texture::LoadFromFile(const std::string& path)
	{
		//TODO
		//Load SDL_Surface using IMG_LOAD
		//Create & Return a new Texture Object (using SDL_Surface)

		// Load image using SDL_image
		SDL_Surface* loadedSurface = IMG_Load(path.c_str());

		return new Texture (loadedSurface);
	}

	ColorRGB Texture::Sample(const Vector2& uv) const
	{
		//SDL_Surface* m_pSurface{ nullptr };
		//uint32_t* m_pSurfacePixels{ nullptr };
		//TODO
		//Sample the correct texel for the given uv
		//Texture::Sample > Samples a specific pixel from the SurfacePixels 
		//m_pSurfacePixels
	
		int texWidth = m_pSurface->w;
		int texHeight = m_pSurface->h;

		int pixelX = static_cast<int>(uv.x * texWidth);
		int pixelY = static_cast<int>(uv.y * texHeight);

		pixelX = std::max(0, std::min(texWidth - 1, pixelX));
		pixelY = std::max(0, std::min(texHeight - 1, pixelY));

		int pixelIndex = pixelY * texWidth + pixelX;

		uint8_t r, g, b;
		SDL_GetRGB(m_pSurfacePixels[pixelIndex], m_pSurface->format, &r, &g, &b);

		float normalizedB = r / 255.0f;
		float normalizedG = g / 255.0f;
		float normalizedR = b / 255.0f;

		return ColorRGB(normalizedR, normalizedG, normalizedB);
	}
}