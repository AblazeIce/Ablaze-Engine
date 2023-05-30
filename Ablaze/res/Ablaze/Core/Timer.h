#pragma once
#include <chrono>

namespace Ablaze {
	class Timer {
	public:
		Timer() {
			Reset();
		}
		inline void Reset() {
			//��ø߾���ʱ���ĵ�ǰʱ��
			m_Start = std::chrono::high_resolution_clock::now();
		}
		//------�������ŵ�ʱ���------
		float Elapsed() {
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f;
		}
		//------ms-------
		float ElapsedMillis() {
			return Elapsed() * 1000.0f;
		}
	private:
		//---����һ���߾���ʱ���--------
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};
}
