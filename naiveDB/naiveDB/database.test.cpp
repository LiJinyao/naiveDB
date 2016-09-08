#include "stdafx.h"
#include "Form.hpp"
using namespace naiveDB::dataprocessor;
void databaseTest() {
	std::vector<std::vector<std::wstring>> fd;
	std::vector<std::wstring> s;

	s.push_back(L"name");
	s.push_back(L"char");
	s.push_back(L"false");
	s.push_back(L"true");
	s.push_back(L"5");

	fd.push_back(s);
	s.clear();

	s.push_back(L"id");
	s.push_back(L"int");
	s.push_back(L"true");
	s.push_back(L"true");
	s.push_back(L"0");

	fd.push_back(s);

	Form *f = new Form(fd, L"students");


	char cmd[10];
	std::wstring x;
	std::wstring y;
	std::wstring z = L"OR";
	while (std::cin >> cmd)
	{
		std::vector<std::wstring> dat;
		std::vector<std::vector<std::wstring>> dat1;
		std::vector<std::vector<std::wstring>> dat2;

		if (cmd[0] == 'i')
		{
			std::wcin >> x >> y;
			dat.push_back(x);
			dat.push_back(y);
			f->Insert(dat);
		}
		if (cmd[0] == 'd')
		{
			//f->Delete();
			std::wstring x1, y1;
			std::wcin >> x >> y;
			dat.push_back(x);
			dat.push_back(y);
			dat1.push_back(dat);
			dat.clear();
			std::wcin >> x1 >> y1;
			dat.push_back(x1);
			dat.push_back(y1);
			dat1.push_back(dat);
			std::cout<< f->Delete(dat1, z)<<std::endl;
		}
		if (cmd[0] == 'u')
		{
			std::wstring x1, x2,x3,x4, y1, y2,y3,y4;
			std::wcin >> x1 >> x2 >>x3>>x4>> y1 >> y2>>y3>>y4;
			dat.push_back(x1);
			dat.push_back(x2);
			dat1.push_back(dat);
			dat.clear();
			dat.push_back(x3);
			dat.push_back(x4);
			dat1.push_back(dat);
			dat.clear();
			dat.push_back(y1);
			dat.push_back(y2);
			dat2.push_back(dat);
			dat.clear();
			dat.push_back(y3);
			dat.push_back(y4);
			dat2.push_back(dat);
			std::cout << f->Update(dat1, dat2) << std::endl;
		}
		if (cmd[0] == 's')
		{
			f->Select();
			std::wstring s1 = L"name", s2 = L"id";
			std::wcin >> x >> y;
			dat.push_back(s1);
			dat.push_back(s2);
			std::wstring condition[2];
			condition[0] = x; condition[1] = y;
			f->Select(dat,condition);
		}

	}
}