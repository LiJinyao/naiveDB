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
	std::wstring formName = cs.tableName; //参数2
	std::vector<std::vector<std::wstring>> formDefine;
	
	std::vector<naiveDB::parser::ColumnStatement> columnDefine = cs.columns;
	
	//制造参数1
	for (unsigned int i = 0; i < columnDefine.size(); i++) {
		std::vector<std::wstring> s;

		//name和type没问题
		s.push_back(columnDefine[i].name);
		s.push_back(columnDefine[i].dataType);
		
		//如果attribute是"NOT NULL"则isPrimaryKey为false，isNotNull为true
		//如果attribute是"PRIMARY KEY"则isPrimaryKey为true,isNotNull为false
		if (columnDefine[i].attribute == L"NOT NULL") {
			s.push_back(L"false");
			s.push_back(L"true");
		}
		else if (columnDefine[i].attribute == L"PRIMARY KEY") {
			s.push_back(L"true");
			s.push_back(L"false");
		}

		//第五项是长度限制，需要把int转成wstring
		std::wostringstream oss;
		oss << columnDefine[i].limit;
		std::wstring wstr = oss.str();
		s.push_back(wstr);

		formDefine.push_back(s);
	}

	naiveDB::dataprocessor::Form f = naiveDB::dataprocessor::Form(formDefine, formName);
	formSet.push_back(f);
}

//注意这里只实现了 INSERT INTO 表名称 VALUES (值1, 值2,....) 一种句式
//该句式要求键值对数不多不少，且各方面符合表定义
void DataBase::Insert(naiveDB::parser::InsertStatement _is) {
	std::wstring formName = _is.tableName;
	std::vector<std::wstring> data = _is.values;
	std::vector<std::wstring> definition = _is.columns;
	for (unsigned int i = 0; i < formSet.size(); i++) {
		
		if (formSet[i].getFormName() == formName) {
			//检查字段数是否匹配
			if (data.size() != formSet[i].getFormHeader().size()) {
				//异常处理
			}

			/*此处需要加入更多异常处理*/

			//确保输入合法后插入记录
			formSet[i].Insert(data);
		}
	}
}

void DataBase::Select(naiveDB::parser::SelectStatement _ss) {

}


