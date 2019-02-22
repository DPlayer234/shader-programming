#pragma once
#include <DirectXMath.h>
#include "typedefs.h"

class Camera
{
public:
	Camera();

	void SetPosition(Float3 position);
	void SetRotation(Float3 rotation);

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	Float3 GetPosition();
	Float3 GetRotation();

	void Render();
	void GetViewMatrix(Matrix* outputMatrix);

private:
	Float3 position;
	Float3 rotation;
	Matrix viewMatrix;
};

