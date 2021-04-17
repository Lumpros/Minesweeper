#pragma once

#include <SDL.h>
#include <vector>
#include <ctime>

namespace Game
{
	struct Frame
	{
		SDL_Rect clip;
		int duration_milliseconds;
	};

	class Animation
	{
	private:
		std::vector<Frame> frames;
		bool animation_is_running = true;

		unsigned int current_index = 0;

		clock_t time_since_last_frame_change_milliseconds;

		bool CurrentFrameIsFinished(void);
		bool IsOnLastFrame(void);

	public:
		void AddFrame(Frame frame);
		SDL_Rect* GetCurrentFrame(void);

		void Reset(void);
		void Update(bool playOnce = false);
		void Pause(bool Pause);

		bool IsRunning(void) const;

		Animation(void);
	};
}