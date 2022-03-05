#include "UI.hpp"

void UI::UIElement::setCallback(const std::function<void(float x, float y)>& callbackFunc)
{
	_callbackFunction = callbackFunc;
}

void UI::UIElement::call(float x, float y)
{
	if (_callbackFunction)
		_callbackFunction(x, y);
}
