#pragma once
#include "stdafx.h"
#include "Key.hpp"
#include "IntKey.hpp"
#include "StringKey.hpp"

namespace naiveDB {
	namespace dataprocessor {
		class Record {
		public:
			Record() {

			}

			
			Record(int _id, std::vector<std::vector<std::wstring>> formDefine) {
				id = _id;
				for (unsigned int i = 0; i < formDefine.size(); i++) {

					if (formDefine[i][1] == L"char") {
						std::wstringstream ss;
						int num;
						ss << formDefine[i][4];
						ss >> num;
						StringKey *sk = new StringKey(
							formDefine[i][0],
							formDefine[i][1],
							formDefine[i][2] == L"true" ? true : false,
							formDefine[i][3] == L"true" ? true : false,
							num,
							formDefine[i][5]
						);
						if (formDefine[i][5] == L"") {
							sk->isEmpty = true;
						}
						
						record.push_back(sk);
					}
					else if (formDefine[i][1] == L"int") {
						
						std::wstringstream ss;
						int num;
						ss << formDefine[i][5];
						ss >> num;
						IntKey *ik = new IntKey(
							formDefine[i][0],
							formDefine[i][1],
							formDefine[i][2] == L"true" ? true : false,
							formDefine[i][3] == L"true" ? true : false,
							num
						);
						if (formDefine[i][5] == L"") {
							ik->isEmpty = true;
						}
						record.push_back(ik);
					}
				}
			}

			~Record() {
				
			}


			int getId() {
				return id;
			}

			void printRecord() {
				for (unsigned int i = 0; i < record.size(); i++) {
					if (record[i]->getTypeName() == L"int") {
						IntKey *k = (IntKey*)record[i];
						std::cout << k->getData() << std::endl;
					}
					else if (record[i]->getTypeName() == L"char") {
						StringKey *k = (StringKey*)record[i];
						std::wcout << k->getData() << std::endl;
					}
				}
			}

			Key* findKey(std::wstring _name) {
				for (int i = 0; i < record.size(); i++)
				{
					if (record[i]->getKeyName() == _name)
					{
						return record[i];
					}
				}
			}

			std::vector<Key *> getRecord() {
				return  record;
			}

			void setId(int x) {
				id = x;
			}

			void setRecord(std::vector<Key *> x) {
				record = x;
			}

		protected:

		private:

			std::vector<Key*> record;
			int id;
		};
	}
}
