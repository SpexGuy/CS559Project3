README

Martin Wickham, Nik Ingrassia

We have built a ...thing.

The background is rendered in screen space.

Multiple ribbons spawn at the far end of the screen and are deleted once they go off of the back.
These ribbons are textured in eye space with simplex noise.

In addition, the screen is rendered on several billboards which always face the camera.
These billboards have several post-processing effects applied including:
	scan lines
	random noise
	solarizing

We have organized our code to make it easy to apply post-processing effects.
Simply list the shaders and draw operations when you create the frame,
and they will be applied when it draws.

