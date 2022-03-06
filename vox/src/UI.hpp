#pragma once

#include "glm/glm.hpp"
#include <functional>
#include <string>
#include <map>
#include <memory>
#include "VertexBufferLayout.hpp"
#include "Shader.hpp"

struct UIVertex
{
	glm::vec2 position;
	glm::vec2 texCoord; // Textures not supported yet
	int ID;
	UIVertex()
		: position(0.0f), texCoord(0.0f), ID(-1)
	{}
	UIVertex(float posX, float posY, float texX, float texY, int id)
		: position(posX, posY), texCoord(texX, texY), ID(id)
	{}
};

struct UIElementData
{
	float x;
	float y;
	float width;
	float height;
	int32_t id;
};

class UIElement
{
private:
	static int32_t _lastID;
	int32_t	_id;
	float _x;
	float _y;
	float _width;
	float _height;
	std::function<void(float value)> _callbackFunction;
protected:
	UIElement(float x, float y, float width, float height);
public:
	UIElement() = delete;
	UIElement(UIElement& other) = delete;
	void operator=(const UIElement& other) = delete;
	virtual ~UIElement() = default;
	void setCallback(const std::function<void(float value)>& callbackFunc);
	void call(float x, float y);
	inline uint32_t getID() const { return _id; }
	UIElementData getData() const;
};

class Button : public UIElement
{
public:
	Button(float x, float y, float width, float height, const std::function<void(float value)>& callbackFunc);
	~Button() = default;
	Button() = delete;
	Button(Button& other) = delete;
	void operator=(const Button& other) = delete;
};

class Slider : public UIElement
{
public:
	Slider(float x, float y, float width, float height, const std::function<void(float value)>& callbackFunc);
	~Slider() = default;
	Slider() = delete;
	Slider(Slider& other) = delete;
	void operator=(const Slider& other) = delete;
};

class UI
{
private:
	typedef std::shared_ptr<UIElement> ElementPtr;
	static UI* _ui;
	std::multimap<std::string, ElementPtr>	_sets;
	VertexBufferLayout	_vbLayout;
	Shader _shader;

	UI();
	~UI() = default;
public:
	static void addElement(const ElementPtr& element, const std::string& setName);
	static void init();
	static void terminate();
	static void draw(const std::string& setName);
	static void call(int32_t id, int x, int y);
};
