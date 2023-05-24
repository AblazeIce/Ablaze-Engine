#pragma once
namespace Ablaze {
	class Timestep {
	public:
		Timestep(float time = 0.0f)
			:m_Time(time){}
		//重写float,使得该类可以直接赋值给float变量;或者直接当做float变量使用
		operator float() const { return m_Time; }

		inline float GetSeconds() const { return m_Time; }
		inline float GetMilliseconds() const { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};

}