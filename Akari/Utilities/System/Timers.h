
#pragma once

#include "../Math.h"

namespace Timers
{
	unsigned int GetTimeNow();
	void Sleep(unsigned int ms);

	constexpr int MAX_VALUE_TIMERS = 20;
	class ValueTimer
	{
	public:
		

	public:
		ValueTimer() {}
		ValueTimer(float* from, float to, unsigned int duration, unsigned int startDelay, int interpolationType);

		bool IsFinished();
		bool IsSame(float* from);
		void Update();

	private:
		float* m_From;
		float m_StartValue;
		float m_To;
		unsigned int m_StartTime;
		unsigned int m_Duration;
		int m_InterpolationType;
	};

	class ChangeOvertime
	{
	public:
		ChangeOvertime() {}
		bool Add(float* from, float to, unsigned int duration, unsigned int startDelay = 0, bool ignoreCheck = false, int interpolationType = 0);
		bool Add(vec2* from, vec2 to, unsigned int duration, unsigned int startDelay = 0, bool ignoreCheck = false, int interpolationType = 0);
		bool Add(vec3* from, vec3 to, unsigned int duration, unsigned int startDelay = 0, bool ignoreCheck = false, int interpolationType = 0);
		bool Add(vec4* from, vec4 to, unsigned int duration, unsigned int startDelay = 0, bool ignoreCheck = false, int interpolationType = 0);
		void Update();

	private:
		bool AlreadyExist(float* from);

	private:
		ValueTimer m_Timers[MAX_VALUE_TIMERS];
	};
}
extern Timers::ChangeOvertime g_ChangeOvertime;