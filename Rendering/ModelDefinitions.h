#pragma once

#include "Model.h"
#include "CubeModel.h"
#include "UnlitShader.h"
#include "BlinnPhongShader.h"

MODEL_SHADER(CubeModelUnlit, CubeModel, UnlitShader);
MODEL_SHADER(CubeModelBP, CubeModel, BlinnPhongShader);
