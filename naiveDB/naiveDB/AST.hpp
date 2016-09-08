#pragma once
/****************************************
*
* SQL AST
*
****************************************/
#define BOOST_SPIRIT_UNICODE
#include "stdafx.h"
#include "Form.hpp"

namespace naiveDB {
	namespace parser {

		/****************************************
		*
		* WHERE satatement AST
		*
		****************************************/
		
		struct WhereConditionStatement {
			std::wstring lh, op, rh;
			// test
			friend std::wostream& operator<<(std::wostream& os, WhereConditionStatement const& ws) {
				return os << ws.lh << " " << ws.op << " " << ws.rh;
			}
		};

		struct WhereStatement {
			std::vector<WhereConditionStatement> statements;

			// test
			friend std::wostream& operator<<(std::wostream& os, WhereStatement const& ws) {
				if (ws.statements.size() > 0) {
					os << "WHERE ";
					for (auto val : ws.statements) {
						os << val << ',';
					}
				}
				return os;
			}
		};

		/****************************************
		*
		* SELECT AST
		*
		****************************************/
		struct SelectStatement {
			std::vector<std::wstring> columns, fromtables; //columns中只有一个元素且这个元素为L""时代表SELECT *
			WhereStatement whereClause;

			// test
			friend std::wostream& operator<<(std::wostream& os, SelectStatement const& ss) {
				os << "SELECT ";
				for (auto val : ss.columns) {
					os << val << ',';
				}
				os << " FROM ";
				for (auto val : ss.fromtables) {
					os << val << ',';
				}
				return os << ss.whereClause;
			}
		};

		/**************************************** 
		*
		* CREATE AST
		*
		****************************************/

		// Column attribute.
		// eg. name [dataType(limit)]
		struct ColumnStatement {
			std::wstring name;
			std::wstring dataType;
			int limit = 0; // length limit.
			std::wstring attribute; // NUT NULL, PRIMARY KEY
			friend std::wostream& operator<<(std::wostream& os, ColumnStatement const& ss) {
				os << ss.name << " " << ss.dataType;
				if (0 != ss.limit) {
					os << "(" << ss.limit << ")";
				}
				if (!ss.attribute.empty()) {
					os << " " << ss.attribute << ',';

				}
				return os;
			}
		};

		struct CreateTableStatement {
			std::wstring tableName;
			std::vector<ColumnStatement> columns;
			friend std::wostream& operator<<(std::wostream& os, CreateTableStatement const& ss) {
				os << "CREATE TABLE " << ss.tableName << "(";
				for (auto val : ss.columns) {
					os << val;
				}

				return os << ')';
			}
		};
		
		struct CreateDatabaseStatement {
			std::wstring dbName;
			friend std::wostream& operator<<(std::wostream& os, CreateDatabaseStatement const& ss) {
				return os << "CREATE DATABASE " << ss.dbName;
			}
		};

		/****************************************
		*
		* DELETE AST
		*
		****************************************/

		struct DeleteStatement {
			std::wstring tableName;
			WhereStatement whereClause;

			// test
			friend std::wostream& operator<<(std::wostream& os, DeleteStatement const& ss) {
				os << "DELETE FROM " << ss.tableName;
				return os << ss.whereClause;
			}

		};

		/****************************************
		*
		* INSERT AST
		* INSERT INTO 表名称 VALUES (值1, 值2,....)
		****************************************/

		struct InsertStatement {
			std::wstring tableName;
			std::vector<std::wstring> columns, values;
			friend std::wostream& operator<<(std::wostream& os, InsertStatement const& ss) {
				os << "INSERT INTO " << ss.tableName
					<< "(";
				for (auto val : ss.columns) {
					os << val << ',';
				}
				os << ")"
					<< " VALUES "
					<< "(";
				for (auto val : ss.values) {
					os << val << ',';
				}
				return os << ')';
			}

		};

		/****************************************
		*
		* UPDATE AST
		* UPDATE 表名称 SET 列名称 = 新值 WHERE 列名称 = 某值
		****************************************/
		// SET 列名称 = 新值 
		struct SetStatement {
			// col： 列名
			// val： 要设置的值
			std::wstring col, val;
		};
		struct UpdateStatement {
			std::wstring tableName; // 要修改的表名
			std::vector<SetStatement> sets; //要修改的内容，见struct SetStatement
			WhereStatement whereClause; //where限制条件
		};

		/****************************************
		*
		* USE statement
		*
		****************************************/
		
		struct UseDatabaseStatement{
			std::wstring dbName;
			friend std::wostream& operator<<(std::wostream& os, UseDatabaseStatement const& ss) {
				return os << "USE DATABASE " << ss.dbName;
			}
		};

		/****************************************
		*
		* SQL AST enter
		*
		****************************************/
		typedef
			boost::variant<UpdateStatement, UseDatabaseStatement, SelectStatement, CreateTableStatement, CreateDatabaseStatement, DeleteStatement, InsertStatement> sqlStatement;

		struct TopSQLStatement {
			sqlStatement sql;
		};

		class SQLparser : public boost::static_visitor<> {
		public:
			void operator()(SelectStatement & i) const;
			void operator()(CreateTableStatement & i) const;
			void operator()(CreateDatabaseStatement & i) const;
			void operator()(DeleteStatement & i) const;
			void operator()(InsertStatement & i) const;
			void operator()(UseDatabaseStatement & i) const;
			void operator()(UpdateStatement & i) const;
		};

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
				if (cs.columns.size() == 0) {
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
					else {
						s.push_back(L"NULL");
						s.push_back(L"NULL");
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
				std::wcout << L"成功创建一张名为" << formName << L"的表。" << std::endl;
				return;
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
				std::vector<std::wstring> dataset;
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
							if (tmp[j] > L'9' || tmp[j] < L'0') {
								if_int = false;
								break;
							}
						}
						if (if_int == true && fh[i][1] == L"char") {
							std::wcout << L"插入了错误的数据类型，" << fh[i][0] << L"是char型。" << std::endl;
							if_TypeError = true;
						}
						else if (if_int == false && fh[i][1] == L"int") {
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
						if (fh[i][1] == L"char") {
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
					/*bool if_PropertyError = false;
					for (unsigned int i = 0; i < data.size(); i++) {
						if(fh[i][2] == L"true")
					}*/

					dataset = data;
				}
				//可插入部分字段
				else {
					if (data.size() != definition.size()) {
						std::wcout << "错误！参数数量不匹配。" << std::endl;
						return;
					}

					//只找出第一个不存在的字段
					for (unsigned int i = 0; i < definition.size(); i++) {
						bool if_exist = false;
						for (unsigned int j = 0; j < fh.size(); j++) {
							if (fh[j][0] == definition[i]) {
								if_exist = true;
								break;
							}
						}
						if (!if_exist) {
							std::wcout << "错误！字段" << definition[i] << "不存在。" << std::endl;
							return;
						}
					}

					bool if_TypeError = false;
					for (unsigned int i = 0; i < data.size(); i++) {
						bool if_int = true;
						std::wstring tmp = data[i];
						for (unsigned int j = 0; j < tmp.length(); j++) {
							if (tmp[j] > L'9' || tmp[j] < L'0') {
								if_int = false;
								break;
							}
						}
						for (unsigned int j = 0; j < fh.size(); j++) {
							if (fh[j][0] == definition[i]) {
								if (fh[j][1] == L"int" && if_int == false) {
									std::wcout << L"插入了错误的数据类型，" << fh[i][0] << L"是int型。" << std::endl;
									if_TypeError = true;
								}
								else if (fh[j][1] == L"char" && if_int == true) {
									std::wcout << L"插入了错误的数据类型，" << fh[i][0] << L"是char型。" << std::endl;
									if_TypeError = true;
								}
							}
						}
					}
					if (if_TypeError) {
						return;
					}

					//进行长度限制检查
					bool if_LengthError = false;
					for (unsigned int i = 0; i < data.size(); i++) {
						for (unsigned int j = 0; j < fh.size(); j++) {
							if (fh[j][0] == definition[i]) {
								if (fh[i][1] == L"char") {
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
						
					  }
					}
					if (if_LengthError) {
						return;
					}
					for (unsigned int i = 0; i < fh.size(); i++) {
						bool if_occupied = false;
						for (unsigned j = 0; j < definition.size(); j++) {
							if (definition[j] == fh[i][0]) {
								if_occupied = true;
								dataset.push_back(data[j]);
								break;
							}
						}
						if (!if_occupied) {
							dataset.push_back(L"");
						}

					}

				}
				
				
				formSet[foundform].Insert(dataset);
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
				if (foundForm == -1) {
					std::wcout << L"错误！没有所要查询的表。" << std::endl;
					return;
				}

				//select全表
				if (ss.columns.size() == 1 && ss.columns[0] == L"*") {
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

		DataBase *db = new DataBase(L"naiveDB");

	}
}
/****************************************
*
* AST visition functions
*
****************************************/

void naiveDB::parser::SQLparser::operator()(SelectStatement & i) const {
	//std::wcout << i << std::endl;
	db->Select(i);
}

void naiveDB::parser::SQLparser::operator()(CreateTableStatement & i) const {
	//std::wcout << i << std::endl;
	db->Create(i);
}

void naiveDB::parser::SQLparser::operator()(DeleteStatement & i) const {
	db->Delete(i);
}
void naiveDB::parser::SQLparser::operator()(InsertStatement & i) const {
	//std::wcout << i << std::endl;
	db->Insert(i);
}

void naiveDB::parser::SQLparser::operator()(CreateDatabaseStatement & i) const {
	//std::wcout << i << std::endl;
	//std::wstring dbName = i.


}
void naiveDB::parser::SQLparser::operator()(UseDatabaseStatement & i) const {
	//std::wcout << i << std::endl;
	//std::wcout << L"当前正在操作数据库" << 
}

void naiveDB::parser::SQLparser::operator()(UpdateStatement & i) const {
	//std::wcout << L"a";
	db->Update(i);
}

/****************************************
*
* Use BOOST_FUSION to adapt AST to grammar structs
*
****************************************/

BOOST_FUSION_ADAPT_STRUCT(
	naiveDB::parser::SelectStatement,
	(std::vector<std::wstring>, columns)
	(std::vector<std::wstring>, fromtables)
	(naiveDB::parser::WhereStatement, whereClause)
)

BOOST_FUSION_ADAPT_STRUCT(
	naiveDB::parser::WhereStatement,
	(std::vector<naiveDB::parser::WhereConditionStatement>, statements)
)

BOOST_FUSION_ADAPT_STRUCT(
	naiveDB::parser::WhereConditionStatement,
	(std::wstring, lh)
	(std::wstring, op)
	(std::wstring, rh)
)

BOOST_FUSION_ADAPT_STRUCT(
	naiveDB::parser::TopSQLStatement,
	(naiveDB::parser::sqlStatement, sql)
)

BOOST_FUSION_ADAPT_STRUCT(
	naiveDB::parser::CreateTableStatement,
	(std::wstring, tableName)
	(std::vector<naiveDB::parser::ColumnStatement>, columns)
)

BOOST_FUSION_ADAPT_STRUCT(
	naiveDB::parser::ColumnStatement,
	(std::wstring, name)
	(std::wstring, dataType)
	(int, limit)
	(std::wstring, attribute)
)

BOOST_FUSION_ADAPT_STRUCT(
	naiveDB::parser::InsertStatement,
	(std::wstring, tableName)
	(std::vector<std::wstring>, columns)
	(std::vector<std::wstring>, values)
)


BOOST_FUSION_ADAPT_STRUCT(
	naiveDB::parser::DeleteStatement,
	(std::wstring, tableName)
	(naiveDB::parser::WhereStatement, whereClause)
)

BOOST_FUSION_ADAPT_STRUCT(
	naiveDB::parser::CreateDatabaseStatement,
	(std::wstring, dbName)
)

BOOST_FUSION_ADAPT_STRUCT(
	naiveDB::parser::UseDatabaseStatement,
	(std::wstring, dbName)
)

BOOST_FUSION_ADAPT_STRUCT(
	naiveDB::parser::UpdateStatement,
	(std::wstring, tableName)
	(std::vector<naiveDB::parser::SetStatement>, sets)
	(naiveDB::parser::WhereStatement, whereClause)
)

BOOST_FUSION_ADAPT_STRUCT(
	naiveDB::parser::SetStatement,
	(std::wstring, col)
	(std::wstring, val)
)