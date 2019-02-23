#include "Camera.h"

Camera::Camera() :
	position(0.0f, 0.0f, 0.0f),
	rotation(0.0f, 0.0f, 0.0f),
	viewMatrix()
{
}

void Camera::SetPosition(Float3 position)
{
	this->position = position;
}

void Camera::SetRotation(Float3 rotation)
{
	this->rotation = rotation;
}

Float3 Camera::GetPosition()
{
	return position;
}

Float3 Camera::GetRotation()
{
	return rotation;
}

void Camera::Render()
{
	Float3 up(0.0f, 1.0f, 0.0f);
	Float3 lookAt(0.0f, 0.0f, 1.0f);
	Vector upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	Matrix rotationMatrix;

	upVector = DirectX::XMLoadFloat3(&up);
	positionVector = DirectX::XMLoadFloat3(&position);
	lookAtVector = DirectX::XMLoadFloat3(&lookAt);

	pitch = rotation.x * DEG_TO_RAD;
	yaw = rotation.y * DEG_TO_RAD;
	roll = rotation.z * DEG_TO_RAD;

	rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookAtVector = DirectX::XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = DirectX::XMVector3TransformCoord(upVector, rotationMatrix);

	lookAtVector = DirectX::XMVectorAdd(positionVector, lookAtVector);

	viewMatrix = DirectX::XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void Camera::GetViewMatrix(Matrix* outputMatrix)
{
	*outputMatrix = viewMatrix;
}
