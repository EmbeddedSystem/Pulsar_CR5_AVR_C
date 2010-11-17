#include "processSpeaker.h"

u08 speakerBuffer_[10];

void InitFsmSpeaker(void)
{
	bufferInit(&speakerBuffer, speakerBuffer_, sizeof(speakerBuffer_));
}

void ProcessSpeaker(void)
{
	u08 static state = 0;
	u08 sound;
	if (GetMessage(MSG_BEEP_STOP))
	{
		state = 0;
		SPEAKER_ON = 0;
		return;
	}
	switch (state)
	{
	case 0:
		if (GetMessage(MSG_BEEP))
			state = 1;
		break;
	case 1:
		sound = bufferGetFromFront(&speakerBuffer);
		if (sound == LONG_BEEP)
		{
			state = 2;
			SPEAKER_ON = 1;
			StartTimer(TIMER_SPEAKER);
		}
		else if (sound == SHORT_BEEP)
		{
			state = 3;
			SPEAKER_ON = 1;
			StartTimer(TIMER_SPEAKER);
		}
		else if (sound == CLICK)
		{
			state = 5;
			SPEAKER_ON = 1;
			StartTimer(TIMER_SPEAKER);
		}
		else if (sound == 0)
		{
			SPEAKER_ON = 0;
			state = 0;
		}
		break;
	case 2:
		if (GetTimer(TIMER_SPEAKER) >= 500 * msec)
		{
			state = 4;
			StartTimer(TIMER_SPEAKER);
			SPEAKER_ON = 0;
		}
		break;
	case 3:
		if (GetTimer(TIMER_SPEAKER) >= 20 * msec)
		{
			state = 4;
			StartTimer(TIMER_SPEAKER);
			SPEAKER_ON = 0;
		}
		break;
	case 4:
		if (GetTimer(TIMER_SPEAKER) >= 100 * msec)
		{
			state = 1;
			PauseTimer(TIMER_SPEAKER);
		}
		break;
	case 5:
		if (GetTimer(TIMER_SPEAKER) >= 1 * msec)
		{
			state = 4;
			StartTimer(TIMER_SPEAKER);
			SPEAKER_ON = 0;
		}
		break;

	default:
		break;
	}
}
