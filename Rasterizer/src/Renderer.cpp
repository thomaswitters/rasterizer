//External includes
#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include "Renderer.h"
#include "Maths.h"
#include "Texture.h"
#include "Utils.h"

using namespace dae;

Renderer::Renderer(SDL_Window* pWindow) :
	m_pWindow(pWindow)
{
	//Initialize
	SDL_GetWindowSize(pWindow, &m_Width, &m_Height);

	//Create Buffers
	m_pFrontBuffer = SDL_GetWindowSurface(pWindow);
	m_pBackBuffer = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0, 0, 0, 0);
	m_pBackBufferPixels = (uint32_t*)m_pBackBuffer->pixels;

	m_pDepthBufferPixels = new float[m_Width * m_Height];
	//Initialize Camera
	m_Camera.Initialize(60.f, { .0f,.0f,-10.f });

	texture = Texture::LoadFromFile("Resources/uv_grid_2.png");
}

Renderer::~Renderer()
{
	delete[] m_pDepthBufferPixels;
	delete texture;
}

void Renderer::Update(Timer* pTimer)
{
	m_Camera.Update(pTimer);
}

void Renderer::Render()
{
	//@START
	//Lock BackBuffer
	SDL_LockSurface(m_pBackBuffer);


	//Render_W1_Part1();
	//Render_W1_Part2();
	//Render_W1_Part3();
	//Render_W1_Part4();
	//Render_W1_Part5();
	//Render_W2_Part1();
	Render_W2_Part2();

	
		

	//@END
	//Update SDL Surface
	SDL_UnlockSurface(m_pBackBuffer);
	SDL_BlitSurface(m_pBackBuffer, 0, m_pFrontBuffer, 0);
	SDL_UpdateWindowSurface(m_pWindow);
}

void Renderer::VertexTransformationFunction(const std::vector<Vertex>& vertices_in, std::vector<Vertex>& vertices_out) const
{
	float aspectRatio = static_cast<float>(m_Width) / static_cast<float>(m_Height);
	vertices_out.clear();

	for (const auto& vertexIn : vertices_in)
	{
		//Transform the point from world space to camera space, often called View Space, by multiplying it with the inverse of the camera matrix(ONB).
		Vector3 transformedVertex = m_Camera.viewMatrix.TransformPoint(vertexIn.position);

		// Perform perspective divide.
		transformedVertex.x /= transformedVertex.z;
		transformedVertex.y /= transformedVertex.z;

		transformedVertex.x /= aspectRatio * m_Camera.fov;
		transformedVertex.y /= m_Camera.fov;

		vertices_out.emplace_back(Vertex{ transformedVertex , vertexIn.color, vertexIn.uv });
	}
}
void Renderer::VertexTransformationFunction(const std::vector<Mesh>& mesh_in, std::vector<Vertex>& vertices_out) const
{
	float aspectRatio = static_cast<float>(m_Width) / static_cast<float>(m_Height);
	vertices_out.clear();

	for (const auto& mesh : mesh_in)
	{
		for (const auto& mesh_index: mesh.indices)
		{
			const auto& vertexIn = mesh.vertices[mesh_index];
			// Transform the point from world space to camera space
			Vector3 transformedVertex = m_Camera.viewMatrix.TransformPoint(vertexIn.position);

			// Perform perspective divide.
			transformedVertex.x /= transformedVertex.z;
			transformedVertex.y /= transformedVertex.z;

			transformedVertex.x /= aspectRatio * m_Camera.fov;
			transformedVertex.y /= m_Camera.fov;

			vertices_out.emplace_back(Vertex{ transformedVertex, vertexIn.color, vertexIn.uv });
		}
	}
}

void Renderer::Render_W2_Part2()
{

	std::fill_n(m_pDepthBufferPixels, m_Width * m_Height, std::numeric_limits<float>::max());
	SDL_FillRect(m_pBackBuffer, nullptr, SDL_MapRGB(m_pBackBuffer->format, 100, 100, 100));

	std::vector<Mesh> meshes1
	{
		Mesh{
			{
				Vertex{{-3, 3, -2}, {1.f,1.f,1.f}, {0.f, 0.f}},
				Vertex{{0, 3, -2}, {1.f,1.f,1.f}, {0.5f, 0.f}},
				Vertex{{3, 3, -2}, {1.f,1.f,1.f}, {1.f, 0.f}},
				Vertex{{-3, 0, -2}, {1.f,1.f,1.f}, {0.f, 0.5f}},
				Vertex{{0, 0, -2}, {1.f,1.f,1.f}, {0.5f, 0.5f}},
				Vertex{{3, 0, -2}, {1.f,1.f,1.f}, {1.f, 0.5f}},
				Vertex{{-3, -3, -2}, {1.f,1.f,1.f}, {0.f, 1.f}},
				Vertex{{0, -3, -2}, {1.f,1.f,1.f}, {0.5f, 1.f}},
				Vertex{{3, -3, -2}, {1.f,1.f,1.f}, {1.f, 1.f}}
			},
				{
				3, 0, 4, 1, 5, 2,
				2, 6,
				6, 3, 7, 4, 8, 5
			},
			PrimitiveTopology::TriangleStrip
		}
	};
	std::vector<Mesh> meshes2
	{
		Mesh{
				{
				Vertex{{-3, 3, -2}, {1.f,1.f,1.f}, {0.f, 0.f}},
				Vertex{{0, 3, -2}, {1.f,1.f,1.f}, {0.5f, 0.f}},
				Vertex{{3, 3, -2}, {1.f,1.f,1.f}, {1.f, 0.f}},
				Vertex{{-3, 0, -2}, {1.f,1.f,1.f}, {0.f, 0.5f}},
				Vertex{{0, 0, -2}, {1.f,1.f,1.f}, {0.5f, 0.5f}},
				Vertex{{3, 0, -2}, {1.f,1.f,1.f}, {1.f, 0.5f}},
				Vertex{{-3, -3, -2}, {1.f,1.f,1.f}, {0.f, 1.f}},
				Vertex{{0, -3, -2}, {1.f,1.f,1.f}, {0.5f, 1.f}},
				Vertex{{3, -3, -2}, {1.f,1.f,1.f}, {1.f, 1.f}}
			},
				{
				3, 0, 1,	1, 4, 3,	4, 1, 2,
				2, 5, 4,	6, 3, 4,	4, 7, 6,
				7, 4, 5,	5, 8, 7

			},
			PrimitiveTopology::TriangleList
		}
	};


	// decide which one to use
	std::vector<Mesh> meshes = meshes2;

	std::vector<Vertex> vertices_projected;
	VertexTransformationFunction(meshes, vertices_projected);

	for (auto mesh : meshes) {
		PrimitiveTopology method = mesh.primitiveTopology;
		int step = (method == PrimitiveTopology::TriangleList) ? 3 : 1;
		int sizeDecrease = std::abs(step - 3); // for TriangleString I've noticed I need decrease looping over the vertices_projected.size by 2

		for (size_t i = 0; i < vertices_projected.size() - sizeDecrease; i += step)
		{
			Vector2 v0(((vertices_projected[i].position.x + 1.f) / 2) * m_Width, ((1.0f - vertices_projected[i].position.y) / 2) * m_Height);
			Vector2 v1(((vertices_projected[i + 1].position.x + 1.f) / 2) * m_Width, ((1.0f - vertices_projected[i + 1].position.y) / 2) * m_Height);
			Vector2 v2(((vertices_projected[i + 2].position.x + 1.f) / 2) * m_Width, ((1.0f - vertices_projected[i + 2].position.y) / 2) * m_Height);

			//TODO: Every other triangle, which has an odd index, is counter - clockwise.To make them clockwise again, we reverse the last two vertices when reading these.
			/*if (mesh.primitiveTopology == PrimitiveTopology::TriangleStrip && i % 2 != 0)
			{
				std::swap(v1, v2);
			}*/

			int min_x = static_cast<int>(std::min({ v0.x, v1.x, v2.x }));
			int min_y = static_cast<int>(std::min({ v0.y, v1.y, v2.y }));
			int max_x = static_cast<int>(std::max({ v0.x, v1.x, v2.x }));
			int max_y = static_cast<int>(std::max({ v0.y, v1.y, v2.y }));

			// Ensure bounding box is within screen boundaries
			min_x = std::max(0, std::min(min_x, m_Width - 1));
			min_y = std::max(0, std::min(min_y, m_Height - 1));
			max_x = std::max(0, std::min(max_x, m_Width - 1));
			max_y = std::max(0, std::min(max_y, m_Height - 1));

			for (int px = min_x; px <= max_x; ++px)
			{
				for (int py = min_y; py <= max_y; ++py)
				{
					Vector2 point(px + 0.5f, py + 0.5f);

					ColorRGB finalColor{ 0, 0, 0 };

					float cross1 = Vector2::Cross(v1 - v0, point - v0);
					float cross2 = Vector2::Cross(v2 - v1, point - v1);
					float cross3 = Vector2::Cross(v0 - v2, point - v2);
					float total = cross1 + cross2 + cross3;

					cross1 /= total;
					cross2 /= total;
					cross3 /= total;

					if ((cross1 >= 0.0f) == (cross2 >= 0.0f) && (cross2 >= 0.0f) == (cross3 >= 0.0f))
					{
						float Zinterpolated = 1.0f / ((1 / vertices_projected[i].position.z) * cross2 +
							(1 / vertices_projected[i + 1].position.z) * cross3 +
							(1 / vertices_projected[i + 2].position.z) * cross1);

						m_pDepthBufferPixels[px + (py * m_Width)] = Zinterpolated;

						Vector2 uv = ((vertices_projected[i].uv / vertices_projected[i].position.z) * cross2 +
							(vertices_projected[i + 1].uv / vertices_projected[i + 1].position.z) * cross3 +
							(vertices_projected[i + 2].uv / vertices_projected[i + 2].position.z) * cross1) * Zinterpolated;

						ColorRGB textureColor = texture->Sample(uv);

						finalColor = textureColor * (ColorRGB{ vertices_projected[i].color.b, vertices_projected[i].color.g, vertices_projected[i].color.r } *cross2 + ColorRGB{ vertices_projected[i + 1].color.b, vertices_projected[i + 1].color.g, vertices_projected[i + 1].color.r } *cross3 + ColorRGB{ vertices_projected[i + 2].color.b, vertices_projected[i + 2].color.g, vertices_projected[i + 2].color.r } *cross1);

						finalColor.MaxToOne();

						m_pBackBufferPixels[px + (py * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
							static_cast<uint8_t>(finalColor.r * 255),
							static_cast<uint8_t>(finalColor.g * 255),
							static_cast<uint8_t>(finalColor.b * 255));
					}
				}
			}
		}
	}
}
void Renderer::Render_W2_Part1()
{

	std::fill_n(m_pDepthBufferPixels, m_Width * m_Height, std::numeric_limits<float>::max());
	SDL_FillRect(m_pBackBuffer, nullptr, SDL_MapRGB(m_pBackBuffer->format, 100, 100, 100));

	std::vector<Mesh> meshes1
	{
		Mesh{
			{
				Vertex{{-3, 3, -2}, {1.f,1.f,1.f}, {0.f, 0.f}},
				Vertex{{0, 3, -2}, {1.f,1.f,1.f}, {0.5f, 0.f}},
				Vertex{{3, 3, -2}, {1.f,1.f,1.f}, {1.f, 0.f}},
				Vertex{{-3, 0, -2}, {1.f,1.f,1.f}, {0.f, 0.5f}},
				Vertex{{0, 0, -2}, {1.f,1.f,1.f}, {0.5f, 0.5f}},
				Vertex{{3, 0, -2}, {1.f,1.f,1.f}, {1.f, 0.5f}},
				Vertex{{-3, -3, -2}, {1.f,1.f,1.f}, {0.f, 1.f}},
				Vertex{{0, -3, -2}, {1.f,1.f,1.f}, {0.5f, 1.f}},
				Vertex{{3, -3, -2}, {1.f,1.f,1.f}, {1.f, 1.f}}
			},
				{
				3, 0, 4, 1, 5, 2,
				2, 6,
				6, 3, 7, 4, 8, 5
			},
			PrimitiveTopology::TriangleStrip
		}
	};
	std::vector<Mesh> meshes2
	{
		Mesh{
				{
				Vertex{{-3, 3, -2}, {1.f,1.f,1.f}, {0.f, 0.f}},
				Vertex{{0, 3, -2}, {1.f,1.f,1.f}, {0.5f, 0.f}},
				Vertex{{3, 3, -2}, {1.f,1.f,1.f}, {1.f, 0.f}},
				Vertex{{-3, 0, -2}, {1.f,1.f,1.f}, {0.f, 0.5f}},
				Vertex{{0, 0, -2}, {1.f,1.f,1.f}, {0.5f, 0.5f}},
				Vertex{{3, 0, -2}, {1.f,1.f,1.f}, {1.f, 0.5f}},
				Vertex{{-3, -3, -2}, {1.f,1.f,1.f}, {0.f, 1.f}},
				Vertex{{0, -3, -2}, {1.f,1.f,1.f}, {0.5f, 1.f}},
				Vertex{{3, -3, -2}, {1.f,1.f,1.f}, {1.f, 1.f}}
			},
				{
				3, 0, 1,	1, 4, 3,	4, 1, 2,
				2, 5, 4,	6, 3, 4,	4, 7, 6,
				7, 4, 5,	5, 8, 7

			},
			PrimitiveTopology::TriangleList
		}
	};


	// decide which one to use
	std::vector<Mesh> meshes = meshes2;

	std::vector<Vertex> vertices_projected;
	VertexTransformationFunction(meshes, vertices_projected);

	for (auto mesh : meshes) {
		PrimitiveTopology method = mesh.primitiveTopology;
		int step = (method == PrimitiveTopology::TriangleList) ? 3 : 1;
		int sizeDecrease = std::abs(step - 3); // for TriangleString I've noticed I need decrease looping over the vertices_projected.size by 2

		for (size_t i = 0; i < vertices_projected.size() - sizeDecrease; i += step)
		{
			Vector2 v0(((vertices_projected[i].position.x + 1.f) / 2) * m_Width, ((1.0f - vertices_projected[i].position.y) / 2) * m_Height);
			Vector2 v1(((vertices_projected[i + 1].position.x + 1.f) / 2) * m_Width, ((1.0f - vertices_projected[i + 1].position.y) / 2) * m_Height);
			Vector2 v2(((vertices_projected[i + 2].position.x + 1.f) / 2) * m_Width, ((1.0f - vertices_projected[i + 2].position.y) / 2) * m_Height);

			//TODO: Every other triangle, which has an odd index, is counter - clockwise.To make them clockwise again, we reverse the last two vertices when reading these.
			/*if (mesh.primitiveTopology == PrimitiveTopology::TriangleStrip && i % 2 != 0)
			{
				std::swap(v1, v2);
			}*/

			int min_x = static_cast<int>(std::min({ v0.x, v1.x, v2.x }));
			int min_y = static_cast<int>(std::min({ v0.y, v1.y, v2.y }));
			int max_x = static_cast<int>(std::max({ v0.x, v1.x, v2.x }));
			int max_y = static_cast<int>(std::max({ v0.y, v1.y, v2.y }));

			// Ensure bounding box is within screen boundaries
			min_x = std::max(0, std::min(min_x, m_Width - 1));
			min_y = std::max(0, std::min(min_y, m_Height - 1));
			max_x = std::max(0, std::min(max_x, m_Width - 1));
			max_y = std::max(0, std::min(max_y, m_Height - 1));

			for (int px = min_x; px <= max_x; ++px)
			{
				for (int py = min_y; py <= max_y; ++py)
				{
					Vector2 point(px + 0.5f, py + 0.5f);

					ColorRGB finalColor{ 0, 0, 0 };

					float cross1 = Vector2::Cross(v1 - v0, point - v0);
					float cross2 = Vector2::Cross(v2 - v1, point - v1);
					float cross3 = Vector2::Cross(v0 - v2, point - v2);
					float total = cross1 + cross2 + cross3;

					cross1 /= total;
					cross2 /= total;
					cross3 /= total;

					if ((cross1 >= 0.0f) == (cross2 >= 0.0f) && (cross2 >= 0.0f) == (cross3 >= 0.0f))
					{
						float depth = cross2 * vertices_projected[i].position.z +
							cross3 * vertices_projected[i + 1].position.z +
							cross1 * vertices_projected[i + 2].position.z;

						if (depth < m_pDepthBufferPixels[px + (py * m_Width)])
						{
							m_pDepthBufferPixels[px + (py * m_Width)] = depth;

							Vector2 uv = vertices_projected[i].uv * cross2 +
								vertices_projected[i + 1].uv * cross3 +
								vertices_projected[i + 2].uv * cross1;

							ColorRGB textureColor = texture->Sample(uv);

							finalColor = textureColor 
								* 
							(ColorRGB{ vertices_projected[i].color.b, vertices_projected[i].color.g, vertices_projected[i].color.r } * cross2
								+
							ColorRGB{ vertices_projected[i + 1].color.b, vertices_projected[i + 1].color.g, vertices_projected[i + 1].color.r } * cross3
								+
							ColorRGB{ vertices_projected[i + 2].color.b, vertices_projected[i + 2].color.g, vertices_projected[i + 2].color.r } * cross1);

							finalColor.MaxToOne();

							m_pBackBufferPixels[px + (py * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
								static_cast<uint8_t>(finalColor.r * 255),
								static_cast<uint8_t>(finalColor.g * 255),
								static_cast<uint8_t>(finalColor.b * 255));
						}
					}
				}
			}
		}
	}
}

void Renderer::Render_W1_Part5()
{
	std::fill_n(m_pDepthBufferPixels, m_Width * m_Height, std::numeric_limits<float>::max());
	SDL_FillRect(m_pBackBuffer, nullptr, SDL_MapRGB(m_pBackBuffer->format, 100, 100, 100));


	std::vector<Vertex> vertices_world
	{
		//Triagle 0
		{{0.f, 2.0f, 0.f}, {1.f,0.f,0.f}},
		{{1.5f, -1.f, 0.f}, {1.f, 0.f, 0.f}},
		{{-1.5f, -1.f, 0.f}, {1.f, 0.f, 0.f}},

		//Triagle 1
		{{0.f, 4.f, 2.f}, {1,0,0}},
		{{3.f, -2.f, 2.f}, {0, 1, 0}},
		{{-3.f, -2.f, 2.f}, {0, 0, 1}}
	};

	std::vector<Vertex> vertices_projected;
	VertexTransformationFunction(vertices_world, vertices_projected);

	for (size_t triangleIndex = 0; triangleIndex < vertices_projected.size() - 2; triangleIndex += 3)
	{
		Vector2 v0(((vertices_projected[triangleIndex].position.x + 1.f) / 2) * m_Width, ((1.0f - vertices_projected[triangleIndex].position.y) / 2) * m_Height);
		Vector2 v1(((vertices_projected[triangleIndex + 1].position.x + 1.f) / 2) * m_Width, ((1.0f - vertices_projected[triangleIndex + 1].position.y) / 2) * m_Height);
		Vector2 v2(((vertices_projected[triangleIndex + 2].position.x + 1.f) / 2) * m_Width, ((1.0f - vertices_projected[triangleIndex + 2].position.y) / 2) * m_Height);

		int min_x = static_cast<int>(std::min({ v0.x, v1.x, v2.x }));
		int min_y = static_cast<int>(std::min({ v0.y, v1.y, v2.y }));
		int max_x = static_cast<int>(std::max({ v0.x, v1.x, v2.x }));
		int max_y = static_cast<int>(std::max({ v0.y, v1.y, v2.y }));

		// Ensure bounding box is within screen boundaries
		min_x = std::max(0, std::min(min_x, m_Width - 1));
		min_y = std::max(0, std::min(min_y, m_Height - 1));
		max_x = std::max(0, std::min(max_x, m_Width - 1));
		max_y = std::max(0, std::min(max_y, m_Height - 1));

		for (int px = min_x; px <= max_x; ++px)
		{
			for (int py = min_y; py <= max_y; ++py)
			{
				Vector2 point(px + 0.5f, py + 0.5f);

				ColorRGB finalColor{ 0, 0, 0 };

				float cross1 = Vector2::Cross(v1 - v0, point - v0);
				float cross2 = Vector2::Cross(v2 - v1, point - v1);
				float cross3 = Vector2::Cross(v0 - v2, point - v2);
				float total = cross1 + cross2 + cross3;

				cross1 /= total;
				cross2 /= total;
				cross3 /= total;

				if ((cross1 >= 0.0f) == (cross2 >= 0.0f) && (cross2 >= 0.0f) == (cross3 >= 0.0f))
				{
					float depth = vertices_projected[triangleIndex].position.z * cross1 +
						vertices_projected[triangleIndex + 1].position.z * cross2 +
						vertices_projected[triangleIndex + 2].position.z * cross3;


					// Depth test
					if (depth < m_pDepthBufferPixels[px + (py * m_Width)])
					{
						// Update depth buffer and render pixel
						m_pDepthBufferPixels[px + (py * m_Width)] = depth;

						finalColor = ColorRGB(
							vertices_projected[triangleIndex].color.r * cross2 + vertices_projected[triangleIndex + 1].color.r * cross3 + vertices_projected[triangleIndex + 2].color.r * cross1,
							vertices_projected[triangleIndex].color.g * cross2 + vertices_projected[triangleIndex + 1].color.g * cross3 + vertices_projected[triangleIndex + 2].color.g * cross1,
							vertices_projected[triangleIndex].color.b * cross2 + vertices_projected[triangleIndex + 1].color.b * cross3 + vertices_projected[triangleIndex + 2].color.b * cross1
						);

						finalColor.MaxToOne();

						m_pBackBufferPixels[px + (py * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
							static_cast<uint8_t>(finalColor.r * 255),
							static_cast<uint8_t>(finalColor.g * 255),
							static_cast<uint8_t>(finalColor.b * 255));
					}


				}


			}
		}
	}
}
void Renderer::Render_W1_Part4()
{


	// Initialize depth buffer with maximum float values
	if (!m_pDepthBufferPixels)
	{
		m_pDepthBufferPixels = new float[m_Width * m_Height];
		std::fill_n(m_pDepthBufferPixels, m_Width * m_Height, std::numeric_limits<float>::max());
	}

	// Clear depth buffer
	std::fill_n(m_pDepthBufferPixels, m_Width * m_Height, std::numeric_limits<float>::max());

	// Clear back buffer
	SDL_FillRect(m_pBackBuffer, nullptr, SDL_MapRGB(m_pBackBuffer->format, 100, 100, 100));


	std::vector<Vertex> vertices_world
	{
		//Triagle 0
		{{0.f, 2.0f, 0.f}, {1.f,0.f,0.f}},
		{{1.5f, -1.f, 0.f}, {1.f, 0.f, 0.f}},
		{{-1.5f, -1.f, 0.f}, {1.f, 0.f, 0.f}},

		//Triagle 1
		{{0.f, 4.f, 2.f}, {1,0,0}},
		{{3.f, -2.f, 2.f}, {0, 1, 0}},
		{{-3.f, -2.f, 2.f}, {0, 0, 1}}
	};

	std::vector<Vertex> vertices_projected;
	VertexTransformationFunction(vertices_world, vertices_projected);

	for (size_t triangleIndex = 0; triangleIndex < vertices_projected.size(); triangleIndex += 3)
	{
		Vector2 v0(((vertices_projected[triangleIndex].position.x + 1.f) / 2) * m_Width, ((1.0f - vertices_projected[triangleIndex].position.y) / 2) * m_Height);
		Vector2 v1(((vertices_projected[triangleIndex + 1].position.x + 1.f) / 2) * m_Width, ((1.0f - vertices_projected[triangleIndex + 1].position.y) / 2) * m_Height);
		Vector2 v2(((vertices_projected[triangleIndex + 2].position.x + 1.f) / 2) * m_Width, ((1.0f - vertices_projected[triangleIndex + 2].position.y) / 2) * m_Height);

		for (int px{}; px < m_Width; ++px)
		{
			for (int py{}; py < m_Height; ++py)
			{
				Vector2 point(px + 0.5f, py + 0.5f);

				ColorRGB finalColor{ 0, 0, 0 };

				float cross1 = Vector2::Cross(v1 - v0, point - v0);
				float cross2 = Vector2::Cross(v2 - v1, point - v1);
				float cross3 = Vector2::Cross(v0 - v2, point - v2);
				float total = cross1 + cross2 + cross3;

				cross1 /= total;
				cross2 /= total;
				cross3 /= total;

				if ((cross1 >= 0.0f) == (cross2 >= 0.0f) && (cross2 >= 0.0f) == (cross3 >= 0.0f))
				{
					float depth = vertices_projected[triangleIndex].position.z * cross2 +
						vertices_projected[triangleIndex + 1].position.z * cross3 +
						vertices_projected[triangleIndex + 2].position.z * cross1;


					// Depth test
					if (depth < m_pDepthBufferPixels[px + (py * m_Width)])
					{
						// Update depth buffer and render pixel
						m_pDepthBufferPixels[px + (py * m_Width)] = depth;

						finalColor = ColorRGB(
							vertices_projected[triangleIndex].color.r * cross2 + vertices_projected[triangleIndex + 1].color.r * cross3 + vertices_projected[triangleIndex + 2].color.r * cross1,
							vertices_projected[triangleIndex].color.g * cross2 + vertices_projected[triangleIndex + 1].color.g * cross3 + vertices_projected[triangleIndex + 2].color.g * cross1,
							vertices_projected[triangleIndex].color.b * cross2 + vertices_projected[triangleIndex + 1].color.b * cross3 + vertices_projected[triangleIndex + 2].color.b * cross1
						);

						finalColor.MaxToOne();

						m_pBackBufferPixels[px + (py * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
							static_cast<uint8_t>(finalColor.r * 255),
							static_cast<uint8_t>(finalColor.g * 255),
							static_cast<uint8_t>(finalColor.b * 255));
					}
					
						
				}
				
			
			}
		}
	}
}
void Renderer::Render_W1_Part3()
{
	std::vector<Vertex> vertices_world
	{
		//Triagle 1
		{{0.f, 4.f, 2.f}, {1,0,0}},
		{{3.f, -2.f, 2.f}, {0, 1, 0}},
		{{-3.f, -2.f, 2.f}, {0, 0, 1}}
	};

	std::vector<Vertex> vertices_projected;
	VertexTransformationFunction(vertices_world, vertices_projected);

	const int arraySize = 10;
	float floatArray[arraySize];
	std::fill_n(floatArray, arraySize, std::numeric_limits<float>::max());

	for (size_t triangleIndex = 0; triangleIndex < vertices_projected.size(); triangleIndex += 3)
	{
		Vector2 v0(((vertices_projected[triangleIndex].position.x + 1.f) / 2) * m_Width, ((1.0f - vertices_projected[triangleIndex].position.y) / 2) * m_Height);
		Vector2 v1(((vertices_projected[triangleIndex + 1].position.x + 1.f) / 2) * m_Width, ((1.0f - vertices_projected[triangleIndex + 1].position.y) / 2) * m_Height);
		Vector2 v2(((vertices_projected[triangleIndex + 2].position.x + 1.f) / 2) * m_Width, ((1.0f - vertices_projected[triangleIndex + 2].position.y) / 2) * m_Height);

		for (int px{}; px < m_Width; ++px)
		{
			for (int py{}; py < m_Height; ++py)
			{
				Vector2 point(px + 0.5f, py + 0.5f);

				float gradient = px / static_cast<float>(m_Width);
				gradient += py / static_cast<float>(m_Width);
				gradient /= 2.0f;

				ColorRGB finalColor{ 0, 0, 0 };

				float cross1 = Vector2::Cross(v1 - v0, point - v0);
				float cross2 = Vector2::Cross(v2 - v1, point - v1);
				float cross3 = Vector2::Cross(v0 - v2, point - v2);
				float total = cross1 + cross2 + cross3;

				cross1 /= total;
				cross2 /= total;
				cross3 /= total;

				if ((cross1 >= 0.0f) == (cross2 >= 0.0f) && (cross2 >= 0.0f) == (cross3 >= 0.0f))
				{
					finalColor = ColorRGB(
						vertices_projected[triangleIndex].color.r * cross2 + vertices_projected[triangleIndex + 1].color.r * cross3 + vertices_projected[triangleIndex + 2].color.r * cross1,
						vertices_projected[triangleIndex].color.g * cross2 + vertices_projected[triangleIndex + 1].color.g * cross3 + vertices_projected[triangleIndex + 2].color.g * cross1,
						vertices_projected[triangleIndex].color.b * cross2 + vertices_projected[triangleIndex + 1].color.b * cross3 + vertices_projected[triangleIndex + 2].color.b * cross1
					);
				}

				finalColor.MaxToOne();

				m_pBackBufferPixels[px + (py * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
					static_cast<uint8_t>(finalColor.r * 255),
					static_cast<uint8_t>(finalColor.g * 255),
					static_cast<uint8_t>(finalColor.b * 255));
			}
		}
	}
}
void Renderer::Render_W1_Part2()
{
	std::vector<Vertex> vertices_world
	{
		//Triagle 0
		{{0.f, 2.0f, 2.f}},
		{{1.5f, -1.f, 0.f}},
		{{-1.5f, -1.f, 0.f}},

	};

	std::vector<Vertex> vertices_projected;
	VertexTransformationFunction(vertices_world, vertices_projected);

	for (size_t triangleIndex = 0; triangleIndex < vertices_projected.size(); triangleIndex += 3)
	{
		Vector2 v0(((vertices_projected[triangleIndex].position.x + 1.f) / 2) * m_Width, ((1.0f - vertices_projected[triangleIndex].position.y) / 2) * m_Height);
		Vector2 v1(((vertices_projected[triangleIndex + 1].position.x + 1.f) / 2) * m_Width, ((1.0f - vertices_projected[triangleIndex + 1].position.y) / 2) * m_Height);
		Vector2 v2(((vertices_projected[triangleIndex + 2].position.x + 1.f) / 2) * m_Width, ((1.0f - vertices_projected[triangleIndex + 2].position.y) / 2) * m_Height);

		for (int px{}; px < m_Width; ++px)
		{
			for (int py{}; py < m_Height; ++py)
			{
				Vector2 point(px + 0.5f, py + 0.5f);

				float gradient = px / static_cast<float>(m_Width);
				gradient += py / static_cast<float>(m_Width);
				gradient /= 2.0f;

				ColorRGB finalColor{ 0, 0, 0 };

				float cross1 = Vector2::Cross(v1 - v0, point - v0);
				float cross2 = Vector2::Cross(v2 - v1, point - v1);
				float cross3 = Vector2::Cross(v0 - v2, point - v2);
				
				if ((cross1 >= 0.0f) == (cross2 >= 0.0f) && (cross2 >= 0.0f) == (cross3 >= 0.0f))
				{
					finalColor = ColorRGB(1, 1, 1);
						
				}

				finalColor.MaxToOne();

				m_pBackBufferPixels[px + (py * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
					static_cast<uint8_t>(finalColor.r * 255),
					static_cast<uint8_t>(finalColor.g * 255),
					static_cast<uint8_t>(finalColor.b * 255));
			}
		}
	}
}
void Renderer::Render_W1_Part1() 
{
	std::vector<Vector3> vertices_ndc
	{
		{0.f, 0.5f, 1.f},
		{0.5f, -0.5f, 1.f},
		{-0.5f, -0.5f, 1.f}
	};

	Vector2 v0(((vertices_ndc[0].x + 1.f) / 2) * m_Width, ((1.0f - vertices_ndc[0].y) / 2) * m_Height);
	Vector2 v1(((vertices_ndc[1].x + 1.f) / 2) * m_Width, ((1.0f - vertices_ndc[1].y) / 2) * m_Height);
	Vector2 v2(((vertices_ndc[2].x + 1.f) / 2) * m_Width, ((1.0f - vertices_ndc[2].y) / 2) * m_Height);

	//RENDER LOGIC
	for (int px{}; px < m_Width; ++px)
	{
		for (int py{}; py < m_Height; ++py)
		{
			Vector2 point(px + 0.5f, py + 0.5f);	
			

			//ColorRGB finalColor{ gradient, gradient, gradient };
			ColorRGB finalColor{ 0, 0, 0 };


			float cross1 = Vector2::Cross(v1 - v0, point - v0);
			float cross2 = Vector2::Cross(v2 - v1, point - v1);
			float cross3 = Vector2::Cross(v0 - v2, point - v2);

			if ((cross1 >= 0.0f) == (cross2 >= 0.0f) && (cross2 >= 0.0f) == (cross3 >= 0.0f))
			{
				finalColor = { 1, 1, 1 };
			}


			//Update Color in Buffer
			finalColor.MaxToOne();

			m_pBackBufferPixels[px + (py * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
				static_cast<uint8_t>(finalColor.r * 255),
				static_cast<uint8_t>(finalColor.g * 255),
				static_cast<uint8_t>(finalColor.b * 255));

		}
	}
}


bool Renderer::SaveBufferToImage() const
{
	return SDL_SaveBMP(m_pBackBuffer, "Rasterizer_ColorBuffer.bmp");
}
