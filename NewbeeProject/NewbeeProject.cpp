// NewbeeProject.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include "NewbeeProject.h"
#include "NObject.h"
#include "NObjectManager.h"
#include "NSerializer.hpp"
#include "NRPCManager.h"

#pragma optimize("", off)

int main()
{
	NObjectManager::Get();
	NRPCManager::Get();

	unsigned int command = 0;

	while (1)
	{
		cout << "\n0. 프로그램 종료\n";
		cout << "1. 오브젝트 스폰\t\t";
		cout << "2. 오브젝트 디스폰\n";
		cout << "3. 오브젝트 선택 및 작업\t";
		cout << "4. 가비지컬렉터 동작\n";
		cout << "5. 리플리케이션 동작 \t\t";
		cout << "6. 오브젝트 목록 조회\n";
		cout << "커맨드를 입력하세요 : ";
		cin >> command;
		
		switch (command)
		{
		case 0:
			return 0;
		break;

		case 1:
		{
			string objName;
			cout << "스폰할 오브젝트의 이름 : ";
			cin >> objName;

			if (NObjectManager::Get()->SpawnObject(objName) == true)
			{
				cout << "오브젝트 " << objName << " 스폰 완료" << endl;
			}
			else
			{
				cout << "오브젝트 스폰 실패" << endl;
			}
		}
		break;
		
		case 2:
		{
			string objName;
			cout << "디스폰할 오브젝트의 이름 : ";
			cin >> objName;

			if (NObjectManager::Get()->DespawnObject(objName) == true)
			{
				cout << "오브젝트 " << objName << "디스폰 완료" << endl;
			}
			else
			{
				cout << "오브젝트 디스폰 실패" << endl;
			}
		}
		break;

		case 3:
		{
			// 여기에서 RootSet 설정이나 Location Move 등  Object의 동작들 추가하면 될듯???
			string objName;
			
			cout << "선택할 오브젝트의 이름 : ";
			cin >> objName;

			NObject* selectedObject = NObjectManager::Get()->GetObject(objName);
			while(selectedObject == nullptr)
			{
				cout << "없어용\n";
				cin >> objName;
			}

			cout << "머하쉴?\n" << "0. 나갈래\n1. 프로퍼티 목록 조회\t2. 프로퍼티 좌표 수정\n3. 프로퍼티 수정\t4. 함수 실행(아직안됨)\n";
			unsigned int objCommand;
			cin >> objCommand;
			switch (objCommand)
			{
			case 0:
				break;
			case 1:
			{
				for (auto var : selectedObject->propertyInfoMap)
				{
					cout << "VariableName : " << var.second.VariableName << endl;
					cout << "DataType : " << var.second.DataTypeStr << endl;
					cout << "Address : " << var.second.DataAddress << endl << endl;
				}
			}
			break;
			case 2:
			{
				float x, y;
				cout << "옮길 좌표 입력하세요 x, y : \n";
				cin >> x >> y;

				selectedObject->Set_locationX(x);
				selectedObject->Set_locationY(y);

				cout << selectedObject->Get_objectName() << "가 x : " << x << ", y : " << y << "로 이동했읍니다\n";
			}
			break;
			case 3:
			{
				cout << "변수 리스트\n";
				for (auto var : selectedObject->propertyInfoMap)
				{
					cout << var.second.DataTypeStr << " " << var.second.VariableName << endl;
				}
				string propertyName;
				cout << "수정 할 Property 이름? : ";
				cin >> propertyName;
				if (selectedObject->propertyInfoMap.find(propertyName) == selectedObject->propertyInfoMap.end())
				{
					cout << "그런 Property 없슴다\n";
					break;
				}

				string newValue;
				cout << "수정 할 값? : ";
				cin >> newValue;

				selectedObject->propertyInfoMap[propertyName].SetValue(newValue);
				break;
			}
			case 4:
			{
				cout << "";
			}
			break;
			}
		}
		break;
		
		case 4:
		{
			cout << "\nPendingKill 상태인 오브젝트들 Unreachable로 Mark 시작\n";
			NObjectManager::Get()->MarkObjectsAsUnreachable();
			cout << "Unreachable로 Mark된 오브젝트들 삭제 시작\n\n";
			NObjectManager::Get()->CollectGarbage();
			cout << "\nGC 완료\n";
		}
		break;
		case 5:
		{
			string objName;
			cout << "리플리케이션 할 오브젝트 이름 : ";
			cin >> objName;

			NObject* obj = NObjectManager::Get()->GetObject(objName);
			if (obj == nullptr)
			{
				cout << "오브젝트가 존재하지 않소,," << endl;
				break;
			}
			obj->Replicate();
		}
		break;
		case 6:
		{
			cout << "오브젝트 목록\n";
			NObjectManager::Get()->PrintValidObjectsName();
		}
		break;
		default:
			cout << "WRONG COMMAND!\n";
		break;
		}		
	}

	//constexpr int framePerSec = 10;
	//float deltaTime = 0;

	//chrono::system_clock::time_point begin = chrono::system_clock::now();
	//chrono::system_clock::time_point lastTickTime = chrono::system_clock::now();

	//while (1)
	//{
	//	chrono::duration<double>sec = chrono::system_clock::now() - begin;

	//	deltaTime = (float)1 / framePerSec;
	//	
	//	chrono::duration<double> count = chrono::system_clock::now()- lastTickTime;

	//	if (count.count() > deltaTime)
	//	{
	//		Tick(deltaTime);

	//		lastTickTime = chrono::system_clock::now();
	//	}
	//}
}