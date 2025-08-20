#include "Camera.h"

CameraRotation m_Rotation{ 0.0f,0.0f };
AngleConstraint m_YConstraint{ 70.0f,-70.0f };
CameraSettings m_Settings{ 0.125f,20.0f };


glm::mat4 m_viewMartix = glm::mat4(1.0f);
glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, 0.0f);

const glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 99999999.0f);


namespace Camera
{
	void Update()
	{
		m_Front.x = cos(glm::radians(m_Rotation.X)) * cos(glm::radians(-m_Rotation.Y));
		m_Front.y = sin(glm::radians(-m_Rotation.Y));
		m_Front.z = sin(glm::radians(m_Rotation.X)) * cos(glm::radians(-m_Rotation.Y));

		m_Front = glm::normalize(m_Front);

		glm::vec3 cameraTarget = m_Position + m_Front;
		m_viewMartix = glm::lookAt(m_Position, cameraTarget, upVector);
	}

	void AddRotation(float DeltaX, float DeltaY)
	{
		m_Rotation.X += DeltaX * m_Settings.Sensitivity;
		m_Rotation.Y = m_YConstraint.Clamp(m_Rotation.Y + (DeltaY * m_Settings.Sensitivity));
	}

	void SetPosition(glm::vec3 NewPosition)
	{
		m_Position = NewPosition;
	}

	CameraRotation GetRotation() { return m_Rotation; }
	glm::mat4 GetViewMatrix() { return  m_viewMartix; }
	glm::mat4 GetProjection() { return  Projection; }
	glm::vec3 GetPosition() { return m_Position; }
	glm::vec3 GetDirection() { return m_Front; }
	float GetCameraSpeed() { return m_Settings.Speed; }
}