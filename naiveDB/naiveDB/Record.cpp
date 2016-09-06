#include "stdafx.h"

using namespace naiveDB::dataprocessor;

//创建一般记录
Record::Record(int _id, std::vector<std::vector<std::wstring>> formDefine) {
	id = _id;
	for (unsigned int i = 0; i < formDefine.size(); i++) {
		if (formDefine[i][1] == L"wstring") {
			std::wstringstream ss;
			int num;
			ss << formDefine[i][4];
			ss >> num;
			record.push_back(new StringKey(
				formDefine[i][0],
				formDefine[i][1],
				formDefine[i][2] == L"0" ? true : false,
				formDefine[i][3] == L"0" ? true : false,
				num,
				formDefine[i][5]
			));
		}
		else if (formDefine[i][1] == L"int") {
			std::wstringstream ss;
			int num;
			ss << formDefine[i][5];
			ss >> num;
			record.push_back(new IntKey(
				formDefine[i][0],
				formDefine[i][1],
				formDefine[i][2] == L"0" ? true : false,
				formDefine[i][3] == L"0" ? true : false,
				num
			));
		}
	}
}

Record::~Record() {
	/*delete &record;
	delete &id;*/
}

int Record::getId() {
	return id;
}

//StringKey* Record::getKey() {
//	return (StringKey*)record[0];
//}


void Record::printRecord() {
	for (unsigned int i = 0; i < record.size(); i++) {
		if (record[i]->getTypeName() == L"int") {
			IntKey *k = (IntKey*)record[i];
			std::cout << k->getData() << std::endl;
		}
		else if (record[i]->getTypeName() == L"wstring") {
			StringKey *k = (StringKey*)record[i];
			std::wcout << k->getData() << std::endl;
		}
	}
	//std::cout << std::endl;
}


Key* Record::findKey(std::wstring _name)
{
	for (int i = 0; i < record.size(); i++)
	{
		if (record[i]->getKeyName() == _name)
		{
			return record[i];
		}
	}
}

std::vector<Key *> Record::getRecord()
{
	return  record;
}