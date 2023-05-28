#pragma once
#ifdef ABLAZE_PLATFORM_WINDOWS
extern Ablaze::Application* Ablaze::CreateApplication();
int main(int argc, char** argv)
{
	Ablaze::Log::Init();
	ABLAZE_CORE_INFO("��������Ablaze��Ϸ����");
	ABLAZE_CORE_TRACE("��������Ablaze��Ϸ����");
	ABLAZE_CORE_ERROR("��������Ablaze��Ϸ����");
	ABLAZE_CORE_WARN("��������Ablaze��Ϸ����");
	ABLAZE_TRACE("��������Ablaze��Ϸ����");
	auto app = Ablaze::CreateApplication();
	app->Run();
	delete app;
}
#endif // ABLAZE_PLATFORM_WINDOWS
