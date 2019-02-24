#pragma once
#include "CubeModel.h"
class Skybox : public CubeModel
{
public:
	void InitializeSkybox();

protected:
	bool CreateIndexArray() override;

private:
	const float skyboxBoundSize = 100.0f;
	static UINT indices[36];
};

