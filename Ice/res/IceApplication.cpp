#include "Ablaze.h"

#include "imgui.h"
class ExampleLayer :public Ablaze::Layer
{
public:
	ExampleLayer()
		:Layer("Example") {}
	void OnUpdate() override
	{
		//ABLAZE_INFO("ExampleLayer:Update");
	}
	void OnEvent(Ablaze::Event& event) override
	{
		//ABLAZE_TRACE("ExampleLayer:{0}", event);
	}
	void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
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