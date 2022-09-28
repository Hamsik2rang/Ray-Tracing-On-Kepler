#pragma once

#include "Kepler.h"

class ResearchLayer : public kepler::Layer
{
private:
	uint8_t* m_pBuffer;

public:
	ResearchLayer()
		: Layer("Research")
	{}

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnEvent(kepler::Event& e) override;
};