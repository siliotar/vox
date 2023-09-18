#pragma once

#include "FrameBuffer.hpp"

class Vox
{
private:
	FrameBuffer* _fb;
public:
	Vox(int screenWidth, int screenHeight);
	~Vox();

	void Run();
};
