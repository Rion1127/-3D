#include "PipelineManager.h"

void PipelineManager::Ini() {
	for (int i = 0; i < 4; i++) {
		Object3dPipeline_.at(i).Ini();
		SpritePipeline_.at(i).Ini();
	}
};