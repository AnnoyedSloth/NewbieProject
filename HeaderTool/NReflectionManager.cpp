#include "NReflectionManager.h"

NReflectionManager* NReflectionManager::instance = nullptr;

const std::string path = "C:\\Users\\soohwan\\source\\repos\\NewbeeProject\\NewbeeProject";

NReflectionManager::NReflectionManager()
{

}

NReflectionManager::~NReflectionManager()
{

}

NReflectionManager* NReflectionManager::Get()
{
	if (instance == nullptr)
		instance = new NReflectionManager();

	return instance;
}

std::vector<std::string> NReflectionManager::FileList()
{	
	auto list = std::filesystem::directory_iterator(path);

	const size_t pathSize = path.size() + 1;

	std::vector<std::string> outList;

	for (auto& file : list)
	{
		std::string temp = file.path().string();
		temp.erase(0, pathSize);

		if (temp.size() > 2 && temp.substr(temp.size() - 2) == ".h" 
			&& temp.find(".gen") == -1)// || (temp.size() > 4 && temp.substr(temp.size() - 4) == ".cpp")) 생각해보니 cpp파일은 매크로 안쓰겟구나,,
		{
			outList.push_back(temp);
		}
	}
	return outList;
}

void NReflectionManager::RemoveBlanksInString(std::string& outString)
{
	while (1)
	{
		if (outString.size() == 0)
			break;

			if (outString[0] == ' ' || outString[0] == '\t')
			{
				outString.erase(0, 1);
			}
			else
			{
				break;
			}
	}
}

void NReflectionManager::MakeReflectionMetaData()
{
	std::vector<std::string> fileArray = FileList();

	// 몇몇 매크로 설정이 필요하지 않은 파일들은 설정목록에서 제거
	for (auto iter = fileArray.begin(); iter != fileArray.end();)
	{
		if (*iter == "NReflectionManager.h" || *iter == "NewbeeProject.h")
		{
			iter = fileArray.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	for (const std::string& fileName : fileArray)
	{
		std::string fullPath = path + "\\" + fileName;
		std::ifstream openFile(fullPath);

		enum class WorkType
		{
			NCLASS,
			NPROPERTY,
			NFUNCTION,
			NONE,
		};

		WorkType workType = WorkType::NONE;

		if (openFile.is_open())
		{
			std::string line;

			std::string writeFilePath = fullPath;
			writeFilePath.insert(writeFilePath.size() - 2, ".generated");

			std::ofstream writeFile(writeFilePath);

			bool isWriteFileEmpty = true;

			std::string propertyInitializeStr;
			std::string propertyGetterSetterStr;

			std::string methodInitializeStr;
			while (getline(openFile, line))
			{
				// 라인 주석 제거
				if (line.find("//") != -1)
				{
					line.erase(line.find("//"), line.length() - 1);
				}
				if (line.find("/*") != -1)
				{
					line.erase(line.find("/*"), line.find("*/"));
				}

				switch (workType)
				{
				case WorkType::NCLASS:
				{
					std::string initializeStr = \
						"#define GENERATED_BODY() \\\n"\
						"bool isReplicate; \\\n" \
						"map<string, NPropertyInfo> propertyInfoMap; \\\n"\
						"virtual void Initialize() \\\n"\
						"{ \\\n";

					writeFile << initializeStr;
				}
				break;
				case WorkType::NPROPERTY:
				{
					RemoveBlanksInString(line);

					size_t idx = line.find(" ");

					std::string propertyType = line.substr(0, idx);
					std::string propertyName = line.substr(idx+1);
					propertyName.pop_back();

					propertyInitializeStr.append("\t\tpropertyInfoMap.insert({\"")
						.append(propertyName)
						.append("\",")
						.append("NPropertyInfo(\"")
						.append(propertyName).append("\", ")
						.append("\"")
						.append(propertyType)
						.append("\", &")
						.append(propertyName)
						.append(")}); \\\n");

					propertyGetterSetterStr.append("\t\t").append(propertyType).append(" Get_").append(propertyName)
						.append("() { return propertyInfoMap[\"").append(propertyName).append("\"].GetValue<").append(propertyType).append(">();}\\\n");
					propertyGetterSetterStr.append("\t\t").append("void Set_").append(propertyName).append("(").append(propertyType).append(" newValue){ ")
						.append("*propertyInfoMap[\"").append(propertyName).append("\"].GetValuePtr<").append(propertyType).append(">() = newValue; }\\\n");
				}
				break;
				case WorkType::NFUNCTION:
				{
					RemoveBlanksInString(line);

					size_t idx = line.find(" ");

					std::string returnType = line.substr(0,idx);
					std::string methodName = line.substr(idx + 1);
					methodName.erase(methodName.find("("), methodName.length() - 1);

					methodInitializeStr.append("\t\tif(isReplicate) NRPCManager::Get()->regist(objectName, \"")
						.append(methodName).append("\", ").append("&NObject::").append(methodName)
						.append(", this);\\\n");
				}
				break;
				case WorkType::NONE:
				{
					//Do nothing
				}
				break;
				default:
					break;
				}
				if (line.find("NCLASS(") != -1)
				{
					workType = WorkType::NCLASS;
					isWriteFileEmpty = false;
				}
				else if (line.find("NPROPERTY(") != -1)
				{
					workType = WorkType::NPROPERTY;
					isWriteFileEmpty = false;
				}
				else if (line.find("NFUNCTION(") != -1)
				{
					workType = WorkType::NFUNCTION;
					isWriteFileEmpty = false;
				}
				else
				{
					workType = WorkType::NONE;
				}
			}
			openFile.close();
			if (isWriteFileEmpty == false)
			{
				writeFile << propertyInitializeStr.append("\\\n");
				writeFile << methodInitializeStr.append("\\\n");
				writeFile << "} \\\n";
				writeFile << propertyGetterSetterStr.append("\\\n");
				writeFile.close();
			}
		}
	}
}