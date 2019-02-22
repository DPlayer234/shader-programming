#pragma once

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
