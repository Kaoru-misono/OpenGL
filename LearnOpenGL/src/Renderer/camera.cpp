#include "camera.h"

Perspective_Camera::Perspective_Camera(glm::vec3 position, glm::vec3 euler, glm::vec3 up, float fov)
	:m_position(position), m_world_up(up), m_fov(fov), m_euler(euler), m_camera_speed(2.5f), m_mouse_sensitivity(0.01f)
{
	m_zoom = m_fov;

	update_camera_space_vector();
	update_view_matrix();
}

void Perspective_Camera::process_key_event(camera_movement direction, float delta_time)
{
	float speed = m_camera_speed * delta_time;
	if (direction == FORWARD)
		m_position += m_front * speed;
	if (direction == BACKWARD)
		m_position -= m_front * speed;
	if (direction == LEFT)
		m_position -= m_right * speed;
	if (direction == RIGHT)
		m_position += m_right * speed;

	update_view_matrix();
}

void Perspective_Camera::process_mouse_event(float x_offset, float y_offset, GLboolean constrain_pitch)
{
	x_offset *= m_mouse_sensitivity;
	y_offset *= m_mouse_sensitivity;

	m_euler.y += x_offset;
	m_euler.x += y_offset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrain_pitch)
	{
		if (m_euler.x > 89.0f)
			m_euler.x = 89.0f;
		if (m_euler.x < -89.0f)
			m_euler.x = -89.0f;
	}

	update_camera_space_vector();
	update_view_matrix();
}

void Perspective_Camera::process_scroll_event(float y_offset)
{
	m_zoom -= (float)y_offset;
	if (m_zoom < 1.0f)
		m_zoom = 1.0f;
	if (m_zoom > 45.0f)
		m_zoom = 45.0f;

	m_fov = m_zoom;

	update_view_matrix();
}

void Perspective_Camera::update_view_matrix()
{
	m_view_matrix = glm::lookAt(m_position, m_position + m_front, m_up);
	glm::mat4 projection_matrix = glm::perspective(glm::radians(m_fov), 800.0f / 600.0f, 0.1f, 100.0f);
	m_view_projection_matrix = projection_matrix * m_view_matrix;

}

void Perspective_Camera::update_camera_space_vector()
{

	glm::vec3 front;
	front.x = cos(glm::radians(m_euler.x)) * cos(glm::radians(m_euler.y));
	front.y = sin(glm::radians(m_euler.x));
	front.z = cos(glm::radians(m_euler.x)) * sin(glm::radians(m_euler.y));
	m_front = glm::normalize(front);
	m_right = glm::normalize(glm::cross(m_front, m_world_up));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}
