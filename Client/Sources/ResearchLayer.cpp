#include "ResearchLayer.h"

#include "stbimage\stb_image.h"

#include <algorithm>
#include <cstring>

static float Clamp(float f, float min, float max)
{
	if (f < min) return min;
	if (f > max) return max;
	return f;
}

void ResearchLayer::OnAttach()
{
	unsigned char* tempBuffer = nullptr;
	int width, height, channel;
	tempBuffer = stbi_load("./Assets/image_3.jpg", &width, &height, &channel, 0);
	if (!tempBuffer)
	{
		KEPLER_ASSERT(false, "");
	}

	m_pBuffer = new uint8_t[width * height * 4]{ 0 };
	m_pTexture = kepler::ITexture2D::Create(kepler::ETextureDataType::UNorm_RGBA8, 1280, 720, 4);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0, k = 0; j < width * channel; j += channel, k += 4)
		{
			m_pBuffer[width * 4 * i + k] = tempBuffer[width * channel * i + j];
			m_pBuffer[width * 4 * i + k + 1] = tempBuffer[width * channel * i + j + 1];
			m_pBuffer[width * 4 * i + k + 2] = tempBuffer[width * channel * i + j + 2];
			m_pBuffer[width * 4 * i + k + 3] = 255;
		}
	}

	//BoxBlur(10);
	//GaussianBlur(10);
	Bloom(0.3, 2, 0.6f);

	m_pTexture->SetData(m_pBuffer, 1280, 720);

	m_camera.SetProjection(0.0f, 1280.0f, 0.0f, 720.0f, 0.0f, 1.0f);
	m_camera.SetPosition({ 0.0f, 0.0f, 0.0f });
	m_camera.SetRotation(0.0f);
}

void ResearchLayer::OnDetach()
{
	if (m_pBuffer)
	{
		delete[] m_pBuffer;
		m_pBuffer = nullptr;
	}
}

void ResearchLayer::OnUpdate(float deltaTime)
{

}

void ResearchLayer::OnRender()
{
	kepler::Renderer2D::Get()->BeginScene(m_camera);

	kepler::Renderer2D::Get()->DrawNonBatchedQuad({ 0, 0 }, 0.0f, { 1280, 720 }, m_pTexture);
	//kepler::Renderer2D::Get()->DrawQuad({ 0, 0 }, 0.0f, { 1280, 720 }, m_pTexture);

	kepler::Renderer2D::Get()->EndScene();
}

void ResearchLayer::OnEvent(kepler::Event& e)
{

}

void ResearchLayer::BoxBlur(int repeatTime)
{
	int width = 1280;
	int height = 720;
	float* blurBuffer1 = new float[width * height * 4];
	float* blurBuffer2 = new float[width * height * 4];

	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width * 4; col++)
		{
			blurBuffer1[width * 4 * row + col] = (float)m_pBuffer[width * 4 * row + col] / 255.0f;
		}
	}

	while (repeatTime--)
	{
		for (int row = 0; row < height; row++)
		{
			for (int col = 0; col < width * 4; col += 4)
			{
				float r = 0.0f, g = 0.0f, b = 0.0f;
				for (int k = col - 8; k <= col + 8; k += 4)
				{
					int index = k;
					if (index < 0)
					{
						index = 0;
					}
					else if (index >= width * 4)
					{
						index = width * 4 - 4;
					}
					r += blurBuffer1[width * 4 * row + index];
					g += blurBuffer1[width * 4 * row + index + 1];
					b += blurBuffer1[width * 4 * row + index + 2];
				}
				r *= 0.2f;
				g *= 0.2f;
				b *= 0.2f;

				blurBuffer2[width * 4 * row + col] = r;
				blurBuffer2[width * 4 * row + col + 2] = b;
				blurBuffer2[width * 4 * row + col + 1] = g;
				blurBuffer2[width * 4 * row + col + 3] = 255.0f;
			}
		}

		std::swap(blurBuffer1, blurBuffer2);

		for (int col = 0; col < width * 4; col++)
		{
			if (col % 4 == 3)
			{
				continue;
			}

			for (int row = 0; row < height; row++)
			{
				float sum = 0.0f;
				for (int k = row - 2; k <= row + 2; k++)
				{
					int index = k;
					if (index < 0)
					{
						index = 0;
					}
					else if (index >= height)
					{
						index = height - 1;
					}
					sum += blurBuffer1[width * 4 * index + col];
				}
				sum *= 0.2f;

				blurBuffer2[width * 4 * row + col] = sum;
			}
		}

		std::swap(blurBuffer1, blurBuffer2);
	}

	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width * 4; col++)
		{
			m_pBuffer[width * 4 * row + col] = (uint8_t)(blurBuffer1[width * 4 * row + col] * 255.0f);
		}
	}


	delete[] blurBuffer1;
	delete[] blurBuffer2;
}


void ResearchLayer::GaussianBlur(float* blurBuffer1, int repeatTime)
{
	int width = 1280;
	int height = 720;
	const float weight[]{ 0.0545f, 0.2442f, 0.4026f, 0.2442f, 0.0545f };
	float* blurBuffer2 = new float[width * height * 4];

	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width * 4; col++)
		{
			blurBuffer1[width * 4 * row + col] = (float)m_pBuffer[width * 4 * row + col] / 255.0f;
		}
	}

	while (repeatTime--)
	{
		for (int row = 0; row < height; row++)
		{
			for (int col = 0; col < width * 4; col += 4)
			{
				float r = 0.0f, g = 0.0f, b = 0.0f;
				for (int k = col - 8, w = 0; k <= col + 8; k += 4, w++)
				{
					int index = k;
					if (index < 0)
					{
						index = 0;
					}
					else if (index >= width * 4)
					{
						index = width * 4 - 4;
					}
					r += blurBuffer1[width * 4 * row + index] * weight[w];
					g += blurBuffer1[width * 4 * row + index + 1] * weight[w];
					b += blurBuffer1[width * 4 * row + index + 2] * weight[w];
				}

				blurBuffer2[width * 4 * row + col] = r;
				blurBuffer2[width * 4 * row + col + 2] = b;
				blurBuffer2[width * 4 * row + col + 1] = g;
				blurBuffer2[width * 4 * row + col + 3] = 255.0f;
			}
		}

		std::swap(blurBuffer1, blurBuffer2);

		for (int col = 0; col < width * 4; col++)
		{
			if (col % 4 == 3)
			{
				continue;
			}

			for (int row = 0; row < height; row++)
			{
				float sum = 0.0f;
				for (int k = row - 2, w = 0; k <= row + 2; k++, w++)
				{
					int index = k;
					if (index < 0)
					{
						index = 0;
					}
					else if (index >= height)
					{
						index = height - 1;
					}
					sum += blurBuffer1[width * 4 * index + col] * weight[w];
				}

				blurBuffer2[width * 4 * row + col] = sum;
			}
		}

		std::swap(blurBuffer1, blurBuffer2);
	}

	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width * 4; col++)
		{
			m_pBuffer[width * 4 * row + col] = (uint8_t)(blurBuffer1[width * 4 * row + col] * 255.0f);
		}
	}


	delete[] blurBuffer2;
}

void ResearchLayer::Bloom(const float th, const int repeatTime, const float weight)
{
	// Relative Luminance Y = 0.2126r + 0.7152g + 0.0722b

	const int width = 1280;
	const int height = 720;

	float* blurBuffer = new float[width * height * 4];

	// 1. 밝지 않은 부분을 모두 검게 바꿉니다.
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width * 4; col += 4)
		{
			float r = (float)m_pBuffer[width * 4 * row + col] / 255.0f * 0.2126f;
			float g = (float)m_pBuffer[width * 4 * row + col + 1] / 255.0f * 0.7152;
			float b = (float)m_pBuffer[width * 4 * row + col + 2] / 255.0f * 0.0722f;

			auto sum = r + g + b;
			if (sum < th)
			{
				blurBuffer[width * 4 * row + col] = 0.0f;
				blurBuffer[width * 4 * row + col + 1] = 0.0f;
				blurBuffer[width * 4 * row + col + 2] = 0.0f;
			}
			else
			{
				blurBuffer[width * 4 * row + col] = (float)m_pBuffer[width * 4 * row + col] / 255.0f;
				blurBuffer[width * 4 * row + col + 1] = (float)m_pBuffer[width * 4 * row + col + 1] / 255.0f;
				blurBuffer[width * 4 * row + col + 2] = (float)m_pBuffer[width * 4 * row + col + 2] / 255.0f;
			}
			blurBuffer[width * 4 * row + col + 3] = 1.0f;
		}
	}


	// 2. 1.에서 완성된 이미지에 Gaussian Blur를 적용합니다.
	GaussianBlur(blurBuffer, repeatTime);

	// 3. 원본 이미지와 블러 처리한 이미지를 합칩니다.
	auto Clamp = [](uint16_t n, uint16_t min, uint16_t max)->uint16_t
	{
		if (n < min) return min;
		if (n > max) return max;
		return n;
	};
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width * 4; col++)
		{
			if (col % 4 == 3)
			{
				continue;
			}
			
			uint16_t curPixelBit = (uint16_t)m_pBuffer[width * 4 * row + col] + (uint16_t)(blurBuffer[width * 4 * row + col] * 255.0f * weight);
			m_pBuffer[width * 4 * row + col] = Clamp(curPixelBit, 0, 255);
		}
	}

	delete[] blurBuffer;
}