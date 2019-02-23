#pragma once

// PI Constant
const float PI = 3.14159265359f;

// Multiply to convert radians to degrees
const float RAD_TO_DEG = 180.0f / PI;

// Multiply to convert degrees to radians
const float DEG_TO_RAD = 1.0f / RAD_TO_DEG;

#define RELEASE(var) \
	if (var) { \
		(var)->Release(); \
		delete (var); \
		(var) = nullptr; \
	}

#define DELETE_P(var) \
	if (var) { \
		delete (var); \
		(var) = nullptr; \
	}

#define DELETE_A(var) \
	if (var) { \
		delete[] (var); \
		(var) = nullptr; \
	}

#define RELEASE_N(var) \
	if (var) { \
		(var)->Release(); \
		(var) = nullptr; \
	}
