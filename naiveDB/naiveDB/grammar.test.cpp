#pragma once
#include "stdafx.h"
#include "Grammar.hpp"
#include "Executor.hpp"
void grammarTest() {
	using boost::spirit::standard_wide::space;
	typedef std::wstring::const_iterator iterator_type;
	typedef naiveDB::parser::SQLRule<iterator_type> SQLRule;
	SQLRule g; // Our grammar
	std::wstring str;
	std::wstring logo = L"\
              _           _____  ____  \n\
             (_)         |  __ \\|  _ \\ \n\
  _ __   __ _ ___   _____| |  | | |_) |\n\
 | '_ \\ / _` | \\ \\ / / _ \\ |  | |  _ < \n\
 | | | | (_| | |\\ V /  __/ |__| | |_) |\n\
 |_| |_|\\__,_|_| \\_/ \\___|_____/|____/ \n\
Welcome to naiveDB!";
	std::wcout << logo << std::endl;
	while (getline(std::wcin, str)) {
		if (str.empty() || str[0] == 'q' || str[0] == 'Q') {
			break;
		}
		naiveDB::parser::TopSQLStatement ste;
		std::wstring::const_iterator iter = str.begin();
		std::wstring::const_iterator end = str.end();
		bool r = phrase_parse(iter, end, g, space, ste);

		if (r && iter == end) {

			std::wcout << "Parsing succeeded\n";
			boost::apply_visitor(naiveDB::executor::SQLparser(), ste.sql);
		}
		else {
			std::wcout << "Parsing failed\n";
			std::wstring rest(iter, end);
			std::wcout << "stopped at: " << rest << std::endl;
		}

	}
}