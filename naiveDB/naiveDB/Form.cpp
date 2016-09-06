#include "Form.h"
#include "stdafx.h" 

#include <sstream>
#include <iostream>
#include <iterator>
#include <iomanip>
#include <set>
using namespace naiveDB::dataprocessor;

//建表
Form::Form(std::vector<std::vector<std::wstring> > formDefine, std::wstring _formName) {
	idTotal = 0;
	keyTotal = formDefine.size();
	formName = _formName;
	formHeader = formDefine;

	for (int i = 0; i < formDefine.size(); i++) {
		if (formDefine[i][1] == L"int") {
			AVL<int> *tmp = new AVL<int>(formDefine[i][0], i);
			intHeader.push_back(tmp);
		}
		if (formDefine[i][1] == L"wstring") {
			AVL<std::wstring> *tmp = new AVL<std::wstring>(formDefine[i][0], i);
			wstringHeader.push_back(tmp);
		}
	}

}

Form::~Form() {
	delete &formName;
	delete &intHeader;
	delete &wstringHeader;
	delete &records;
	delete &formHeader;
}

void Form::Insert(std::vector<std::wstring> dataSet) {

	idTotal++;
	//建立可以传给Record的参数formDefine
	std::vector<std::vector<std::wstring>> formDefine;

	for (unsigned int i = 0; i < formHeader.size(); i++) {
		std::vector<std::wstring> s;
		s.push_back(formHeader[i][0]);
		s.push_back(formHeader[i][1]);
		s.push_back(formHeader[i][2]);
		s.push_back(formHeader[i][3]);
		s.push_back(formHeader[i][4]);
		s.push_back(dataSet[i]);
		formDefine.push_back(s);
	}

	//把新纪录插入map
	Record r = Record(idTotal, formDefine);
	records.insert(std::pair<int, Record>(idTotal, r));

	//在AVL中插入新纪录
	for (unsigned int i = 0; i < formDefine.size(); i++) {
		for (unsigned int j = 0; j < intHeader.size(); j++) {
			if (intHeader[j]->GetId() == i) {
				std::wstringstream ss;
				int num;
				ss << formDefine[i][5];
				ss >> num;
				intHeader[j]->insert(num, idTotal);
			}
		}
		for (unsigned int j = 0; j < wstringHeader.size(); j++) {
			if (wstringHeader[j]->GetId() == i) {
				wstringHeader[j]->insert(formDefine[i][5], idTotal);
			}
		}
	}
}

void Form::Delete()
{
	records.erase(records.begin(), records.end());
	idTotal = 0;
	ifNull = 0;
	for (unsigned int i = 0; i < wstringHeader.size(); i++)
	{
		wstringHeader[i]->clear();
	}
	for (int i = 0; i < intHeader.size(); i++)
	{
		intHeader[i]->clear();
	}
	std::cout << "make a empty form" << std::endl;					//打印空表 
}

int Form::Delete(std::vector<std::vector<std::wstring>> condition, std::wstring relation)
{
	std::vector<std::vector<int> > ans;
	for (int i = 0; i < condition.size(); i++)
	{
		for (int j = 0; j < wstringHeader.size(); j++)
		{

			if (wstringHeader[j]->GetName() == condition[i][0])
			{
				ans.push_back(wstringHeader[j]->finddata(condition[i][1]));
				//wstringHeader[j]->erase(condition[i][1]);
			}
		}
		for (int j = 0; j < intHeader.size(); j++)
		{
			std::wstringstream ss;
			ss << condition[i][1];
			int x;
			ss >> x;
			if (intHeader[j]->GetName() == condition[i][0])
			{
				ans.push_back(intHeader[j]->finddata(x));
				//intHeader[j]->erase(x);
			}
		}
	}
	for (int i = 0; i < ans.size(); i++)
	{
		sort(ans[i].begin(), ans[i].end());
	}
	std::vector<int>temp;

	std::wstring sa = L"AND";
	if (relation == sa)
	{
		for (int i = 0; i <ans.size(); i++)
		{
			temp.clear();
			std::set_intersection(ans[0].begin(), ans[0].end(), ans[i].begin(), ans[i].end(), back_inserter(temp));
			ans[0].clear();
			for (int j = 0; j < temp.size(); j++)
			{
				ans[0].push_back(temp[j]);
			}
		}
	}
	std::wstring sb = L"OR";
	if (relation == sb)
	{
		for (int i = 0; i <ans.size(); i++)
		{
			temp.clear();
			std::set_union(ans[0].begin(), ans[0].end(), ans[i].begin(), ans[i].end(), back_inserter(temp));
			ans[0].clear();
			for (int j = 0; j < temp.size(); j++)
			{
				ans[0].push_back(temp[j]);
			}
		}
	}
	std::map<int, Record>::iterator it;
	for (int i = 0; i < ans[0].size(); i++)
	{
		it = records.find(ans[0][i]);

		if (it != records.end())
		{
			for (int j = 0; j < intHeader.size(); j++)
			{
				IntKey* key = (IntKey*)it->second.findKey(intHeader[j]->GetName());
				{
					intHeader[j]->erasenew((key)->getData(), ans[0][i]);
				}

			}
			for (int j = 0; j < wstringHeader.size(); j++)
			{

				StringKey* key = (StringKey*)it->second.findKey(wstringHeader[j]->GetName());
				{
					wstringHeader[j]->erasenew((key)->getData(), ans[0][i]);
				}

			}	//删除树； 
			records.erase(it);
		}
	}
	return ans[0].size();
}

void Form::Select()
{
	std::map<int, Record>::iterator it;

	std::cout.setf(std::ios::left, std::ios::adjustfield);

	for (int i = 0; i < formHeader.size(); i++)
	{
		std::wcout << std::setw(8) << formHeader[i][0];
	}
	std::cout << std::endl;
	std::wstring sa = L"wstring";
	std::wstring sb = L"int";
	for (it = records.begin(); it != records.end(); it++)
	{
		std::vector<Key*> tmpp = it->second.getRecord();
		for (int j = 0; j < tmpp.size(); j++)
		{
			if (tmpp[j]->getTypeName() == sa)
			{
				StringKey* now = (StringKey*)tmpp[j];
				std::wcout << std::setw(8) << now->getData();
			}
			else
			{
				IntKey* now = (IntKey*)tmpp[j];
				std::wcout << std::setw(8) << now->getData();

			}
		}

		std::wcout << std::endl;
	}
}

void Form::Select(std::vector<std::wstring> keyNames)
{
	std::map<int, Record>::iterator it;
	std::cout.setf(std::ios::left, std::ios::adjustfield);
	for (int i = 0; i < keyNames.size(); i++)
	{
		std::wcout << std::setw(8) << keyNames[i];
	}
	std::cout << std::endl;
	std::wstring sa = L"wstring";
	std::wstring sb = L"int";
	for (it = records.begin(); it != records.end(); it++)
	{
		for (int j = 0; j < keyNames.size(); j++)
		{
			std::vector<Key*> tmpp = it->second.getRecord();
			for (int k = 0; k < tmpp.size(); k++)
			{
				if (tmpp[k]->getKeyName() == keyNames[j])
				{
					if (tmpp[k]->getTypeName() == sa)
					{
						StringKey* now = (StringKey*)tmpp[k];
						std::wcout << std::setw(8) << now->getData();
					}
					else
					{
						IntKey* now = (IntKey*)tmpp[k];
						std::wcout << std::setw(8) << now->getData();

					}
				}
			}
		}
		std::cout << std::endl;
	}
}

int Form::Select(std::vector<std::wstring> keyNames, std::wstring condition[2])
{
	std::map<int, Record>::iterator it;
	std::cout.setf(std::ios::left, std::ios::adjustfield);
	std::vector<int>tmp;
	std::wstring sa = L"wstring";
	std::wstring sb = L"int";
	for (int j = 0; j < wstringHeader.size(); j++)
	{
		if (wstringHeader[j]->GetName() == condition[0])
		{
			tmp = wstringHeader[j]->finddata(condition[1]);
		}
	}
	for (int j = 0; j < intHeader.size(); j++)
	{
		if (intHeader[j]->GetName() == condition[0])
		{
			std::wstringstream ss;
			ss << condition[1];
			int x;
			ss >> x;
			tmp = intHeader[j]->finddata(x);

		}
	}
	for (int i = 0; i < keyNames.size(); i++)
	{
		std::wcout << std::setw(8) << keyNames[i];
	}
	std::cout << std::endl;
	for (int i = 0; i < tmp.size(); i++)
	{

		for (int j = 0; j < keyNames.size(); j++)
		{

			std::vector<Key*> tmpp = it->second.getRecord();
			for (int k = 0; k < tmpp.size(); k++)
			{
				if (tmpp[k]->getKeyName() == keyNames[j])
				{
					if (tmpp[j]->getTypeName() == sa)
					{
						StringKey* now = (StringKey*)tmpp[j];
						std::wcout << std::setw(8) << now->getData();
					}
					else
					{
						IntKey* now = (IntKey*)tmp[j];
						std::wcout << std::setw(8) << now->getData();

					}
				}
			}
		}
		std::cout << std::endl;
	}
	return tmp.size();
}

int Form::Update(std::vector<std::vector<std::wstring>> set, std::vector<std::vector<std::wstring>> condition)
{
	std::set<int> ans;
	std::vector<int>tmp;
	std::set<int>temp;
	std::set<int>::iterator it;
	std::map<int, Record>::iterator itMap;
	for (int i = 0; i < condition.size(); i++)
	{
		for (int j = 0; j < wstringHeader.size(); j++)
		{
			if (wstringHeader[j]->GetName() == condition[i][0])
			{
				tmp = wstringHeader[j]->finddata(condition[i][1]);
			}
		}
		for (int j = 0; j < intHeader.size(); j++)
		{
			if (intHeader[j]->GetName() == condition[i][0])
			{
				std::wstringstream ss;
				ss << condition[i][1];
				int x;
				ss >> x;
				tmp = intHeader[j]->finddata(x);

			}
		}
		if (i == 0)
		{
			for (int j = 0; j < tmp.size(); j++) ans.insert(tmp[j]);
		}
		else
		{
			for (int j = 0; j < tmp.size(); j++)
			{
				if (ans.count(tmp[j]) != 0)
					temp.insert(tmp[j]);
			}
			ans.empty();
			for (it = temp.begin(); it != temp.end(); it++) ans.insert(*it);
		}
	}
	for (int i = 0; i < set.size(); i++)
	{
		for (int j = 0; j < wstringHeader.size(); j++)
		{
			if (wstringHeader[j]->GetName() == set[i][0])
			{
				for (it = ans.begin(); it != ans.end(); it++)
				{
					itMap = records.find(*it);
					std::vector<Key*> tmpp = itMap->second.getRecord();
					for (int k = 0; k < tmpp.size(); k++)
					{
						if (tmpp[k]->getKeyName() == set[i][0])
						{
							wstringHeader[j]->update(set[i][1], ((StringKey*)tmpp[k])->getData(), *it);
							((StringKey*)tmpp[k])->setData(set[i][1]);
						}
					}

				}
			}
		}
		for (int j = 0; j < intHeader.size(); j++)
		{
			if (intHeader[j]->GetName() == set[i][0])
			{
				for (it = ans.begin(); it != ans.end(); it++)
				{
					itMap = records.find(*it);
					std::vector<Key*> tmpp = itMap->second.getRecord();
					for (int k = 0; k < tmpp.size(); k++)
					{
						if (tmpp[k]->getKeyName() == set[i][0])
						{
							std::wstringstream ss;
							ss << set[i][1];
							int x;
							ss >> x;
							intHeader[j]->update(x, ((IntKey*)tmpp[k])->getData(), *it);
							((IntKey*)tmpp[k])->setData(x);

						}
					}

				}
			}
		}

	}
	return ans.size();
}


//获取记录集合
std::map<int, Record> Form::getForm() {
	return records;
}

//获取表名
std::wstring Form::getFormName() {
	return formName;
}