#pragma once

class Texture {
public:
	virtual bool initialize() = 0;
	virtual void bind() = 0;
	virtual void takeDown() = 0;
	virtual ~Texture() {}
};

