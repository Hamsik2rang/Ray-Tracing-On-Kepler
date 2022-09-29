#include "ResearchLayer.h"

#include "stbimage\stb_image.h"

void ResearchLayer::OnAttach()
{
	unsigned char* tempBuffer = nullptr;
	int width, height, channel;
	tempBuffer = stbi_load("./Assets/image_1.JPG", &width, &height, &channel, 0);
	if (!tempBuffer)
	{
		KEPLER_ASSERT(false, "");
	}

	m_pBuffer = new uint8_t[width * height * 4]{ 0 };
	m_pTexture = kepler::ITexture2D::Create(kepler::ETextureDataType::UNorm_RGBA8, 1280, 720, 4);
	for (int i = 0; i < 720; i++)
	{
		for (int j = 0, k = 0; j < width * channel; j += channel, k += 4)
		{
			m_pBuffer[width * 4 * i + k] = tempBuffer[width * channel * i + j];
			m_pBuffer[width * 4 * i + k + 1] = tempBuffer[width * channel * i + j + 1];
			m_pBuffer[width * 4 * i + k + 2] = tempBuffer[width * channel * i + j + 2];
			m_pBuffer[width * 4 * i + k + 3] = 255;
		}
	}
	m_pTexture->SetData(m_pBuffer, 1280, 720);
	//m_pTexture = kepler::ITexture2D::Create(kepler::ETextureDataType::UNorm_RGBA8, "./Assets/image_1.JPG");


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
	kepler::Renderer2D::Get()->BeginScene(m_camera);

	kepler::Renderer2D::Get()->DrawNonBatchedQuad({ 0, 0 }, 0.0f, { 1280, 720}, m_pTexture);

	kepler::Renderer2D::Get()->EndScene();
}

void ResearchLayer::OnRender()
{

}

void ResearchLayer::OnEvent(kepler::Event& e)
{

}
