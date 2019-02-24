#pragma once

#include <DirectXMath.h>
#include "defines.h"
#include "typedefs.h"

// Adds two Float3s and returns the result.
Float3 add(Float3 a, Float3 b);

// Adds three Float3s and returns the result.
Float3 add(Float3 a, Float3 b, Float3 c);

// Performs a unary minus operation (-v) and returns the result.
Float3 unm(Float3 v);

// Multiplies a Float3 with a scalar and returns the result.
Float3 mul(Float3 v, float s);
