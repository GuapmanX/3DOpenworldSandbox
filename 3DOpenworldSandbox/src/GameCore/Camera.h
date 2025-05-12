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

struct CameraSettings
{
	float Sensitivity = 1.0f;
	float Speed = 1.0f;
};

namespace Camera
{
	void Update();
	void AddRotation(float DeltaX, float DeltaY);
	void SetPosition(glm::vec3 NewPosition);

	CameraRotation GetRotation();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjection();
	glm::vec3 GetPosition();
	glm::vec3 GetDirection();
	float GetCameraSpeed();
}