#include "NObject.h"

NObject::NObject()
{
	locationX = 0;
	locationY = 0;
	parent = nullptr;
	isReplicate = false;

	propertyInfoMap.clear();
	functionInfoMap.clear();
	observingObjects.clear();

	camera = new NObject(this, "Camera");
	weapon = new NObject(this, "Weapon");
}

NObject::NObject(NObject* parent_, string name)
{
	parent = parent_;
	objectName = name;
}

void NObject::Move(float x, float y)
{
	locationX += x;
	locationY += y;

	cout << objectName << " location set to : " << locationX << ", " << locationY << endl;
}