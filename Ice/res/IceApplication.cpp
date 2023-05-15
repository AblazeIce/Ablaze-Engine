#include "Ablaze.h"
class ExampleLayer :public Ablaze::Layer
{
public:
	ExampleLayer()
		:Layer("Example") {}
	void OnUpdate() override
	{
		ABLAZE_INFO("ExampleLayer:Update");
	}
	void OnEvent(Ablaze::Event& event) override
	{
		ABLAZE_TRACE("ExampleLayer:{0}", event);
	}
};
class Ice :public Ablaze::Application
{
public:
	Ice() {
		PushLayer(new ExampleLayer());
	}
	~Ice() {}
};
Ablaze::Application* Ablaze::CreateApplication()
{
	return new Ice();
}