#pragma once
#include "NewbeeProject.h"
#include "NSerializer.hpp"
#include <functional>

struct NFunctionInfo
{
	enum class Types
	{
		TYPE_VOID = 0,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_DOUBLE,
		TYPE_STRING,
		TYPE_NONE
	};

	NFunctionInfo(){}

	NFunctionInfo(string funcName, string returnType, std::function<void(Serializer*, const char*, size_t)> funcPtr)
	{

	}

	string FuncName;
	std::function<void(Serializer*, const char*, size_t)> FuncPtr;
	string ReturnType;
	vector<string> ArgumentsType;
};