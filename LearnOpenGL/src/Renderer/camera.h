#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum camera_movement {
	FORWARD, BACKWARD, LEFT, RIGHT
};

class Perspective_Camera
{
public:
	Perspective_Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 euler = glm::vec3(0.0f, -90.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float fov = 45.0f);

	void process_key_event(camera_movement direction, float delta_time);
	void process_mouse_event(float x_offset, float y_offset, GLboolean constrainPitch = true);
	void process_scroll_event(float y_offset);
	
	void set_camera_speed(float speed) { m_camera_speed = speed; }
	void set_mouse_sensitivity(float sensitivity) { m_mouse_sensitivity = sensitivity; }
	void set_possition(glm::vec3 position) { m_position = position; }
	void set_euler(glm::vec3 euler) { m_euler = euler; }

	const glm::mat4 get_view_matrix() const { return m_view_matrix; }
	const glm::mat4 get_view_projection_matrix() const { return m_view_projection_matrix; }
	const glm::vec3 get_euler() const { return m_euler; }

private:
	void update_view_matrix();
	void update_camera_space_vector();
private:
	glm::vec3 m_position;
	glm::vec3 m_world_up;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_euler;

	float m_fov;

	float m_camera_speed;
	float m_mouse_sensitivity;
	float m_zoom;

	glm::mat4 m_view_matrix;
	glm::mat4 m_view_projection_matrix;
};