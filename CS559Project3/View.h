#pragma once
#include <vector>
#include "Camera.h"
#include "Projection.h"
#include "Drawable.h"
#include "Graphics.h"

class ViewOverlay;

/**
 * The view class encapsulates the responsibility of using a Camera
 * to render a Model.
 */
class View {
private:
	View(); //force use of constructor with arguments
	void setupCamera();
protected:
	ViewOverlay *overlay;
	Projection *proj;
	Camera *camera;
	Drawable *model;
public:
	View(Projection *p, Camera *camera, Drawable *model, ViewOverlay *o);
	
	virtual void render();
	
	inline Camera *getCamera() {
		return camera;
	}
	inline Drawable *getModel() {
		return model;
	}
	inline void setOverlay(ViewOverlay *o) {
		this->overlay = o;
	}
	inline void setCamera(Camera *c) {
		this->camera = c;
	}
	inline void setModel(Drawable *m) {
		this->model = m;
	}
};

/** An abstract representation of a 2D overlay to be drawn on a view */
class ViewOverlay {
public:
	ViewOverlay();
	/* renders the overlay. Returns the rendering context for the overlay. */
	virtual glm::mat4 draw();
protected:
	/* sets up an orthographic projection matrix with the bottom left corner
	 * at (0, 0) and the top right at (size.x, size.y), the near plane at
	 * 0 and the far plane at 1. 
	 * Sets the view to the base rendering context at (0, 0) */
	virtual void setupCamera() const;
	
};

/**
 * HudOverlay is an overlay that draws text infront of 
 * everything else. It holds and array of strings that
 * each represents a line in the HUD.
 */
class HudOverlay : public ViewOverlay{
public:
	virtual glm::mat4 draw();

	/*
	 * Creates a HudOverlay with a vector of strings.
	 */
	HudOverlay(std::vector<char*> text);

protected:
	//The vector of shrings of the HudOverlay.
	std::vector<char*> text;
};
