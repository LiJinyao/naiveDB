#define BOOST_SPIRIT_UNICODE
#include "stdafx.h"
#include "AST.hpp"
namespace naiveDB {
	namespace parser {
		namespace qi = boost::spirit::qi;
		namespace encoding = boost::spirit::standard_wide;
		namespace phoenix = boost::phoenix;

		// qi using
		using qi::lit;
		using qi::lexeme;
		using encoding::char_;
		using qi::int_;
		using std::wstring;
		using qi::alpha;
		using qi::alnum;
		using qi::space;
		using qi::no_case;
		using qi::optional;
		using qi::attr;
		using qi::on_error;
		using qi::fail;
		using boost::spirit::qi::symbols;
		using qi::digit;
		// top rule
		template <typename Iterator> struct SQLRule;
		// Columns attribute statement rule
		template <typename Iterator> struct ColumnsClause;
		/****************************************
		*
		* command rules
		*
		****************************************/
		template <typename Iterator> struct SelectRule;
		template <typename Iterator> struct CreateTableRule;
		template <typename Iterator> struct CreateDatabaseRule;
		template <typename Iterator> struct DeletetRule;
		template <typename Iterator> struct InsertRule;
		template <typename Iterator> struct UseDatabasetRule;
		// TODO
		template <typename Iterator> struct UpdateRule;


		/****************************************
		*
		* keyword rules
		*
		****************************************/
		// where
		template <typename Iterator> struct WhereClause;
		template <typename Iterator> struct WhereConditionClause;

		// dataType statement
		template <typename Iterator> struct DataTypeDef;
		template <typename Iterator> struct DataTypeLimit;

		//select symble
		symbols<wchar_t, std::wstring> selectedColumns;


	}
}



// top SQL rule
template <typename Iterator>
struct naiveDB::parser::SQLRule : qi::grammar<Iterator, TopSQLStatement(), encoding::space_type> {
	SQLRule() : SQLRule::base_type(start) {
		using qi::lit;
		using qi::optional;

		start %= selectRule | createRule | deleteRule | insertRule | createDatabaseRule | useDatabasetRule;
	}
	qi::rule<Iterator, TopSQLStatement(), encoding::space_type> start;
	SelectRule<Iterator> selectRule;
	CreateTableRule<Iterator> createRule;
	DeletetRule<Iterator> deleteRule;
	InsertRule<Iterator> insertRule;
	CreateDatabaseRule<Iterator> createDatabaseRule;
	UseDatabasetRule<Iterator> useDatabasetRule;
};

// Where condition rule
template <typename Iterator>
struct naiveDB::parser::WhereConditionClause : qi::grammar<Iterator, WhereConditionStatement(), encoding::space_type> {
	WhereConditionClause() : WhereConditionClause::base_type(start) {

		quotedString %= lexeme['"' >> +(char_ - (lit('"') | ';' | ',')) >> '"'];

		whereOperator %= lexeme[encoding::string(L">=") | encoding::string(L"<=") | encoding::string(L"!=") | char_("[=][>][>][<][<]")];
		columnsident %= lexeme[alpha >> *alnum];
		// columnsident %= selectedColumns;
		// optional quotate string
		condition %= quotedString | lexeme[+(char_ - (encoding::space | lit('"') | ';' | ',' ))];
		// start %= columnsident > whereOperator> condition;
		start %= columnsident >> whereOperator >> condition;
	}
	qi::rule<Iterator, std::wstring(), encoding::space_type> columnsident, condition, whereOperator, quotedString;
	//qi::rule<Iterator, void(std::wstring), encoding::space_type> ;
	qi::rule<Iterator, WhereConditionStatement(), encoding::space_type> start;
};

// Where clause rule
template <typename Iterator>
struct naiveDB::parser::WhereClause : qi::grammar<Iterator, WhereStatement(), encoding::space_type> {
	WhereClause() : WhereClause::base_type(start) {

		start %= no_case["where"] >> whereCondition % (no_case["and"] | no_case["or"])>> -lit(';');

	}
	qi::rule<Iterator, std::wstring(), encoding::space_type> columnsident, whereOperator, condition;
	qi::rule<Iterator, WhereStatement(), encoding::space_type> start;
	WhereConditionClause<Iterator> whereCondition;
};

void addColSym(std::wstring str) {
	naiveDB::parser::selectedColumns.add(str, str);
}

// Select statement rule
// SELECT 列名称 FROM 表名称;
template <typename Iterator>
struct naiveDB::parser::SelectRule : qi::grammar<Iterator, SelectStatement(), encoding::space_type> {
	SelectRule() : SelectRule::base_type(start) {

		tablesident %= lexeme[alpha >> *alnum]; // table name
		columnsident %= lexeme[(alpha | '*') >> *alnum]; // columns name could be *
		columns %= no_case["select"] >> (columnsident[&addColSym] % ',');
		tables %= no_case["from"] >> (tablesident % ',');

		start %= columns >> tables
			>> -whereStatement;
	}

	qi::rule<Iterator, std::wstring(), encoding::space_type> tablesident, columnsident;
	qi::rule<Iterator, std::vector<std::wstring>(), encoding::space_type> columns, tables;
	qi::rule<Iterator, SelectStatement(), encoding::space_type> start;
	WhereClause<Iterator> whereStatement;
};

// column attribute statement rule
template <typename Iterator>
struct naiveDB::parser::ColumnsClause : qi::grammar<Iterator, ColumnStatement(), encoding::space_type> {
	ColumnsClause() : ColumnsClause::base_type(start, "ColumsClause") {
		dataType %= no_case[encoding::string("int")] | no_case[encoding::string("char")];
		dataAtrribute %= no_case[encoding::string("not null")] | no_case[encoding::string("primary key")];
		name %= lexeme[alpha >> *alnum];
		/**
		* optional attribute with BOOST_FUSION_ADAPT_STRUCT
		* http://stackoverflow.com/questions/36165747/boostspirit-parsing-into-a-fusion-adapted-structure-optional-but-exclusive
		* * will not work in the BOOST_FUSION_ADAPT_STRUCT.
		* use int_ | attr(0)
		*/
		start %= name >> dataType >> ('(' >> int_ >> ')' | attr(0)) >> *dataAtrribute;
	}
	qi::rule<Iterator, std::wstring(), encoding::space_type> dataType, dataAtrribute, name;
	qi::rule<Iterator, ColumnStatement(), encoding::space_type> start;

};

// Create statement rule 
// CREATE TABLE 表名称(列名称1 数据类型(limit),列名称2 数据类型,列名称3 数据类型)
template <typename Iterator>
struct naiveDB::parser::CreateTableRule : qi::grammar<Iterator, CreateTableStatement(), encoding::space_type> {
	CreateTableRule() : CreateTableRule::base_type(start, "CREATE TABLE") {

		using namespace qi::labels;
		using phoenix::construct;
		using phoenix::val;

		tablename %= lexeme[alpha >> *alnum];
		start %= no_case["create table"] > tablename >> lit('(') >> colum % ',' >> lit(')') >> -lit(';');
		tablename.name("表名格式错误");
		start.name("CREATE TABLE");
		on_error<fail>(
			start,
			std::cout
			<< val("Grammar Error: Expecting")
			<< _4
			<< val("at ")
			<< construct<std::string>(_3, _2)
			<< std::endl
			);
	}
	qi::rule<Iterator, std::wstring(), encoding::space_type> tablename;
	qi::rule<Iterator, CreateTableStatement(), encoding::space_type> start;
	ColumnsClause<Iterator> colum;
};

template <typename Iterator>
struct naiveDB::parser::CreateDatabaseRule : qi::grammar<Iterator, CreateDatabaseStatement(), encoding::space_type> {
	CreateDatabaseRule() : CreateDatabaseRule::base_type(start) {
		start %= no_case["create database"] >> lexeme[alpha >> *alnum] >> -lit(';');
	}
	qi::rule<Iterator, CreateDatabaseStatement(), encoding::space_type> start;
};



// Insert statement rule
// INSERT INTO table_name (列1, 列2,...) VALUES (值1, 值2,....)
// INSERT INTO 表名称 VALUES (值1, 值2,....)
template <typename Iterator>
struct naiveDB::parser::InsertRule : qi::grammar<Iterator, InsertStatement(), encoding::space_type> {
	InsertRule() : InsertRule::base_type(start) {
		tablename %= lexeme[alpha >> *alnum];
		quotedString %= lexeme[lit('"') >> +(char_ - (lit('"') | ';' | ',')) >> lit('"')];
		values %= quotedString | lexeme[digit >> *digit];
		start %= no_case["insert into"]
			>> tablename
			>> *('(' >> tablename % ',' >> ')')
			>> no_case["values"]
			>> '('
			>> values % ','
			>> ')'
			>> -lit(';');
	}
	qi::rule<Iterator, std::wstring(), encoding::space_type> tablename, quotedString, values;
	qi::rule<Iterator, InsertStatement(), encoding::space_type> start;
};

// Delete statement rule
// DELETE FROM 表名称 WHERE 列名称 = 值
template <typename Iterator>
struct naiveDB::parser::DeletetRule : qi::grammar<Iterator, DeleteStatement(), encoding::space_type> {
	DeletetRule() : DeletetRule::base_type(start) {
		tablename %= lexeme[alpha >> *alnum];
		start %= no_case["delete from"]
			>> tablename
			>> -whereStatement;
	}
	qi::rule<Iterator, std::wstring(), encoding::space_type> tablename;
	qi::rule<Iterator, DeleteStatement(), encoding::space_type> start;
	WhereClause<Iterator> whereStatement;
};

template <typename Iterator>
struct naiveDB::parser::UseDatabasetRule : qi::grammar<Iterator, UseDatabaseStatement(), encoding::space_type> {
	UseDatabasetRule() : UseDatabasetRule::base_type(start) {
		start %= no_case["use"] >> lexeme[alpha >> *alnum] >> -lit(';');
	}
	qi::rule<Iterator, UseDatabaseStatement(), encoding::space_type> start;
};