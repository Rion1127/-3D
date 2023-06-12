#pragma once
#include "Pipeline.h"
#include <array>
#include <string>
#include <map>

class PipelineManager {
public:
	static void Ini();

	static void Create(
		std::string pipelinename, CULL_MODE cullmode,
		TOPOLOGY_TYPE topologytype, WRIGHT_MASK depthWriteMasc,
		TEXTURE_ADDRESS_MODE uvMode);
	static void AddPipeline(std::string pipelinename);
public:
	//ÉQÉbÉ^Å[
	static PipelineObject* GetPipelineObjects(std::string name) { return pipelineObjects_[name].get(); }
private:
	static std::map<std::string, std::unique_ptr<PipelineObject>> pipelineObjects_;
};