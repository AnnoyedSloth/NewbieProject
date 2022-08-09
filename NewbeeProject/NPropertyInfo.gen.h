#pragma once
#include "NewbeeProject.h"

struct NPropertyInfo
{
	string VariableName;
	string DataType;
	void* DataAddress;

	NPropertyInfo(string variableName, string dataType,  void* dataAddress)
	{
		VariableName = variableName;
		DataType = dataType;
		DataAddress = dataAddress;
	}
};
