#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

const glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

struct CameraRotation
{
	float X, Y;
};

struct AngleConstraint
{
	float MaxAngle, MinAngle;

	float Clamp(float Angle)
	{
		if (Angle > MaxAngle) { return MaxAngle; }
		if (Angle < MinAngle) { return MinAngle; }
		return Angle;
	}
};

struct Camera
{
	CameraRotation m_Rotation{ 0.0f,0.0f };
	AngleConstraint m_YConstraint{ 70.0f,-70.0f };


	glm::mat4 m_viewMartix = glm::mat4(1.0f);
	glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, 0.0f);


	void Update()
	{
		m_Front.x = cos(glm::radians(m_Rotation.X)) * cos(glm::radians(-m_Rotation.Y));
		m_Front.y = sin(glm::radians(-m_Rotation.Y));
		m_Front.z = sin(glm::radians(m_Rotation.X)) * cos(glm::radians(-m_Rotation.Y));

		m_Front = glm::normalize(m_Front);

		glm::vec3 cameraTarget = m_Position + m_Front;
		m_viewMartix = glm::lookAt(m_Position, cameraTarget, upVector);
	};

	void AddRotation(float DeltaX, float DeltaY)
	{
		m_Rotation.X += DeltaX;
		m_Rotation.Y = m_YConstraint.Clamp(m_Rotation.Y + DeltaY);
	}

	void SetPosition(glm::vec3 NewPosition)
	{
		m_Position = NewPosition;
	}

	inline CameraRotation GetRotation() const { return m_Rotation; }
	inline glm::mat4 GetViewMatrix() const { return  m_viewMartix; }
	inline glm::vec3 GetPosition() const { return m_Position; }
	inline glm::vec3 GetDirection() const { return m_Front; }

};