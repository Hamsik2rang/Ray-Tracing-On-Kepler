#pragma once

#include "Kepler.h"
#include <memory>

class ResearchLayer : public kepler::Layer
{
private:
	uint8_t* m_pBuffer;
	std::shared_ptr<kepler::ITexture2D> m_pTexture;
	kepler::Scene m_scene;
	kepler::OrthographicCamera m_camera;

	kepler::Entity* m_pPlane;

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