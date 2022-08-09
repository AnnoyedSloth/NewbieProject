#pragma once
#include <vector>
#include <filesystem>
#include <fstream>
#include <string>

class NReflectionManager
{	
private:

	NReflectionManager();
	~NReflectionManager();
	static NReflectionManager* instance;

public:
	static NReflectionManager* Get();

	// 현재 폴더에 있는 .h 파일 리스트화
	std::vector<std::string> FileList();

	void RemoveBlanksInString(std::string& outString);

	// 파일 한줄씩 읽어서 NCLASS, NPROPERTY, NFUNCTION 존재시 라인 및 매크로 종류 Mark 및 수정
	void MakeReflectionMetaData();
};
