#pragma once
#include <DirectXMath.h>
#include "defines.h"
#include "typedefs.h"

// Defines the view matrix
class Camera
{
public:
	// Set the position of the camera
	void SetPosition(Float3 position);

	// Set the rotation of the camera
	void SetRotation(Float3 rotation);

	// Get the position of the camera
	Float3 GetPosition();

	// Get the rotation of the camera
	Float3 GetRotation();

	// To be called when rendering. Recomputes the view matrix.
	void Render();

	// Writes the view matrix to the parameter
	void GetViewMatrix(Matrix* outputMatrix);

private:
	// Stores the position
	Float3 position = Float3(0.0f, 0.0f, 0.0f);

	// Stores the rotation
	Float3 rotation = Float3(0.0f, 0.0f, 0.0f);

	// Stores the view matrix
	Matrix viewMatrix = Matrix();
};

