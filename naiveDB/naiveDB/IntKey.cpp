#include "stdafx.h"

using namespace naiveDB::dataprocessor;

IntKey::IntKey(
	std::wstring _keyName,
	std::wstring _typeName,
	bool _ifPrimaryKey,
	bool _ifNotNull,
	int _data)
	: Key(_keyName, _typeName, _ifPrimaryKey, _ifNotNull) {
	data = _data;
}

IntKey::~IntKey() {
	delete &keyName;
	delete &typeName;
	delete &data;
}

int IntKey::getData() {
	return data;
}

void IntKey::setData(int _data) {
	data = _data;
}