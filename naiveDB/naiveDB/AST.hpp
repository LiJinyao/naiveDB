#pragma once
/****************************************
*
* SQL AST
*
****************************************/
#define BOOST_SPIRIT_UNICODE
#include "stdafx.h"
#include "DataBase.hpp"
#include <boost/algorithm/string.hpp> 
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
			std::vector<std::wstring> columns, fromtables; //columns中只有一个元素且这个元素为L"*"时代表SELECT *
			WhereStatement whereClause;
			std::wstring orderBy; // 排序的字段
			std::wstring order; //排序顺序，可能为: ""（未指定）, "DESC"（降序）, "ASC"（升序）；

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
			std::wstring attribute; // NOT NULL, PRIMARY KEY
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
		* DROP statement
		*
		****************************************/
		struct DropStatement {
			std::wstring table;
		};

		struct ShowStatement {
			std::wstring showWaht;
		};
		/****************************************
		*
		* SQL AST enter
		*
		****************************************/
		typedef
			boost::variant<UpdateStatement, ShowStatement, DropStatement, UseDatabaseStatement, SelectStatement, CreateTableStatement, CreateDatabaseStatement, DeleteStatement, InsertStatement> sqlStatement;

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
			void operator()(DropStatement & i) const;
			void operator()(ShowStatement & i) const;
		};
		
		
		std::vector<DataBase*> dataBaseSet;
		DataBase *pointer;

		void buildFolder() {
			std::wstring folderName = L".\\naiveDB";
			std::wstring command = L"md ";
			command = command + folderName;
			if (_waccess(folderName.data(), 0) == -1) {
				_wsystem(command.data());
			}
			else {
				std::wcout << L"根目录已存在" << std::endl;
			}
			return;
		}

		std::vector<std::wstring> loadDataBaseName() {
			std::wstring fileName = L".\\naiveDB\\dataBaseNameList.dat";
			std::vector<std::wstring> v;
			if (_waccess(fileName.data(), 0) == -1) {
				return v;
			}
			std::ifstream fin(fileName, std::ios::binary);
			boost::archive::binary_iarchive ia(fin);
			int size;
			ia >> size;
			std::wstring wstr;
			for (int i = 0; i < size; i++) {
				ia >> wstr;
				v.push_back(wstr);
			}
			return v;
		}
	
		 void Initialize() {
			 std::vector<std::wstring> dbNames = loadDataBaseName();
			for (unsigned int i = 0; i < dbNames.size(); i++) {
				DataBase *db = new DataBase(dbNames[i], i);
				std::vector<std::wstring> formNameList = db->loadFormName();
				//对每一个表进行还原
				for (unsigned int j = 0; j < formNameList.size(); j++) {
					naiveDB::dataprocessor::Form f1 = db->loadForm(formNameList[j]);
					std::vector<std::vector<std::wstring>> hd = f1.getFormHeader();
					naiveDB::dataprocessor::Form f2 = naiveDB::dataprocessor::Form(hd, formNameList[j]);
					std::map<int, naiveDB::dataprocessor::Record> recordsCopy = f1.getForm();
					

					for (unsigned int k = 0; k < recordsCopy.size(); k++) {
						std::vector<std::wstring> dataset;
						std::vector<naiveDB::dataprocessor::Key*> r = recordsCopy[k].getRecord();
						for (unsigned int l = 0; l < r.size(); l++) {
							if (r[l]->getTypeName() == L"char") {
								naiveDB::dataprocessor::StringKey *p = (naiveDB::dataprocessor::StringKey*)r[l];
								dataset.push_back(p->getData());
							}
							else if (r[l]->getTypeName() == L"int") {
								naiveDB::dataprocessor::IntKey *p = (naiveDB::dataprocessor::IntKey*)r[l];
								std::wstringstream ss;
								int num = p->getData();
								std::wstring str;
								ss << num;
								ss >> str;
								dataset.push_back(str);
							}
							else if (r[l]->getTypeName() == L"bool") {
								naiveDB::dataprocessor::StringKey *p = (naiveDB::dataprocessor::StringKey*)r[l];
								dataset.push_back(p->getData());
							}
							else if (r[l]->getTypeName() == L"date") {
								naiveDB::dataprocessor::StringKey *p = (naiveDB::dataprocessor::StringKey*)r[l];
								dataset.push_back(p->getData());
							}
						}
						if (dataset.size() > 0) {
							f2.Insert(dataset);
						}
					}
					db->addForm(f2);
				}
				dataBaseSet.push_back(db);
			}
			return;
		}
		
		

	}
}
/****************************************
*
* AST visition functions
*
****************************************/

void naiveDB::parser::SQLparser::operator()(SelectStatement & i) const {
	if (pointer == NULL) {
		std::wcout << L"错误！请先声明所要操作的数据库。" << std::endl;
	}
	else {
		std::vector<std::wstring> columns = i.columns;
		std::vector<std::wstring> fromtables = i.fromtables;
		std::wstring order = i.order; //排序顺序，可能为: ""（未指定）, "DESC"（降序）, "ASC"（升序）；
		boost::algorithm::to_upper(order);
		std::vector<std::vector<std::wstring>> whereClause;
		for (unsigned int j = 0; j < i.whereClause.statements.size(); j++) {
			std::vector<std::wstring> s;
			s.push_back(i.whereClause.statements[j].lh);
			s.push_back(i.whereClause.statements[j].rh);
			s.push_back(i.whereClause.statements[j].op);
			whereClause.push_back(s);
			s.clear();
		}

		std::wstring orderBy = i.orderBy;

		pointer->Select(columns, fromtables, whereClause, orderBy, order);
	}
}

void naiveDB::parser::SQLparser::operator()(CreateTableStatement & i) const {
	if (pointer == NULL) {
		std::wcout << L"错误！请先声明所要操作的数据库。" << std::endl;
	}
	else {
		CreateTableStatement cs = i;
		std::wstring formName = cs.tableName; //参数2
		std::vector<std::vector<std::wstring>> formDefine;
		std::vector<ColumnStatement> columnDefine = cs.columns;

		for (unsigned int i = 0; i < columnDefine.size(); i++) {
			std::vector<std::wstring> s;
			s.push_back(columnDefine[i].name);
			s.push_back(columnDefine[i].dataType);
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
			std::wstringstream ss;
			ss << columnDefine[i].limit;
			std::wstring wstr;
			ss >> wstr;
			s.push_back(wstr);

			formDefine.push_back(s);
			s.clear();
		}

		pointer->Create(formDefine, formName);

	}
}

void naiveDB::parser::SQLparser::operator()(DeleteStatement & i) const {
	if (pointer == NULL) {
		std::wcout << L"错误！请先声明所要操作的数据库。" << std::endl;
	}
	else {
		std::wstring tableName = i.tableName;
		std::vector<std::vector<std::wstring>> whereClause;
		for (unsigned int j = 0; j < i.whereClause.statements.size(); j++) {
			std::vector<std::wstring> s;
			s.push_back(i.whereClause.statements[j].lh);
			s.push_back(i.whereClause.statements[j].rh);
			s.push_back(i.whereClause.statements[j].op);
			whereClause.push_back(s);
			s.clear();
		}
		pointer->Delete(tableName, whereClause);
	}
	
}

void naiveDB::parser::SQLparser::operator()(InsertStatement & i) const {
	if (pointer == NULL) {
		std::wcout << L"错误！请先声明所要操作的数据库。" << std::endl;
	}
	else {
		std::wstring formName = i.tableName;
		std::vector<std::wstring> data = i.values;
		std::vector<std::wstring> definition = i.columns;

		pointer->Insert(formName, data, definition);
	}
	

}

void naiveDB::parser::SQLparser::operator()(CreateDatabaseStatement & i) const {
    std::wstring dbname = i.dbName;
	for (unsigned int i = 0; i < dataBaseSet.size(); i++) {
		if (dataBaseSet[i]->getDBName() == dbname) {
			std::wcout << L"该数据库已存在。" << std::endl;
			return;
		}
	}
	DataBase *db = new DataBase(dbname);
	pointer = db;
	dataBaseSet.push_back(db);
	//saveDataBaseName
	std::wstring fileName = L".\\naiveDB\\dataBaseNameList.dat";
	std::ofstream fout(fileName, std::ios::binary);
	boost::archive::binary_oarchive oa(fout);
	std::vector<DataBase*>::iterator it;
	oa << dataBaseSet.size();
	for (it = dataBaseSet.begin(); it != dataBaseSet.end(); it++) {
		oa << (*it)->getDBName();
	}
	fout.close();
	std::wcout << L"成功创建名为" << dbname << L"的数据库。" << std::endl;
}

void naiveDB::parser::SQLparser::operator()(UseDatabaseStatement & i) const {
	std::wstring  dbName = i.dbName;
	bool ifFound = false;
	for (unsigned i = 0; i < dataBaseSet.size(); i++) {
		if (dataBaseSet[i]->getDBName() == dbName) {
			pointer = dataBaseSet[i];
			ifFound = true;
			std::wcout << L"已切换到数据库" << dataBaseSet[i]->getDBName() << std::endl;
			break;
		}
	}
	if (!ifFound) {
		std::wcout << L"该数据库不存在。" << std::endl;
	}

}

void naiveDB::parser::SQLparser::operator()(UpdateStatement & i) const {
	if (pointer == NULL) {
		std::wcout << L"错误！请先声明所要操作的数据库。" << std::endl;
	}
	else {
		std::wstring tableName = i.tableName;
		std::vector<std::vector<std::wstring>> sets;
		std::vector<std::vector<std::wstring>> whereClause;

		for (unsigned int j = 0; j < i.sets.size(); j++) {
			std::vector<std::wstring> s;
			s.push_back(i.sets[j].col);
			s.push_back(i.sets[j].val);
			sets.push_back(s);
			s.clear();
		}

		for (unsigned int j = 0; j < i.whereClause.statements.size(); j++) {
			std::vector<std::wstring> s;
			s.push_back(i.whereClause.statements[j].lh);
			s.push_back(i.whereClause.statements[j].rh);
			s.push_back(i.whereClause.statements[j].op);
			whereClause.push_back(s);
			s.clear();
		}
		pointer->Update(tableName, sets, whereClause);
	}
}

void naiveDB::parser::SQLparser::operator()(DropStatement & i) const {
	//std::wcout << i.table;
	if (pointer == NULL) {
		std::wcout << L"错误！请先声明所要操作的数据库。" << std::endl;
	}
	else {
		pointer->Drop(i.table);
	}
}

void naiveDB::parser::SQLparser::operator()(ShowStatement & i) const {
	std::wstring showWhat = i.showWaht;
	boost::algorithm::to_upper(showWhat);
	if (showWhat == L"DATABASES") {
		std::wstring fileName = L".\\naiveDB\\dataBaseNameList.dat";
		if (_waccess(fileName.data(), 0) == -1) {
			std::wcout << L"无预先保存的数据库" << std::endl;
		}
		else {
			std::ifstream fin(fileName, std::ios::binary);
			boost::archive::binary_iarchive ia(fin);
			int size;
			ia >> size;
			if (size == 0) {
				std::wcout << L"无预先保存的数据库" << std::endl;
			}
			else {
				std::wstring wstr;
				std::wcout << L"共存在" << size << L"个数据库" << std::endl;
				for (int i = 0; i < size; i++) {
					ia >> wstr;
					std::wcout << wstr << std::endl;
				}
			}
		}

	}
	else if (showWhat == L"TABLES") {
		if (pointer == NULL) {
			std::wcout << L"错误！请先声明所要操作的数据库。" << std::endl;
		}
		else {
			pointer->showTables();
		}
	}
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
	(std::wstring, orderBy)
	(std::wstring, order)
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

BOOST_FUSION_ADAPT_STRUCT(
	naiveDB::parser::DropStatement,
	(std::wstring, table)
)

BOOST_FUSION_ADAPT_STRUCT(
	naiveDB::parser::ShowStatement,
	(std::wstring, showWaht)
)