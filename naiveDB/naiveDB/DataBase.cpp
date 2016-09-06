#pragma once
#include "DataBase.h"
#include "AST.hpp"
#include "stdafx.h"

using namespace naiveDB;

DataBase::DataBase(std::wstring _DBName) {
	DBName = _DBName;
}

DataBase::~DataBase() {

}

void DataBase::Create(naiveDB::parser::CreateStatement _cs) {
	naiveDB::parser::CreateStatement cs = _cs;
	std::wstring formName = cs.tableName; //����2
	std::vector<std::vector<std::wstring>> formDefine;
	
	std::vector<naiveDB::parser::ColumnStatement> columnDefine = cs.columns;
	
	//�������1
	for (unsigned int i = 0; i < columnDefine.size(); i++) {
		std::vector<std::wstring> s;

		//name��typeû����
		s.push_back(columnDefine[i].name);
		s.push_back(columnDefine[i].dataType);
		
		//���attribute��"NOT NULL"��isPrimaryKeyΪfalse��isNotNullΪtrue
		//���attribute��"PRIMARY KEY"��isPrimaryKeyΪtrue,isNotNullΪfalse
		if (columnDefine[i].attribute == L"NOT NULL") {
			s.push_back(L"false");
			s.push_back(L"true");
		}
		else if (columnDefine[i].attribute == L"PRIMARY KEY") {
			s.push_back(L"true");
			s.push_back(L"false");
		}

		//�������ǳ������ƣ���Ҫ��intת��wstring
		std::wostringstream oss;
		oss << columnDefine[i].limit;
		std::wstring wstr = oss.str();
		s.push_back(wstr);

		formDefine.push_back(s);
	}

	naiveDB::dataprocessor::Form f = naiveDB::dataprocessor::Form(formDefine, formName);
	formSet.push_back(f);
}

//ע������ֻʵ���� INSERT INTO ������ VALUES (ֵ1, ֵ2,....) һ�־�ʽ
//�þ�ʽҪ���ֵ�������಻�٣��Ҹ�������ϱ���
void DataBase::Insert(naiveDB::parser::InsertStatement _is) {
	std::wstring formName = _is.tableName;
	std::vector<std::wstring> data = _is.values;
	std::vector<std::wstring> definition = _is.columns;
	for (unsigned int i = 0; i < formSet.size(); i++) {
		
		if (formSet[i].getFormName() == formName) {
			//����ֶ����Ƿ�ƥ��
			if (data.size() != formSet[i].getFormHeader().size()) {
				//�쳣����
			}

			/*�˴���Ҫ��������쳣����*/

			//ȷ������Ϸ�������¼
			formSet[i].Insert(data);
		}
	}
}

void DataBase::Select(naiveDB::parser::SelectStatement _ss) {

}


