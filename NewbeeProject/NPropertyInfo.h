#pragma once
#include "NewbeeProject.h"

struct NPropertyInfo
{
	enum class Type
	{
		TYPE_INT = 0,
		TYPE_LONG,
		TYPE_FLOAT,
		TYPE_DOUBLE,
		TYPE_STRING,
		TYPE_NONE
	};

	string VariableName;
	string DataTypeStr;
	void* DataAddress;
	Type PropertyType;

	NPropertyInfo(){}

	NPropertyInfo(string variableName, string dataType,  void* dataAddress)
	{
		VariableName = variableName;
		DataTypeStr = dataType;
		DataAddress = dataAddress;

		if (dataType == "int")
			PropertyType = Type::TYPE_INT;
		else if (dataType == "long")
			PropertyType = Type::TYPE_LONG;
		else if (dataType == "float")
			PropertyType = Type::TYPE_FLOAT;
		else if (dataType == "double")
			PropertyType = Type::TYPE_DOUBLE;
		else if (dataType == "string")
			PropertyType = Type::TYPE_STRING;
		else
			PropertyType = Type::TYPE_NONE;
	}

	template<typename T>
	T GetValue()
	{
		T* value = (T*)DataAddress;
		return *value;
	}

	template<typename T>
	T* GetValuePtr()
	{
		T* value = (T*) DataAddress;
		return value;
	}

	void SetValue(string& newValue)
	{

		switch (PropertyType)
		{
		case Type::TYPE_INT:
		{
			*GetValuePtr<int>() = std::stoi(newValue);
		}
		break;
		case Type::TYPE_LONG:
		{
			*GetValuePtr<long>() = std::stol(newValue);
		}
		break;
		case Type::TYPE_FLOAT:
		{
			*GetValuePtr<float>() = std::stof(newValue);
		}
		break;
		case Type::TYPE_DOUBLE:
		{
			*GetValuePtr<double>() = std::stod(newValue);
		}
		break;
		case Type::TYPE_STRING:
		{
			*GetValuePtr<string>() = newValue;
		}
		break;
		default:
		{
			cout << "Type does not exist\n" << endl;
		}
		break;
		}

	}
};