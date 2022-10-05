#pragma once

#include "Kepler.h"
#include <memory>

class ResearchLayer : public kepler::Layer
{
private:
	uint8_t* m_pBuffer;
	std::shared_ptr<kepler::ITexture2D> m_pTexture;
	kepler::OrthographicCamera m_camera;

	kepler::Entity* m_pPlane;

	void BoxBlur(int repeatTime);
	void GaussianBlur(float* blurBuffer1, int repeatTime);
	void Bloom(const float th, const int repeatTime, const float weight);

public:
	ResearchLayer()
		: Layer("Research")
		, m_pBuffer{ nullptr }
		, m_pTexture{ nullptr }
	{}

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnEvent(kepler::Event& e) override;
};