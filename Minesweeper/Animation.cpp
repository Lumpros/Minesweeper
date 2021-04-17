#include "Animation.h"

void Game::Animation::AddFrame(Game::Frame frame)
{
	frames.push_back({ frame.clip, frame.duration_milliseconds });
}

SDL_Rect* Game::Animation::GetCurrentFrame(void)
{
	return &frames[current_index].clip;
}

Game::Animation::Animation(void)
{
	time_since_last_frame_change_milliseconds = clock();
}

void Game::Animation::Update(bool shouldPlayAnimationOnce)
{
	if (!IsRunning())
		return;

	if (CurrentFrameIsFinished()) 
	{
		++current_index;
		if (IsOnLastFrame()) {
			Reset();
			if (shouldPlayAnimationOnce)
				Pause(true);
		}
		time_since_last_frame_change_milliseconds = clock();
	}
}

bool Game::Animation::IsRunning(void) const
{
	return animation_is_running;
}

bool Game::Animation::CurrentFrameIsFinished(void)
{
	return clock() - time_since_last_frame_change_milliseconds >= frames[current_index].duration_milliseconds;
}

bool Game::Animation::IsOnLastFrame(void)
{
	return current_index == frames.size();
}

void Game::Animation::Reset(void)
{
	current_index = 0;
}

void Game::Animation::Pause(bool Pause)
{
	animation_is_running = !Pause;
}