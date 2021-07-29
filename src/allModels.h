#ifndef ALLMODELS_H
#define ALLMODELS_H

#include <vector>
#include <memory>

#include "Model.h"


namespace allModels
{
	static std::vector<std::shared_ptr<Model>> allModelsList;
	void CreateModel();
	inline int GetSize() { return allModelsList.size(); }
	void DeleteObject(int index);
	std::shared_ptr<Model> GetObject(int index);
	void Serialize();
	void DeSerialize();
}

#endif