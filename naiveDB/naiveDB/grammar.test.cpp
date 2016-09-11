#pragma once
#include "stdafx.h"
#include "Grammar.hpp"

void  naiveDB::parser::Initialize();

void grammarTest() {
	using boost::spirit::standard_wide::space;
	typedef std::wstring::const_iterator iterator_type;
	typedef naiveDB::parser::SQLRule<iterator_type> SQLRule;
	SQLRule g; // Our grammar
	std::wstring str;
	std::wstring logo = L"\n\
              _           _____  ____  \n\
             (_)         |  __ \\|  _ \\ \n\
  _ __   __ _ ___   _____| |  | | |_) |\n\
 | '_ \\ / _` | \\ \\ / / _ \\ |  | |  _ < \n\
 | | | | (_| | |\\ V /  __/ |__| | |_) |\n\
 |_| |_|\\__,_|_| \\_/ \\___|_____/|____/ \n\n\
Welcome to naiveDB!";
	std::locale loc("chs");
	std::wstring help = L"\
------------------naiveDB帮助信息------------------\n\
- 格式规定：\n\
数字不需要加引号， 字符串用双引号 \"\"\n\
- 创建表语法：\n\
CREATE TABLE 表名称(字段1 数据类型,字段2 数据类型,....)\n\
- 查询语法：\n\
SELECT 列名称 FROM 表名称 WHERE 条件 = 指定值\n\
- 更新语法：\n\
UPDATE 表名称 SET 列名称 = 新值 WHERE 列名称 = 某值\n\
- 删除语法：\n\
DELETE FROM 表名称 WHERE 列名称 = 值\n\
		";
	std::wcin.imbue(loc);
	std::wcout.imbue(loc);
	std::wcout << logo << std::endl;
	std::wcout << help << std::endl;

	naiveDB::parser::Initialize();


	while (getline(std::wcin, str)) {
		if (str.empty() || str[0] == 'q' || str[0] == 'Q') {
			break;
		}
		naiveDB::parser::TopSQLStatement ste;
		std::wstring::const_iterator iter = str.begin();
		std::wstring::const_iterator end = str.end();
		bool r = phrase_parse(iter, end, g, space, ste);

		if (r && iter == end) {

			//std::wcout << "Parsing succeeded\n";
			boost::apply_visitor(naiveDB::parser::SQLparser(), ste.sql);
		}
		else {
			std::wcout << "Parsing failed\n";
			std::wstring rest(iter, end);
			std::wcout << "stopped at: " << rest << std::endl;
		}

	}
}