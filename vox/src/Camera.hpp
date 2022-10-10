#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Window.hpp"

class Camera
{
private:
	static Camera* _camera;
	glm::vec3	_position;
	glm::vec3	_front;
	glm::vec3	_right;
	glm::vec3	_up;
	glm::mat4	_rotation;

	Camera();
	~Camera();

	void	_updateVectors();
public:
	Camera(Camera& other) = delete;
	void operator=(const Camera& other) = delete;

	static void	init();
	static void	shutdown();

	static glm::mat4	getProjection();
	static glm::mat4	getView();

	static void			rotate(float x, float y, float z);
	static void			moveFront(float value);
	static void			moveRight(float value);
	static void			moveUp(float value);
	static void			resetRotation();

	static inline const glm::vec3&	getPlayerPosition() { return _camera->_position; }

	static float		fov;
	static float		aspect;
	static float		near;
	static float		far;

	static inline const glm::vec3& getFront() { return _camera->_front; }
	static inline const glm::vec3& getRight() { return _camera->_right; }
	static inline const glm::vec3& getUp() { return _camera->_up; }
};
