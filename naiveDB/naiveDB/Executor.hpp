#pragma once
#include "DataBase.hpp"
namespace naiveDB {
	namespace executor {
		using namespace naiveDB::parser;
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
		naiveDB::DataBase *db = new DataBase(L"naiveDB");
		
	}
}
/****************************************
*
* AST visition functions
*
****************************************/

void naiveDB::executor::SQLparser::operator()(SelectStatement & i) const {
	//std::wcout << i << std::endl;
	db->Select(i);
}

void naiveDB::executor::SQLparser::operator()(CreateTableStatement & i) const {
	//std::wcout << i << std::endl;
	db->Create(i);
}

void naiveDB::executor::SQLparser::operator()(DeleteStatement & i) const {
	db->Delete(i);
}
void naiveDB::executor::SQLparser::operator()(InsertStatement & i) const {
	//std::wcout << i << std::endl;
	db->Insert(i);
}

void naiveDB::executor::SQLparser::operator()(CreateDatabaseStatement & i) const {
	//std::wcout << i << std::endl;
	//std::wstring dbName = i.


}
void naiveDB::executor::SQLparser::operator()(UseDatabaseStatement & i) const {
	//std::wcout << i << std::endl;
	//std::wcout << L"当前正在操作数据库" << 
}

void naiveDB::executor::SQLparser::operator()(UpdateStatement & i) const {
	std::wcout << L"a";
	i;
}