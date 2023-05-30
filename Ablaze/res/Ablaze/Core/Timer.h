#pragma once
#include <chrono>

namespace Ablaze {
	class Timer {
	public:
		Timer() {
			Reset();
		}
		inline void Reset() {
			//获得高精度时间点的当前时间
			m_Start = std::chrono::high_resolution_clock::now();
		}
		//------计算消逝的时间段------
		float Elapsed() {
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f;
		}
		//------ms-------
		float ElapsedMillis() {
			return Elapsed() * 1000.0f;
		}
	private:
		//---设置一个高精度时间点--------
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};
}
