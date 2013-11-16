#pragma once

/** a base class for an animation.
 * it encapsulates the pause functionality. */
class Animation {
protected:
	bool paused;
	bool willPause;
	int timeSpentPaused;
	int timePauseStarted;

	/* a virtual update function
	 * for subclasses to override */
	virtual void doUpdate(int time) = 0;
public:
	/** starts the animation */
	Animation();

	/** alters the time based on pause functionality
	 * then calls doUpdate() */
	void update(int time);

	/** pauses the animation */
	void pause();
	/** continues the animation */
	void play();
	/** resets the animation */
	virtual void reset();

	virtual ~Animation() {}

	inline bool isPaused() { return paused; }
};
