#pragma once

struct FrameRateCounter
{
	float duration;
	float sampleDuration = 0.1f; // sec
	int frame;
	float FPS;

	void OnUpdate(float ts)
	{
		duration += ts;
		frame++;
		if (duration > sampleDuration)
		{
			FPS = frame / duration;
			duration = 0.f;
			frame = 0;
		}
	}
};