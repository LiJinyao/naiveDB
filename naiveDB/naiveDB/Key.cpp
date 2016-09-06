#include <string>
#include <iostream>
#include "Key.h"
#include "stdafx.h"

using namespace naiveDB::dataprocessor;

Key::Key(
	std::wstring _keyName,
	std::wstring _typeName,
	bool _ifPrimaryKey,
	bool _ifNotNull) {
	keyName = _keyName;
	typeName = _typeName;
	ifPrimaryKey = _ifPrimaryKey;
	ifNotNull = _ifNotNull;
}

Key::~Key() {
	delete &keyName;
	delete &typeName;
}



std::wstring Key::getKeyName() {
	return keyName;
}

std::wstring Key::getTypeName() {
	return typeName;
}

bool Key::getPrimaryStatus() {
	return ifPrimaryKey;
}

bool Key::getNotNullStatus() {
	return ifNotNull;
}