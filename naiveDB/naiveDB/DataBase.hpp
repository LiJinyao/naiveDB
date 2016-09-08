#pragma once
#include "stdafx.h"
#include "Form.hpp"
#include "AST.hpp"

namespace naiveDB {
	class DataBase {
	public:
		DataBase() {

		}

		DataBase(std::wstring _dbName) {
			DBName = _dbName;
		}


		~DataBase() {

		}

		//建表
		void Create(naiveDB::parser::CreateTableStatement _cs) {
			naiveDB::parser::CreateTableStatement cs = _cs;
			std::wstring formName = cs.tableName; //参数2
			std::vector<std::vector<std::wstring>> formDefine;
			if (formDefine.size() == 0) {
				std::wcout << L"错误！不能创建空表，请给出表的定义。" << std::endl;
				return;
			}
			for (unsigned int i = 0; i < formSet.size(); i++) {
				if (formSet[i].getFormName() == formName) {
					std::wcout << L"名为" << formName << L"的表已经存在，请选用其他名称。" << std::endl;
					return;
				}
			}

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
				std::wstringstream ss;
				ss << columnDefine[i].limit;
				std::wstring wstr;
				ss >> wstr;
				s.push_back(wstr);

				formDefine.push_back(s);
			}

			naiveDB::dataprocessor::Form f = naiveDB::dataprocessor::Form(formDefine, formName);
			formSet.push_back(f);
		}

		//注意这里只实现了 INSERT INTO 表名称 VALUES (值1, 值2,....) 一种句式
		//该句式要求键值对数不多不少，且各方面符合表定义
		void Insert(naiveDB::parser::InsertStatement _is) {
			std::wstring formName = _is.tableName;
			std::vector<std::wstring> data = _is.values;
			std::vector<std::wstring> definition = _is.columns;

			bool if_found = false;
			int foundform = -1;
			//先判断有没有这张表
			for (unsigned int i = 0; i < formSet.size(); i++) {
				if (formSet[i].getFormName() == formName) {
					if_found = true;
					foundform = i;
					break;
				}
			}
			if (!if_found) {
				std::wcout << L"表" << formName << L"不存在，请在建表后进行插入操作。" << std::endl;
				return;
			}

			std::vector<std::vector<std::wstring>> fh = formSet[foundform].getFormHeader();

			//必须插入完整记录
			if (definition.size() == 0) {
				if (data.size() != fh.size()) {
					std::wcout << L"错误！参数数量不匹配。" << std::endl;
					return;
				}
				//进行数据类型检查
				bool if_TypeError = false;
				for (unsigned int i = 0; i < data.size(); i++) {
					bool if_int = true;
					std::wstring tmp = data[i];
					for (unsigned int j = 0; j < tmp.length(); j++) {
						if (tmp[j] >= L'9' || tmp[j] <= L'0') {
							if_int = false;
							break;
						}
					}
					if (if_int == true && fh[i][1] == L"wstring") {
						std::wcout << L"插入了错误的数据类型，" << fh[i][0] << L"是char型。" << std::endl;
						if_TypeError = true;
					}
					if (if_int == false && fh[i][1] == L"int") {
						std::wcout << L"插入了错误的数据类型，" << fh[i][0] << L"是int型。" << std::endl;
						if_TypeError = true;
					}
				}
				if (if_TypeError) {
					return;
				}

				//进行长度限制检查
				bool if_LengthError = false;
				for (unsigned int i = 0; i < data.size(); i++) {
					if (fh[i][1] == L"wstring") {
						std::wstringstream ss;
						int limit;
						ss << fh[i][4];
						ss >> limit;
						if (data[i].length() > limit) {
							std::wcout << L"错误！字段" << fh[i][0] << L"超出长度限制。" << std::endl;
							if_LengthError = true;
						}
					}
				}
				if (if_LengthError) {
					return;
				}

				//进行属性限制检查
				bool if_PropertyError = false;

			}
			//可插入部分字段
			else {
				if (data.size() != definition.size()) {
					std::wcout << L"错误！参数数量不匹配。" << std::endl;
					return;
				}

			}
			formSet[foundform].Insert(data);
		}

		//这里只实现选择一张表中的内容
		void Select(naiveDB::parser::SelectStatement _ss) {
			naiveDB::parser::SelectStatement ss = _ss;
			int foundForm = -1;
			for (unsigned int i = 0; i < formSet.size(); i++) {
				if (formSet[i].getFormName() == ss.fromtables[0]) {
					foundForm = i;
					break;
				}
			}
			//select全表
			if (ss.columns.size() == 1 && ss.columns[0] == L"") {
				formSet[foundForm].Select();
				return;
			}
			//select部分列（无条件）
			else if (ss.whereClause.statements.size() == 0) {
				formSet[foundForm].Select(ss.columns);
				return;
			}
			//select部分列（单条件）
			else { 
				std::wstring condition[2];
				condition[0] = ss.whereClause.statements[0].lh;
				condition[1] = ss.whereClause.statements[0].rh;
				formSet[foundForm].Select(ss.columns, condition);
				return;
			}
			
		}

		void Delete(naiveDB::parser::DeleteStatement _ds) {
			naiveDB::parser::DeleteStatement ds = _ds;
			std::wstring formName = ds.tableName;
			int foundForm = -1;
			//检查表是否存在
			bool if_found = false;
			for (unsigned int i = 0; i < formSet.size(); i++) {
				if (formSet[i].getFormName() == formName) {
					if_found = true;
					foundForm = i;
					break;
				}
			}
			if (!if_found) {
				std::wcout << L"所要删除的表不存在。" << std::endl;
				return;
			}

			//删除全表
			if (ds.whereClause.statements.size() == 0) {
				formSet[foundForm].Delete();
			}
			//删除部分记录(目前只支持单条件)
			else {
				bool keyflag = false;
				bool valueflag = false;
				int keycolumn = -1;

				std::vector<std::vector<std::wstring>> _copy = formSet[foundForm].getFormHeader();
				for (unsigned int i = 0; i < _copy.size(); i++) {
					if (_copy[i][0] == ds.whereClause.statements[0].lh) {
						keyflag = true;
						keycolumn = i;
						break;
					}
				}
				if (!keyflag) {
					std::wcout << L"错误！" << ds.whereClause.statements[0].lh
						<< L"不是表" << formSet[foundForm].getFormName()
						<< L"的成员。" << std::endl;
					return;
				}
				//检查键值是否存在
				/*if (keyflag) {
				std::vector<naiveDB::dataprocessor::AVL<int>*> introots = formSet[foundForm].getIntHeader();
				std::vector<naiveDB::dataprocessor::AVL<std::wstring>*> wstringroots = formSet[foundForm].getWstringHeader();
				for (unsigned int i = 0; i < introots.size(); i++) {
				if (introots[i]->GetName() == ds.whereClause.statements[0].lh) {
				std::wstringstream ss;
				int num;
				ss << ds.whereClause.statements[0].
				}
				}


				}*/

				std::vector<std::vector<std::wstring>> condition;
				std::vector<std::wstring> s;
				s.push_back(ds.whereClause.statements[0].lh);
				s.push_back(ds.whereClause.statements[0].rh);
				condition.push_back(s);
				std::wstring relation = L"NONE";
				int affected = formSet[foundForm].Delete(condition, relation);

				if (affected == 0) {
					std::wcout << L"删除失败，没有符合条件的记录" << std::endl;
				}
			}
		}

		void Update(naiveDB::parser::UpdateStatement _us) {
			naiveDB::parser::UpdateStatement us = _us;
			int foundForm = -1;
			for (unsigned int i = 0; i < formSet.size(); i++) {
				if (formSet[i].getFormName() == us.tableName) {
					foundForm = i;
					break;
				}
			}

			if (foundForm == -1) {
				std::wcout << L"错误！所要操作的表不存在。" << std::endl;
				return;
			}

			std::vector<std::vector<std::wstring>> set;
			std::vector<std::vector<std::wstring>> condition;
			for (unsigned int i = 0; i < us.sets.size(); i++) {
				std::vector<std::wstring> s;
				s.push_back(us.sets[i].col);
				s.push_back(us.sets[i].val);
				set.push_back(s);
				s.clear();
			}
		
			for (unsigned int i = 0; i < us.whereClause.statements.size(); i++) {
				std::vector<std::wstring> s;
				s.push_back(us.whereClause.statements[i].lh);
				s.push_back(us.whereClause.statements[i].rh);
				condition.push_back(s);
				s.clear();
			}

			formSet[foundForm].Update(set, condition);

		}

		void Use(naiveDB::parser::UseDatabaseStatement _uds) {

		}


	protected:

	private:
		std::vector<dataprocessor::Form> formSet;
		std::wstring DBName;
	};
}

		

	



