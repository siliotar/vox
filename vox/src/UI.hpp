#pragma once

#include "glm/glm.hpp"
#include <functional>

namespace UI
{
	struct UIVertex
	{
		glm::vec2 position;
		glm::vec2 texCoord;
		int ID;
	};

	class UIElement
	{
	private:
		int	_id;
		float _x;
		float _y;
		std::function<void(float x, float y)> _callbackFunction;
	public:
		UIElement() = default;
		virtual ~UIElement() = default;
		void setCallback(const std::function<void(float x, float y)>& callbackFunc);
		void call(float x, float y);
		inline int getID() const { return _id; }
	};

	class Button : public UIElement
	{

	};

	class Slider : public UIElement
	{

	};
}
