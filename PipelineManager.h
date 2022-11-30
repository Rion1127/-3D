#pragma once
#include "Pipeline.h"
#include <array>
class PipelineManager {
public:
	static void Ini();

	static Pipeline GetObj3dPipeline();
private:
	static std::array<Pipeline, 4> Object3dPipeline_;
	static std::array<Pipeline, 4> SpritePipeline_;


};