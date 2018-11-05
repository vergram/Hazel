#pragma once

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* CreateApplication();

int main(int argc, char** argv)
{
	Hazel::Log::Init();
	HZ_CORE_WARN("Hazel Initialized!");
	int a = 5;
	HZ_INFO("var a = {0}", a);
	::Hazel::Log::GetClientLogger()->error("test");

	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif // HZ_PLATFORM_WINDOWS
