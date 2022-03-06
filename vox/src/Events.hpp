#pragma once

#include "Window.hpp"

typedef unsigned int uint;

struct Events
{
	static float _deltaX;
	static float _deltaY;
	static float _x;
	static float _y;
	static bool _cursor_locked;
	static bool _cursor_started;
	static uint32_t _frame;

	static void init();

	static void poll();

	static bool pressed(uint key);

	static bool repeat(uint key);

	static bool clicked(uint button);

	static bool clickRepeat(uint button);
};
