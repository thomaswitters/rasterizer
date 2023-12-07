#pragma once

#include <cstdint>
#include <vector>

#include "Camera.h"
#include <iostream>

struct SDL_Window;
struct SDL_Surface;

namespace dae
{
	class Texture;
	struct Mesh;
	struct Vertex;
	class Timer;
	class Scene;

	struct Vertex_Out;


	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void Update(Timer* pTimer);
		void Render();

		bool SaveBufferToImage() const;

		void VertexTransformationFunction(const std::vector<Vertex>& vertices_in, std::vector<Vertex>& vertices_out) const;
		void VertexTransformationFunction(const std::vector<Mesh>& mesh_in, std::vector<Vertex>& vertices_out) const;
		void VertexTransformationFunction(const std::vector<Mesh>& mesh_in, std::vector<Vertex_Out>& vertices_out) const;

		void Render_W1_Part1();
		void Render_W1_Part2();
		void Render_W1_Part3();
		void Render_W1_Part4();
		void Render_W1_Part5();

		void Render_W2_Part1();
		void Render_W2_Part2();

		void Render_W3();
		void Render_W3_Part2();

		void Render_W4();
		void PixelShading(const Vertex_Out& v);

		float Remap(float value, float oldMin, float oldMax);

		void ChangeRenderColor()
		{
			renderFinalColor = !renderFinalColor;
		}


		void CycleLightingMode() {
			switch (m_CurrentLightingMode)
			{
			case LightingMode::ObservedArea:
				m_CurrentLightingMode = LightingMode::Radiance;
				std::cout << " \nLIGHTING MODE: " << "RADIANCE" << std::endl;
				break;
			case LightingMode::Radiance:
				m_CurrentLightingMode = LightingMode::BRDF;
				std::cout << " \nLIGHTING MODE: " << "BRDF" << std::endl;
				break;
			case LightingMode::BRDF:
				m_CurrentLightingMode = LightingMode::Combined;
				std::cout << " \nLIGHTING MODE: " << "COMBINED" << std::endl;
				break;
			case LightingMode::Combined:
				m_CurrentLightingMode = LightingMode::ObservedArea;
				std::cout << " \nLIGHTING MODE: " << "OBSERVED AREA" << std::endl;
				break;
			}
		};

	private:
		SDL_Window* m_pWindow{};

		SDL_Surface* m_pFrontBuffer{ nullptr };
		SDL_Surface* m_pBackBuffer{ nullptr };
		uint32_t* m_pBackBufferPixels{};

		float* m_pDepthBufferPixels{nullptr};

		Camera m_Camera{};

		int m_Width{};
		int m_Height{};

		Texture* textureUvGrid;
		Texture* textureTukTuk;
		Texture* textureVehicleDiffuse;
		Texture* textureVehicleNormal;
		Texture* textureVehicleGloss;
		Texture* textureVehicleSpecular;

		std::vector<Mesh> tuktuk;
		std::vector<Mesh> vehicle;

		float draai;

		bool renderFinalColor = true;

		ColorRGB finalColorFinal;

		ColorRGB textureColorDiffuse;
		ColorRGB textureColorNormal;
		ColorRGB textureColorGloss;
		ColorRGB textureColorSpecular;

		Vector3 normals;
		Vector3 tangent;

		enum class ShadingMode
		{
			ObservedArea,
			Diffuse,
			Specular,
			Combined
		};

		ShadingMode m_CurrentLightingMode{ ShadingMode::Combined };
	};
}
