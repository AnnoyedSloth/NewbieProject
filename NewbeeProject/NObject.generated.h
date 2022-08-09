#define GENERATED_BODY() \
bool isReplicate; \
map<string, NPropertyInfo> propertyInfoMap; \
virtual void Initialize() \
{ \
		propertyInfoMap.insert({"locationX",NPropertyInfo("locationX", "float", &locationX)}); \
		propertyInfoMap.insert({"locationY",NPropertyInfo("locationY", "float", &locationY)}); \
		propertyInfoMap.insert({"objectName",NPropertyInfo("objectName", "string", &objectName)}); \
		propertyInfoMap.insert({"camera",NPropertyInfo("camera", "NObject*", &camera)}); \
		propertyInfoMap.insert({"weapon",NPropertyInfo("weapon", "NObject*", &weapon)}); \
\
		if(isReplicate) NRPCManager::Get()->regist(objectName, "Replicate", &NObject::Replicate, this);\
		if(isReplicate) NRPCManager::Get()->regist(objectName, "Update", &NObject::Update, this);\
		if(isReplicate) NRPCManager::Get()->regist(objectName, "Move", &NObject::Move, this);\
\
} \
		float Get_locationX() { return propertyInfoMap["locationX"].GetValue<float>();}\
		void Set_locationX(float newValue){ *propertyInfoMap["locationX"].GetValuePtr<float>() = newValue; }\
		float Get_locationY() { return propertyInfoMap["locationY"].GetValue<float>();}\
		void Set_locationY(float newValue){ *propertyInfoMap["locationY"].GetValuePtr<float>() = newValue; }\
		string Get_objectName() { return propertyInfoMap["objectName"].GetValue<string>();}\
		void Set_objectName(string newValue){ *propertyInfoMap["objectName"].GetValuePtr<string>() = newValue; }\
		NObject* Get_camera() { return propertyInfoMap["camera"].GetValue<NObject*>();}\
		void Set_camera(NObject* newValue){ *propertyInfoMap["camera"].GetValuePtr<NObject*>() = newValue; }\
		NObject* Get_weapon() { return propertyInfoMap["weapon"].GetValue<NObject*>();}\
		void Set_weapon(NObject* newValue){ *propertyInfoMap["weapon"].GetValuePtr<NObject*>() = newValue; }\
\
