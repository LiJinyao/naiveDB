#pragma once
#include "stdafx.h"
#include "AVL.hpp"
#include "Record.hpp"

namespace naiveDB {
	namespace dataprocessor {
		class Form {
		public:
			Form() {

			}

			Form(std::vector<std::vector<std::wstring>> formDefine, std::wstring _formName) {
				idTotal = 0;
				keyTotal = formDefine.size();
				formName = _formName;
				formHeader = formDefine;
				ifFull = false;
				ifNull = true;
				for (unsigned int i = 0; i < formHeader.size(); i++) {
					bool if_primary = formHeader[i][2] == L"true" ? true : false;
					if (formHeader[i][1] == L"int") {
						AVL<int> *tmp = new AVL<int>(formHeader[i][0], i, if_primary);
						intHeader.push_back(tmp);
					}
					if (formHeader[i][1] == L"char") {
						AVL<std::wstring> *tmp = new AVL<std::wstring>(formHeader[i][0], i, if_primary);
						wstringHeader.push_back(tmp);
					}
				}
			}

			~Form() {

			}

			/*bool cmp1(const std::pair<int, std::wstring> &m_one, const std::pair<int, std::wstring>&m_two)
			{
				return (m_one.second> m_two.second);//定义了大于，就是按从小到大排序 //可以定义任何你想要的比较操作  
			}
			bool cmp2(std::pair<int, int> m_one, std::pair<int,int>m_two)
			{
				return (m_one.second > m_two.second);//定义了大于，就是按从小到大排序 //可以定义任何你想要的比较操作  
			}*/
			void Insert(std::vector<std::wstring> dataSet) {

				idTotal++;
				//建立可以传给Record的参数formDefine
				std::vector<std::vector<std::wstring>> formDefine;

				for (unsigned int i = 0; i < dataSet.size(); i++) {

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

			//删除全部记录
			void Delete() {
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

			//删除符合多种条件的记录(A&&B,A||B)
			int Delete(std::vector<std::vector<std::wstring>> condition, std::wstring relation) {
				std::vector<std::vector<int> > ans;
				for (int i = 0; i < condition.size(); i++)
				{
					for (int j = 0; j < wstringHeader.size(); j++)
					{

						if (wstringHeader[j]->GetName() == condition[i][0])
						{
							ans.push_back(wstringHeader[j]->finddata(condition[i][1], condition[i][2]));
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
							ans.push_back(intHeader[j]->finddata(x, condition[i][2]));
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

			//打印整个表
			void Select(int orderx,std::wstring name) {
				std::map<int, Record>::iterator it;
				std::map<std::wstring, int>mp1;
				std::map<int, int>mp2;
				int flag = 0;
				//	std::cout.setf(std::ios::right, std::ios::adjustfield);

				for (int i = 0; i < formHeader.size(); i++)
				{
					std::wcout << std::setw(24) << formHeader[i][0];
				}
				std::cout << std::endl;
				std::wstring sa = L"char";
				std::wstring sb = L"int";
				if (orderx > 0)
				{
					for (it = records.begin(); it != records.end(); it++)
					{
						std::vector<Key*> tmp = it->second.getRecord();
						for (int i = 0; i < tmp.size(); i++)
						{
							if (tmp[i]->getKeyName() == name)
							{
								if (tmp[i]->getTypeName() == L"wstring")
								{
									flag = 1;
									StringKey * p = (StringKey*)tmp[i];
									int x = it->first;
									std::wstring y = p->getData();
									mp1.insert(std::map<std::wstring, int>::value_type(y, x));
								}
								else
								{
									IntKey * p = (IntKey*)tmp[i];
									int x = it->first;
									int y = p->getData();
									mp2.insert(std::map<int, int>::value_type(y, x));
								}
							}
						}
					}

				}
				std::vector<int> pp;
				if (orderx != 0)
				{
					if (flag == 1)
					{
						if (orderx == 1)
						{
							for (std::map<std::wstring, int>::iterator rit = mp1.begin(); rit != mp1.end(); rit++)
							{
								pp.push_back(rit->second);
							}
						}
						else
						{
							for (std::map<std::wstring, int>::reverse_iterator rit = mp1.rbegin(); rit != mp1.rend(); rit++)
							{
								pp.push_back(rit->second);
							}

						}
					}
					else
					{
						if (orderx == 1)
						{
							for (std::map<int, int>::iterator rit = mp2.begin(); rit != mp2.end(); rit++)
							{
								pp.push_back(rit->second);
							}
						}
						else
						{
							for (std::map<int, int>::reverse_iterator rit = mp2.rbegin(); rit != mp2.rend(); rit++)
							{
								pp.push_back(rit->second);
							}

						}
					}
				}
					for (int i = 0; i < pp.size(); i++)
					{
						it = records.find(pp[i]);
						std::vector<Key*> tmpp = it->second.getRecord();

						for (int j = 0; j < tmpp.size(); j++)
						{
							if (tmpp[j]->getTypeName() == sa)
							{
								StringKey* now = (StringKey*)tmpp[j];
								if (!now->isEmpty) {
									std::wcout << std::setw(24) << now->getData();
								}
								else {
									std::wcout << std::setw(24) << " ";
								}

							}
							else
							{
								IntKey* now = (IntKey*)tmpp[j];
								//std::wcout << std::setw(8) << now->getData();
								if (!now->isEmpty) {
									std::wcout << std::setw(24) << now->getData();
								}
								else {
									std::wcout << std::setw(24) << " ";
								}
							}
						}

						std::wcout << std::endl;
					}
			
			}

			//打印表的部分列
			void Select(std::vector<std::wstring> keyNames,int orderx,std::wstring name) {
				std::map<int, Record>::iterator it;
				std::map<std::wstring, int>mp1;
				std::map<int, int>mp2;
				int flag = 0;
				std::cout.setf(std::ios::left, std::ios::adjustfield);
				for (int i = 0; i < keyNames.size(); i++)
				{
					std::wcout << std::setw(24) << keyNames[i];
				}
				std::cout << std::endl;
				std::wstring sa = L"char";
				std::wstring sb = L"int";
				if (orderx > 0)
				{
					for (it = records.begin(); it != records.end(); it++)
					{
						std::vector<Key*> tmp = it->second.getRecord();
						for (int i = 0; i < tmp.size(); i++)
						{
							if (tmp[i]->getKeyName() == name)
							{
								if (tmp[i]->getTypeName() == L"wstring")
								{
									flag = 1;
									StringKey * p = (StringKey*)tmp[i];
									int x = it->first;
									std::wstring y = p->getData();
									mp1.insert(std::map<std::wstring, int>::value_type(y, x));
								}
								else
								{
									IntKey * p = (IntKey*)tmp[i];
									int x = it->first;
									int y = p->getData();
									mp2.insert(std::map<int, int>::value_type(y, x));
								}
							}
						}
					}

				}
				std::vector<int> pp;
				if (orderx != 0)
				{
					if (flag == 1)
					{
						if (orderx == 1)
						{
							for (std::map<std::wstring, int>::iterator rit = mp1.begin(); rit != mp1.end(); rit++)
							{
								pp.push_back(rit->second);
							}
						}
						else
						{
							for (std::map<std::wstring, int>::reverse_iterator rit = mp1.rbegin(); rit != mp1.rend(); rit++)
							{
								pp.push_back(rit->second);
							}

						}
					}
					else
					{
						if (orderx == 1)
						{
							for (std::map<int, int>::iterator rit = mp2.begin(); rit != mp2.end(); rit++)
							{
								pp.push_back(rit->second);
							}
						}
						else
						{
							for (std::map<int, int>::reverse_iterator rit = mp2.rbegin(); rit != mp2.rend(); rit++)
							{
								pp.push_back(rit->second);
							}

						}
					}
				}
				for (int i  = 0; i <= pp.size();i++)
				{
					it = records.find(pp[i]);
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
									//std::wcout << std::setw(8) << now->getData();
									if (!now->isEmpty) {
										std::wcout << std::setw(24) << now->getData();
									}
									else {
										std::wcout << std::setw(24) << " ";
									}
								}
								else
								{
									IntKey* now = (IntKey*)tmpp[k];
									//std::wcout << std::setw(8) << now->getData();
									if (!now->isEmpty) {
										std::wcout << std::setw(24) << now->getData();
									}
									else {
										std::wcout << std::setw(24) << " ";
									}

								}
							}
						}
					}
					std::cout << std::endl;
				}
			}

			//打印符合条件的记录
			int Select(std::vector<std::wstring> keyNames, std::vector<std::vector<std::wstring> >condition, int orderx, std::wstring name) {
				std::map<int, Record>::iterator it;
				std::cout.setf(std::ios::left, std::ios::adjustfield);
				std::vector<int>tmp;
				std::wstring sa = L"char";
				std::wstring sb = L"int";
				std::set<int>stmp;
				std::map<std::wstring,int> mp1;
				std::map<int, int> mp2;
				for (int i = 0; i < condition.size(); i++)
				{
					tmp.clear();
					for (int j = 0; j < wstringHeader.size(); j++)
					{
						if (wstringHeader[j]->GetName() == condition[i][0])
						{
							tmp = wstringHeader[j]->finddata(condition[i][1], condition[i][2]);
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
							tmp = intHeader[j]->finddata(x, condition[i][2]);
						}
					}
					for (int j = 0; j < tmp.size(); j++) stmp.insert(tmp[j]);
				}
				tmp.clear();
				if (orderx > 0)
				{
					for (int i = 0; i < wstringHeader.size(); i++)
					{
						if (wstringHeader[i]->GetName() == name)
						{
							std::set<int>::iterator it;
							for (it = stmp.begin(); it != stmp.end(); it++)
							{
								int x = (*it);
								std::vector<Key*> h = records[x].getRecord();
								int j;
								for (j = 0; j < h.size(); j++)
								{
									if (h[j]->getKeyName() == name)
									break;
								}
								StringKey * p = (StringKey*)h[j];
								std::wstring y = p->getData();
								mp1.insert(std::map<std::wstring,int>::value_type(y, x));
								
									
							}
							//std::sort(v1.begin(), v1.end());
							if (orderx == 1)
							{
								for (std::map<std::wstring,int>::iterator rit = mp1.begin(); rit != mp1.end(); rit++)
								{
									tmp.push_back(rit->second);
								}
							}
							if (orderx == 2)
							{
								for (std::map<std::wstring, int>::reverse_iterator rit = mp1.rbegin(); rit != mp1.rend(); rit++)
								{
									tmp.push_back(rit->second);
								}
							}
						}

					}
					for (int i = 0; i < intHeader.size(); i++)
					{
						if (intHeader[i]->GetName() == name)
						{
							std::set<int>::iterator it;
							for (it = stmp.begin(); it != stmp.end(); it++)
							{
								int x = (*it);
								std::vector<Key*> h = records[x].getRecord();
								int j;
								for (j = 0; j < h.size(); j++)
								{
									if (h[j]->getKeyName() == name)
										break;
								}
								IntKey * p = (IntKey*)h[j];
								int y = p->getData();
								mp2.insert(std::map<int, int>::value_type(y, x));
							}
							//std::sort(v2.begin(), v2.end());
							if (orderx == 1)
							{
								for (std::map<int, int>::iterator rit = mp2.begin(); rit != mp2.end(); rit++)
								{
									tmp.push_back(rit->second);
								}
							}
							if (orderx == 2)
							{
								for (std::map<int, int>::reverse_iterator rit = mp2.rbegin(); rit != mp2.rend(); rit++)
								{
									tmp.push_back(rit->second);
								}
							}
						}


					}
				}
				else
				{
					std::set<int>::iterator it;
					for (it = stmp.begin(); it != stmp.end(); it++)
					{
						tmp.push_back(*it);
					}
				}

				for (int i = 0; i < keyNames.size(); i++)
				{
					std::wcout << std::setw(24) << keyNames[i];
				}
				std::cout << std::endl;
				for (int i = 0; i < tmp.size(); i++)
				{

					for (int j = 0; j < keyNames.size(); j++)
					{
						it = records.find(tmp[i]);
						std::vector<Key*> tmpp = it->second.getRecord();
						for (int k = 0; k < tmpp.size(); k++)
						{
							if (tmpp[k]->getKeyName() == keyNames[j])
							{
								if (tmpp[k]->getTypeName() == sa)
								{
									StringKey* now = (StringKey*)tmpp[k];
									if (!now->isEmpty) {
										std::wcout << std::setw(24) << now->getData();
									}
									else {
										std::wcout << std::setw(24) << " ";
									}
									//std::wcout << std::setw(8) << now->getData();
								}
								else
								{
									IntKey* now = (IntKey*)tmpp[k];
									if (!now->isEmpty) {
										std::wcout << std::setw(24) << now->getData();
									}
									else {
										std::wcout << std::setw(24) << " ";
									}
									//std::wcout << std::setw(8) << now->getData();

								}
							}
						}
					}
					std::cout << std::endl;
				}
				return tmp.size();
			}

			int Update(std::vector<std::vector<std::wstring>> set, std::vector<std::vector<std::wstring>> condition) {
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
							tmp = wstringHeader[j]->finddata(condition[i][1], condition[i][2]);
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
							tmp = intHeader[j]->finddata(x, condition[i][2]);

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

			std::wstring getFormName() {
				return formName;
			}

			std::map<int, Record> getForm() {
				return records;
			}

			std::vector<AVL<std::wstring>*> getWstringHeader() {
				return wstringHeader;
			}

			std::vector<AVL<int>*> getIntHeader() {
				return intHeader;
			}

			int getIdTotal() {
				return idTotal;
			}

			int getKeyTotal() {
				return keyTotal;
			}

			std::vector<std::vector<std::wstring>> getFormHeader() {
				return formHeader;
			}

			bool getFull() {
				return ifFull;
			}

			bool getNull() {
				return ifNull;
			}

			void setFormName(std::wstring x) {
				formName = x;
			}

			void setForm(std::map<int, Record>x) {
				records = x;
			}

			void setWstringHeader(std::vector<AVL<std::wstring>*> x) {
				wstringHeader = x;
			}

			void setIntHeader(std::vector<AVL<int>*>x) {
				intHeader = x;
			}

			void setIdTotal(int x) {
				idTotal = x;
			}

			void setKeyTotal(int x) {
				keyTotal = x;
			}

			void setFormHeader(std::vector<std::vector<std::wstring>>x) {
				formHeader = x;
			}

			void setFull(bool x) {
				ifFull = x;
			}

			void setNull(bool x) {
				ifNull = x;
			}
		protected:

		private:
			std::wstring formName;
			std::vector<AVL<std::wstring>*> wstringHeader;
			std::vector<AVL<int>*> intHeader;
			int idTotal;
			int keyTotal;
			std::vector<std::vector<std::wstring>> formHeader;//表头
			std::map<int, Record> records;//重载Record的'<'

			bool ifNull;
			bool ifFull;
		};
	}
}
