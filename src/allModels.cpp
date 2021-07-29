/******************************************************
*   OpenGL demo
*
*   allModels is a namespace that contains and handles
*	all Model objects in the demo.
*	A Model object is a GameObject that has a model attached
*	to it. 
*
*	Author: Magnus Söderberg
*
********************************************************/

#include "allModels.h"

void allModels::CreateModel() {
	allModelsList.push_back(std::make_shared<Model>());
}

std::shared_ptr<Model> allModels::GetObject(int index) {
	if (index < allModelsList.size() && index >= 0)
		return allModelsList[index];
	else
		throw std::runtime_error("---ERROR: Wanted Model was out of range----");
}

void allModels::DeleteObject(int index) {
	if (index < allModels::GetSize() && index >= 0) {
		allModelsList.erase(allModelsList.begin() + index);
	}
	else
		throw std::runtime_error("----ERROR: Could not delete Model, index out of bounds---");
}


void allModels::Serialize() {

	std::ofstream out("saved_data/models.json");
	out << "[";
	json j;
	j["Number Of Models"] = allModels::GetSize();
	out << j;
	out << ",";

	for (int i = 0; i < allModels::GetSize(); i++) {

		out << std::setw(4) << allModelsList[i]->Serialize();
		if (i != allModels::GetSize() - 1) {
			out << ",";
		}
	}

	out << "]";
}

void allModels::DeSerialize() {
	json j;
	{
	    std::ifstream in("saved_data/models.json");

	    in >> j;
	    in.close();
	}
	int num = 0;

	for (auto& el : j[0].items())
	{
	    if (el.key() == "Number Of Models")
	    {
	        num = el.value();
	    }
	}

	for (int i = 0; i < num; i++)
	{
	    CreateModel();
	}

	for (int i = 0; i < GetSize(); i++)
	{
	    GetObject(i)->DeSerialize(j[i+1]);
	}
	std::cout << "Deserialized all Model:s" << std::endl;
	    
}


