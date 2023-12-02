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

		// Check if the image loaded successfully
		//if (!loadedSurface)
		//{
		//	// Handle the error, for example, print an error message
		//	std::cerr << "Failed to load image: " << SDL_GetError() << std::endl;
		//	return nullptr; // or return a default texture
		//}

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
	
		// Get the width and height of the texture
		int texWidth = m_pSurface->w;
		int texHeight = m_pSurface->h;

		// Map UV coordinates to pixel coordinates
		int pixelX = static_cast<int>(uv.x * texWidth);
		int pixelY = static_cast<int>(uv.y * texHeight);

		// Ensure that pixelX and pixelY are within valid bounds
		pixelX = std::max(0, std::min(texWidth - 1, pixelX));
		pixelY = std::max(0, std::min(texHeight - 1, pixelY));

		// Calculate the index of the pixel in the 1D array
		int pixelIndex = pixelY * texWidth + pixelX;

		// Read the color from the pixel using SDL_GetRGB
		uint8_t r, g, b;
		SDL_GetRGB(m_pSurfacePixels[pixelIndex], m_pSurface->format, &r, &g, &b);

		// Remap the color to the [0, 1] range and swap red and blue
		float normalizedB = r / 255.0f;
		float normalizedG = g / 255.0f;
		float normalizedR = b / 255.0f;

		// Return the sampled color
		return ColorRGB(normalizedR, normalizedG, normalizedB);
	}
}