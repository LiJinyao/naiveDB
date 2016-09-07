/****************************************
*
* SQL AST
*
****************************************/
#define BOOST_SPIRIT_UNICODE
#include "stdafx.h"
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
			std::vector<std::wstring> columns, fromtables;
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

		struct UpdateStatement {
			std::wstring tableName;
			std::vector<std::wstring> columns, values;
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
			boost::variant<UseDatabaseStatement, SelectStatement, CreateTableStatement, CreateDatabaseStatement, DeleteStatement, InsertStatement> sqlStatement;

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
		};
	}
}

/****************************************
*
* AST visition functions
*
****************************************/

void naiveDB::parser::SQLparser::operator()(SelectStatement & i) const {
	std::wcout << i << std::endl;
}

void naiveDB::parser::SQLparser::operator()(CreateTableStatement & i) const {
	std::wcout << i << std::endl;
}

void naiveDB::parser::SQLparser::operator()(DeleteStatement & i) const {
	std::wcout << i << std::endl;
}

void naiveDB::parser::SQLparser::operator()(InsertStatement & i) const {
	std::wcout << i << std::endl;
}

void naiveDB::parser::SQLparser::operator()(CreateDatabaseStatement & i) const {
	std::wcout << i << std::endl;
}
void naiveDB::parser::SQLparser::operator()(UseDatabaseStatement & i) const {
	std::wcout << i << std::endl;
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