#pragma once
#include <DirectXMath.h>
#include "defines.h"
#include "typedefs.h"

class Camera
{
public:
	void SetPosition(Float3 position);
	void SetRotation(Float3 rotation);

	Float3 GetPosition();
	Float3 GetRotation();

	void Render();
	void GetViewMatrix(Matrix* outputMatrix);

private:
	Float3 position = Float3(0.0f, 0.0f, 0.0f);
	Float3 rotation = Float3(0.0f, 0.0f, 0.0f);
	Matrix viewMatrix = Matrix();
};

