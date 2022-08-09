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

	// ���� ������ �ִ� .h ���� ����Ʈȭ
	std::vector<std::string> FileList();

	void RemoveBlanksInString(std::string& outString);

	// ���� ���پ� �о NCLASS, NPROPERTY, NFUNCTION ����� ���� �� ��ũ�� ���� Mark �� ����
	void MakeReflectionMetaData();
};
