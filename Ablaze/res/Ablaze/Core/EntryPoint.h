#pragma once
#ifdef ABLAZE_PLATFORM_WINDOWS
extern Ablaze::Application* Ablaze::CreateApplication();
int main(int argc, char** argv)
{
	Ablaze::Log::Init();
	ABLAZE_CORE_INFO("欢饮来到Ablaze游戏引擎");
	ABLAZE_PROFILE_BEGIN_SESSION("Startup", "AblazeProfile-Start.json");
	auto app = Ablaze::CreateApplication();
	ABLAZE_PROFILE_END_SESSION();
	ABLAZE_PROFILE_BEGIN_SESSION("Runtime", "AblazeProfile-Runtime.json");
	app->Run();
	ABLAZE_PROFILE_END_SESSION();

	ABLAZE_PROFILE_BEGIN_SESSION("Startup", "AblazeProfile-Shutdown.json");
	delete app;
	ABLAZE_PROFILE_END_SESSION();

}
#endif // ABLAZE_PLATFORM_WINDOWS
