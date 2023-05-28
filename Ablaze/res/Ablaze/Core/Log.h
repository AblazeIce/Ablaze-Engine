#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Ablaze {
	class ABLAZE_API Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return _CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return _ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> _CoreLogger;
		static std::shared_ptr<spdlog::logger> _ClientLogger;
	};
}
#define ABLAZE_CORE_TRACE(...) ::Ablaze::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ABLAZE_CORE_INFO(...)  ::Ablaze::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ABLAZE_CORE_WARN(...)  ::Ablaze::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ABLAZE_CORE_ERROR(...) ::Ablaze::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ABLAZE_CORE_FATAL(...) ::Ablaze::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define ABLAZE_TRACE(...)      ::Ablaze::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ABLAZE_INFO(...)       ::Ablaze::Log::GetClientLogger()->info(__VA_ARGS__)
#define ABLAZE_WARN(...)       ::Ablaze::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ABLAZE_ERROR(...)      ::Ablaze::Log::GetClientLogger()->error(__VA_ARGS__)
#define ABLAZE_FATAL(...)      ::Ablaze::Log::GetClientLogger()->fatal(__VA_ARGS__)