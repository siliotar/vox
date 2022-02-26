#include "Camera.hpp"

Camera* Camera::_camera = nullptr;

Camera::Camera()
	: _position(0.5f, 5.0f, 0.5f), _fov(glm::radians(90.0f)), _rotation(1.0f)
{
	_updateVectors();
}

Camera::~Camera()
{}

void	Camera::_updateVectors()
{
	_front = glm::vec3(_rotation * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	_right = glm::vec3(_rotation * glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f));
	_up = glm::vec3(_rotation * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
}

void	Camera::init()
{
	if (_camera != nullptr)
		return;

	_camera = new Camera();
}

void	Camera::shutdown()
{
	if (_camera == nullptr)
		return;

	delete _camera;
	_camera = nullptr;
}

glm::mat4	Camera::getProjection()
{
	return glm::perspective(_camera->_fov, (float)Window::width / (float)Window::height, 0.1f, 100.0f);
}

glm::mat4	Camera::getView()
{
	return glm::lookAt(_camera->_position, _camera->_position + _camera->_front, _camera->_up);
}

void		Camera::rotate(float x, float y, float z)
{
	_camera->_rotation = glm::rotate(_camera->_rotation, z, glm::vec3(0.0f, 0.0f, 1.0f));
	_camera->_rotation = glm::rotate(_camera->_rotation, y, glm::vec3(0.0f, 1.0f, 0.0f));
	_camera->_rotation = glm::rotate(_camera->_rotation, x, glm::vec3(1.0f, 0.0f, 0.0f));

	_camera->_updateVectors();
}

void		Camera::moveFront(float value)
{
	glm::vec3	worldFront(_camera->_front.x, 0.0f, _camera->_front.z);
	worldFront = glm::normalize(worldFront);
	_camera->_position += worldFront * value;
}

void		Camera::moveRight(float value)
{
	_camera->_position += _camera->_right * value;
}

void		Camera::moveUp(float value)
{
	_camera->_position += glm::vec3(0.0f, 1.0f, 0.0f) * value;
}

void		Camera::resetRotation()
{
	_camera->_rotation = glm::mat4(1.0f);
}
