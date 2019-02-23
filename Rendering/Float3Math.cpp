#include "Float3Math.h"

Float3 add(Float3 a, Float3 b)
{
	return Float3(
		a.x + b.x,
		a.y + b.y,
		a.z + b.z);
}

Float3 add(Float3 a, Float3 b, Float3 c)
{
	return Float3(
		a.x + b.x + c.x,
		a.y + b.y + c.y,
		a.z + b.z + c.z);
}

Float3 unm(Float3 v)
{
	return Float3(-v.x, -v.y, -v.z);
}

Float3 mul(Float3 v, float s)
{
	return Float3(v.x * s, v.y * s, v.z * s);
}
