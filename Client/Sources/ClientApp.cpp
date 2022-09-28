#include "ResearchLayer.h"

#include "Core/EntryPoint.h"


class Client : public kepler::Application
{
public:
	Client()
		: Application(kepler::EGraphicsAPI::DirectX11)
	{
		PushLayer(new ResearchLayer);
	}
	~Client() = default;
};

kepler::Application* kepler::CreateApplication()
{
	return new Client();
}