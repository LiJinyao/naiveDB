#include <string>
#include <iostream>
#include "StringKey.h"
#include "stdafx.h"

using namespace naiveDB::dataprocessor;

StringKey::StringKey(
	std::wstring _keyName,
	std::wstring _typeName,
	bool _ifPrimaryKey,
	bool _ifNotNull,
	int _lengthLimit,
	std::wstring _data)
	: Key(_keyName, _typeName, _ifPrimaryKey, _ifNotNull) {
	lengthLimit = _lengthLimit;
	data = _data;
}

StringKey::~StringKey() {
	delete &keyName;
	delete &typeName;
	delete &data;
}

std::wstring StringKey::getData() {
	return data;
}

int StringKey::getLengthLimit() {
	return lengthLimit;
}

void StringKey::setData(std::wstring _data) {
	data = _data;
}