#include "UI.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Window.hpp"
#include <vector>

int32_t UIElement::_lastID = 0;
UI* UI::_ui = nullptr;

UIElement::UIElement(float x, float y, float width, float height)
	: _id(_lastID++), _x(x), _y(y), _width(width), _height(height)
{}

void UIElement::setCallback(const std::function<void(float value)>& callbackFunc)
{
	_callbackFunction = callbackFunc;
}

void UIElement::call(float x, float y)
{
	float hit = (x - _x) / _width;
	if (_callbackFunction)
		_callbackFunction(hit);
}

UIElementData UIElement::getData() const
{
	return { _x, _y, _width, _height, _id };
}

Button::Button(float x, float y, float width, float height, const std::function<void(float value)>& callbackFunc)
	: UIElement(x, y, width, height)
{
	setCallback(callbackFunc);
}

Slider::Slider(float x, float y, float width, float height, const std::function<void(float value)>& callbackFunc)
	: UIElement(x, y, width, height)
{
	setCallback(callbackFunc);
}

UI::UI()
	: _shader("res/shaders/UI.vert", "res/shaders/UI.frag")
{
	_vbLayout.push<float>(2);
	_vbLayout.push<float>(2);
	_vbLayout.push<int>(1);
}

void UI::init()
{
	if (!_ui)
		_ui = new UI;
}

void UI::terminate()
{
	if (!_ui)
		return;
	delete _ui;
	_ui = nullptr;
}

void UI::addElement(const ElementPtr& element, const std::string& setName)
{
	_ui->_sets.emplace(setName, element);
}

void UI::draw(const std::string& setName)
{
	if (_ui->_sets.find(setName) == _ui->_sets.end())
		return; // Error
	GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, buffers);
	auto set = _ui->_sets.equal_range(setName);
	std::vector<UIVertex>	vertexData;
	std::vector<uint32_t>	indexData;
	for (auto i = set.first; i != set.second; ++i)
	{
		indexData.push_back(vertexData.size());
		indexData.push_back(vertexData.size() + 1);
		indexData.push_back(vertexData.size() + 2);
		indexData.push_back(vertexData.size() + 2);
		indexData.push_back(vertexData.size() + 3);
		indexData.push_back(vertexData.size());

		UIElementData uiData = i->second->getData();
		vertexData.emplace_back(uiData.x,                uiData.y,                 0.0f, 0.0f, uiData.id);
		vertexData.emplace_back(uiData.x + uiData.width, uiData.y,                 1.0f, 0.0f, uiData.id);
		vertexData.emplace_back(uiData.x + uiData.width, uiData.y + uiData.height, 1.0f, 1.0f, uiData.id);
		vertexData.emplace_back(uiData.x,                uiData.y + uiData.height, 0.0f, 1.0f, uiData.id);
	}
	IndexBuffer ib(indexData.data(), indexData.size() * sizeof(uint32_t), GL_STATIC_DRAW);
	VertexBuffer vb(vertexData.data(), vertexData.size() * sizeof(UIVertex), GL_STATIC_DRAW);
	VertexArray va;
	va.addBuffer(vb, _ui->_vbLayout);
	ib.bind();
	vb.bind();
	va.bind();
	_ui->_shader.bind();
	glDisable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, indexData.size(), GL_UNSIGNED_INT, nullptr);
}

void UI::call(int32_t id, int x, int y)
{
	float floatX = float(x) / Window::width - (Window::width / 2);
	float floatY = float(y) / Window::height - (Window::height / 2);
	auto end = _ui->_sets.end();
	for (auto i = _ui->_sets.begin(); i != end; ++i)
	{
		if (i->second->getID() == id)
		{
			i->second->call(floatX, floatY);
			return;
		}
	}
}
