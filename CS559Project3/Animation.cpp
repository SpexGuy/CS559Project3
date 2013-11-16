/* See Animation.h for architecture comments */
#include "Animation.h"

using namespace std;


//----------------- Animation -------------------

Animation::Animation() {
	paused = false;
	willPause = false;
	timeSpentPaused = 0;
	timePauseStarted = 0;
}

void Animation::update(int time) {
	/* detect and handle edge on pause flag */
	if (willPause != paused) {
		if (willPause)
			timePauseStarted = time;
		else
			timeSpentPaused += time - timePauseStarted;
		paused = willPause;
	}
	if (!paused)
		doUpdate(time - timeSpentPaused);
}

void Animation::pause() {
	willPause = true;
}

void Animation::play() {
	willPause = false;
}

void Animation::reset() {
	//trigger a positive edge
	paused = true;
	/* upon positive edge,
	 * timeSpentPaused = time + timeSpentPaused - timePauseStarted
	 * by setting timePauseStarted equal to timeSpentPaused,
	 * this becomes timeSpentPaused = time,
	 * effectively resetting the system. */
	timeSpentPaused = timePauseStarted;
}
