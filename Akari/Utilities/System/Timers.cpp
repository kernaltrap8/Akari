
#include "Timers.h"
#include "../Math.h"
#include <sys/sys_time.h>
#include <sys/timer.h>

Timers::ChangeOvertime g_ChangeOvertime;

namespace Timers
{
	unsigned int GetTimeNow()
	{
		return sys_time_get_system_time() / 1000;
	}

	void Sleep(unsigned int ms)
	{
		sys_timer_usleep(ms * 1000);
	}

	ValueTimer::ValueTimer(float* from, float to, unsigned int duration, unsigned int startDelay, int interpolationType)
		: m_From(from), m_To(to), m_Duration(duration), m_InterpolationType(interpolationType)
	{
		m_StartValue = *from;
		m_StartTime = GetTimeNow() + startDelay;
	}

	bool ValueTimer::IsFinished()
	{
		return m_From == nullptr;
	}

	bool ValueTimer::IsSame(float* from)
	{
		return m_From == from;
	}

	void ValueTimer::Update()
	{
		if (!m_From)
			return;

		uint64_t timeNow = GetTimeNow();
		if (timeNow < m_StartTime)
			return;

		if (timeNow >= m_StartTime + m_Duration)
		{
			*m_From = m_To;
			m_From = nullptr;
			return;
		}

		uint64_t timePassed = timeNow - m_StartTime;
		float timeFactor = static_cast<float>(timePassed) / static_cast<float>(m_Duration);

		switch (m_InterpolationType)
		{
		case 0://linear mah boi
			*m_From = m_StartValue + (m_To - m_StartValue) * timeFactor;
			break;
		default:
			auto easingFunction = Math::getEasingFunction((Math::easing_functions)(m_InterpolationType - 1));
			if (easingFunction)
				*m_From = m_StartValue + (m_To - m_StartValue) * easingFunction(timeFactor);
			else
				*m_From = m_StartValue + (m_To - m_StartValue) * timeFactor;
			break;
		}
	}


	bool ChangeOvertime::Add(float* from, float to, unsigned int duration, unsigned int startDelay, bool ignoreCheck, int interpolationType)
	{
		if (!ignoreCheck && AlreadyExist(from))
			return false;

		for (int i = 0; i < MAX_VALUE_TIMERS; i++)
		{
			if (m_Timers[i].IsFinished())
			{
				m_Timers[i] = ValueTimer(from, to, duration, startDelay, interpolationType);
				return true;
			}
		}
		return false;
	}

	bool ChangeOvertime::Add(vec2* from, vec2 to, unsigned int duration, unsigned int startDelay, bool ignoreCheck, int interpolationType)
	{
		for (int i = 0; i < 2; i++)
			if (!Add(&from->v[i], to.v[i], duration, startDelay, ignoreCheck, interpolationType))
				return false;
		return true;
	}

	bool ChangeOvertime::Add(vec3* from, vec3 to, unsigned int duration, unsigned int startDelay, bool ignoreCheck, int interpolationType)
	{
		for (int i = 0; i < 3; i++)
			if (!Add(&from->v[i], to.v[i], duration, startDelay, ignoreCheck, interpolationType))
				return false;
		return true;
	}

	bool ChangeOvertime::Add(vec4* from, vec4 to, unsigned int duration, unsigned int startDelay, bool ignoreCheck, int interpolationType)
	{
		for (int i = 0; i < 4; i++)
			if (!Add(&from->v[i], to.v[i], duration, startDelay, ignoreCheck, interpolationType))
				return false;
		return true;
	}

	void ChangeOvertime::Update()
	{
		for (int i = 0; i < MAX_VALUE_TIMERS; i++)
			m_Timers[i].Update();
	}

	bool ChangeOvertime::AlreadyExist(float* from)
	{
		for (int i = 0; i < MAX_VALUE_TIMERS; i++)
		{
			if (m_Timers[i].IsSame(from))
				return true;
		}

		return false;
	}
}