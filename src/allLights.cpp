/******************************************************
*   OpenGL demo
*
*   allLights is a namespace which contains a list
*	of all lights in the demo. 
*
	Author: Magnus Söderberg
*
********************************************************/

#include "allLights.h"
#include <iostream>
class Light;

void allLights::CreateLight() {
	allLightsList.push_back(std::make_shared<Light>());
}

std::shared_ptr<Light> allLights::GetObject(int index){ 
	if (index < allLightsList.size() && index >= 0)
		return allLightsList[index];
	else
		throw std::out_of_range("Wanted Light was out of range");
}

std::vector<const char*> allLights::GetAllLightNames() {
	std::vector<const char*> list;
	for (int i = 0; i < allLightsList.size(); i++) {
		list.push_back(allLightsList[i]->GetName());
	}
	return list;
}

void allLights::DeleteObject(int index) {
	if (index < allLights::GetSize() && index >= 0) {
		allLightsList.erase(allLightsList.begin() + index);
	}
	else
		throw std::out_of_range("----ERROR: Could not delete light, index out of bounds---");
}

void allLights::Serialize() {

	std::ofstream out("saved_data/lights.json");
	out << "[";
	json j;
	j["Number Of Lights"] = allLights::GetSize();
	out << j;
	out << ",";

	for (int i = 0; i < allLights::GetSize(); i++) {

		out << std::setw(4) << allLightsList[i]->Serialize();
		if (i != allLights::GetSize() - 1) {
			out << ",";
		}

	}

	out << "]";
}

void allLights::DeSerialize() {
	json j;
	{
		std::ifstream in("saved_data/lights.json");

		in >> j;
		in.close();
	}
	int num = 0;

	for (auto& el : j[0].items())
	{
		if (el.key() == "Number Of Lights")
		{
			num = el.value();
		}
	}

	for (int i = 0; i < num; i++)
	{
		CreateLight();
	}

	for (int i = 0; i < GetSize(); i++)
	{
		GetObject(i)->DeSerialize(j[i + 1]);
	}
	std::cout << "Deserialized all Light:s " << std::endl;

}


