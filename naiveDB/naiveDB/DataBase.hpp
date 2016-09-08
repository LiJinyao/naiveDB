#pragma once
#include "stdafx.h"
#include "Form.hpp"
#include "AST.hpp"

namespace naiveDB {
	class DataBase {
	public:
		DataBase() {

		}

		DataBase(std::wstring _dbName) {
			DBName = _dbName;
		}


		~DataBase() {

		}

		//����
		void Create(naiveDB::parser::CreateTableStatement _cs) {
			naiveDB::parser::CreateTableStatement cs = _cs;
			std::wstring formName = cs.tableName; //����2
			std::vector<std::vector<std::wstring>> formDefine;
			if (formDefine.size() == 0) {
				std::wcout << L"���󣡲��ܴ����ձ��������Ķ��塣" << std::endl;
				return;
			}
			for (unsigned int i = 0; i < formSet.size(); i++) {
				if (formSet[i].getFormName() == formName) {
					std::wcout << L"��Ϊ" << formName << L"�ı��Ѿ����ڣ���ѡ���������ơ�" << std::endl;
					return;
				}
			}

			std::vector<naiveDB::parser::ColumnStatement> columnDefine = cs.columns;

			//�������1
			for (unsigned int i = 0; i < columnDefine.size(); i++) {
				std::vector<std::wstring> s;

				//name��typeû����
				s.push_back(columnDefine[i].name);
				s.push_back(columnDefine[i].dataType);

				//���attribute��"NOT NULL"��isPrimaryKeyΪfalse��isNotNullΪtrue
				//���attribute��"PRIMARY KEY"��isPrimaryKeyΪtrue,isNotNullΪfalse
				if (columnDefine[i].attribute == L"NOT NULL") {
					s.push_back(L"false");
					s.push_back(L"true");
				}
				else if (columnDefine[i].attribute == L"PRIMARY KEY") {
					s.push_back(L"true");
					s.push_back(L"false");
				}

				//�������ǳ������ƣ���Ҫ��intת��wstring
				std::wstringstream ss;
				ss << columnDefine[i].limit;
				std::wstring wstr;
				ss >> wstr;
				s.push_back(wstr);

				formDefine.push_back(s);
			}

			naiveDB::dataprocessor::Form f = naiveDB::dataprocessor::Form(formDefine, formName);
			formSet.push_back(f);
		}

		//ע������ֻʵ���� INSERT INTO ������ VALUES (ֵ1, ֵ2,....) һ�־�ʽ
		//�þ�ʽҪ���ֵ�������಻�٣��Ҹ�������ϱ���
		void Insert(naiveDB::parser::InsertStatement _is) {
			std::wstring formName = _is.tableName;
			std::vector<std::wstring> data = _is.values;
			std::vector<std::wstring> definition = _is.columns;

			bool if_found = false;
			int foundform = -1;
			//���ж���û�����ű�
			for (unsigned int i = 0; i < formSet.size(); i++) {
				if (formSet[i].getFormName() == formName) {
					if_found = true;
					foundform = i;
					break;
				}
			}
			if (!if_found) {
				std::wcout << L"��" << formName << L"�����ڣ����ڽ������в��������" << std::endl;
				return;
			}

			std::vector<std::vector<std::wstring>> fh = formSet[foundform].getFormHeader();

			//�������������¼
			if (definition.size() == 0) {
				if (data.size() != fh.size()) {
					std::wcout << L"���󣡲���������ƥ�䡣" << std::endl;
					return;
				}
				//�����������ͼ��
				bool if_TypeError = false;
				for (unsigned int i = 0; i < data.size(); i++) {
					bool if_int = true;
					std::wstring tmp = data[i];
					for (unsigned int j = 0; j < tmp.length(); j++) {
						if (tmp[j] >= L'9' || tmp[j] <= L'0') {
							if_int = false;
							break;
						}
					}
					if (if_int == true && fh[i][1] == L"wstring") {
						std::wcout << L"�����˴�����������ͣ�" << fh[i][0] << L"��char�͡�" << std::endl;
						if_TypeError = true;
					}
					if (if_int == false && fh[i][1] == L"int") {
						std::wcout << L"�����˴�����������ͣ�" << fh[i][0] << L"��int�͡�" << std::endl;
						if_TypeError = true;
					}
				}
				if (if_TypeError) {
					return;
				}

				//���г������Ƽ��
				bool if_LengthError = false;
				for (unsigned int i = 0; i < data.size(); i++) {
					if (fh[i][1] == L"wstring") {
						std::wstringstream ss;
						int limit;
						ss << fh[i][4];
						ss >> limit;
						if (data[i].length() > limit) {
							std::wcout << L"�����ֶ�" << fh[i][0] << L"�����������ơ�" << std::endl;
							if_LengthError = true;
						}
					}
				}
				if (if_LengthError) {
					return;
				}

				//�����������Ƽ��
				bool if_PropertyError = false;

			}
			//�ɲ��벿���ֶ�
			else {
				if (data.size() != definition.size()) {
					std::wcout << L"���󣡲���������ƥ�䡣" << std::endl;
					return;
				}

			}
			formSet[foundform].Insert(data);
		}

		//����ֻʵ��ѡ��һ�ű��е�����
		void Select(naiveDB::parser::SelectStatement _ss) {
			naiveDB::parser::SelectStatement ss = _ss;
			int foundForm = -1;
			for (unsigned int i = 0; i < formSet.size(); i++) {
				if (formSet[i].getFormName() == ss.fromtables[0]) {
					foundForm = i;
					break;
				}
			}
			//selectȫ��
			if (ss.columns.size() == 1 && ss.columns[0] == L"") {
				formSet[foundForm].Select();
				return;
			}
			//select�����У���������
			else if (ss.whereClause.statements.size() == 0) {
				formSet[foundForm].Select(ss.columns);
				return;
			}
			//select�����У���������
			else { 
				std::wstring condition[2];
				condition[0] = ss.whereClause.statements[0].lh;
				condition[1] = ss.whereClause.statements[0].rh;
				formSet[foundForm].Select(ss.columns, condition);
				return;
			}
			
		}

		void Delete(naiveDB::parser::DeleteStatement _ds) {
			naiveDB::parser::DeleteStatement ds = _ds;
			std::wstring formName = ds.tableName;
			int foundForm = -1;
			//�����Ƿ����
			bool if_found = false;
			for (unsigned int i = 0; i < formSet.size(); i++) {
				if (formSet[i].getFormName() == formName) {
					if_found = true;
					foundForm = i;
					break;
				}
			}
			if (!if_found) {
				std::wcout << L"��Ҫɾ���ı����ڡ�" << std::endl;
				return;
			}

			//ɾ��ȫ��
			if (ds.whereClause.statements.size() == 0) {
				formSet[foundForm].Delete();
			}
			//ɾ�����ּ�¼(Ŀǰֻ֧�ֵ�����)
			else {
				bool keyflag = false;
				bool valueflag = false;
				int keycolumn = -1;

				std::vector<std::vector<std::wstring>> _copy = formSet[foundForm].getFormHeader();
				for (unsigned int i = 0; i < _copy.size(); i++) {
					if (_copy[i][0] == ds.whereClause.statements[0].lh) {
						keyflag = true;
						keycolumn = i;
						break;
					}
				}
				if (!keyflag) {
					std::wcout << L"����" << ds.whereClause.statements[0].lh
						<< L"���Ǳ�" << formSet[foundForm].getFormName()
						<< L"�ĳ�Ա��" << std::endl;
					return;
				}
				//����ֵ�Ƿ����
				/*if (keyflag) {
				std::vector<naiveDB::dataprocessor::AVL<int>*> introots = formSet[foundForm].getIntHeader();
				std::vector<naiveDB::dataprocessor::AVL<std::wstring>*> wstringroots = formSet[foundForm].getWstringHeader();
				for (unsigned int i = 0; i < introots.size(); i++) {
				if (introots[i]->GetName() == ds.whereClause.statements[0].lh) {
				std::wstringstream ss;
				int num;
				ss << ds.whereClause.statements[0].
				}
				}


				}*/

				std::vector<std::vector<std::wstring>> condition;
				std::vector<std::wstring> s;
				s.push_back(ds.whereClause.statements[0].lh);
				s.push_back(ds.whereClause.statements[0].rh);
				condition.push_back(s);
				std::wstring relation = L"NONE";
				int affected = formSet[foundForm].Delete(condition, relation);

				if (affected == 0) {
					std::wcout << L"ɾ��ʧ�ܣ�û�з��������ļ�¼" << std::endl;
				}
			}
		}

		void Update(naiveDB::parser::UpdateStatement _us) {
			naiveDB::parser::UpdateStatement us = _us;
			int foundForm = -1;
			for (unsigned int i = 0; i < formSet.size(); i++) {
				if (formSet[i].getFormName() == us.tableName) {
					foundForm = i;
					break;
				}
			}

			if (foundForm == -1) {
				std::wcout << L"������Ҫ�����ı����ڡ�" << std::endl;
				return;
			}

			std::vector<std::vector<std::wstring>> set;
			std::vector<std::vector<std::wstring>> condition;
			for (unsigned int i = 0; i < us.sets.size(); i++) {
				std::vector<std::wstring> s;
				s.push_back(us.sets[i].col);
				s.push_back(us.sets[i].val);
				set.push_back(s);
				s.clear();
			}
		
			for (unsigned int i = 0; i < us.whereClause.statements.size(); i++) {
				std::vector<std::wstring> s;
				s.push_back(us.whereClause.statements[i].lh);
				s.push_back(us.whereClause.statements[i].rh);
				condition.push_back(s);
				s.clear();
			}

			formSet[foundForm].Update(set, condition);

		}

		void Use(naiveDB::parser::UseDatabaseStatement _uds) {

		}


	protected:

	private:
		std::vector<dataprocessor::Form> formSet;
		std::wstring DBName;
	};
}

		

	



