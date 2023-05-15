#pragma once
#ifdef ABLAZE_PLATFORM_WINDOWS
extern Ablaze::Application* Ablaze::CreateApplication();
int main(int argc, char** argv)
{
	Ablaze::Log::Init();
	ABLAZE_CORE_INFO("欢饮来到Ablaze游戏引擎");
	ABLAZE_CORE_TRACE("欢饮来到Ablaze游戏引擎");
	ABLAZE_CORE_ERROR("欢饮来到Ablaze游戏引擎");
	ABLAZE_CORE_WARN("欢饮来到Ablaze游戏引擎");
	ABLAZE_TRACE("欢饮来到Ablaze游戏引擎");
	auto app = Ablaze::CreateApplication();
	app->Run();
	delete app;
}
#endif // ABLAZE_PLATFORM_WINDOWS
