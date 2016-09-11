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
------------------naiveDB������Ϣ------------------\n\
- ��ʽ�涨��\n\
���ֲ���Ҫ�����ţ� �ַ�����˫���� \"\"\n\
- �������﷨��\n\
CREATE TABLE ������(�ֶ�1 ��������,�ֶ�2 ��������,....)\n\
- ��ѯ�﷨��\n\
SELECT ������ FROM ������ WHERE ���� = ָ��ֵ\n\
- �����﷨��\n\
UPDATE ������ SET ������ = ��ֵ WHERE ������ = ĳֵ\n\
- ɾ���﷨��\n\
DELETE FROM ������ WHERE ������ = ֵ\n\
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