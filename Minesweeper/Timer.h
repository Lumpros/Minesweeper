#pragma once

#include <ctime>

class Timer
{
private:
	clock_t millisecondsSinceLastCall = 0;
	clock_t lastClock = 0;
	unsigned int total_seconds_passed = 0;

	bool is_ongoing = false;

public:
	void Start(void);
	void Stop(void);
	void Update(void);
	void Reset(void);

	int GetTotalSecondsPassed(void) const;
};

