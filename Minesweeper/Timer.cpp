#include "Timer.h"

void Timer::Start(void)
{
	is_ongoing = true;
	lastClock = clock();
}

void Timer::Stop(void)
{
	is_ongoing = false;
}

void Timer::Reset(void)
{
	millisecondsSinceLastCall = 0;
	total_seconds_passed = 0;
}

void Timer::Update(void)
{
	if (is_ongoing)
	{
		millisecondsSinceLastCall += clock() - lastClock;

		if (millisecondsSinceLastCall >= 1000)
		{
			millisecondsSinceLastCall -= 1000;
			++total_seconds_passed;
		}
	}

	lastClock = clock();
}

int Timer::GetTotalSecondsPassed(void) const
{
	return total_seconds_passed;
}

