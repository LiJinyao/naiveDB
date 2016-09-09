#include "Form.hpp"
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
		void Create(
			std::vector<std::vector<std::wstring>> _formDefine, 
			std::wstring _formName) {
			std::wstring formName = _formName;
			std::vector<std::vector<std::wstring>> formDefine = _formDefine;
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

			naiveDB::dataprocessor::Form f = naiveDB::dataprocessor::Form(formDefine, formName);
			formSet.push_back(f);
			std::wcout << L"�ɹ�����һ����Ϊ" << formName << L"�ı�" << std::endl;
			return;
		}




		//ע������ֻʵ���� INSERT INTO ������ VALUES (ֵ1, ֵ2,....) һ�־�ʽ
		//�þ�ʽҪ���ֵ�������಻�٣��Ҹ�������ϱ���
		void Insert(
			std::wstring _formName, 
			std::vector<std::wstring> _data, 
			std::vector<std::wstring> _definition) {
			std::wstring formName = _formName;
			std::vector<std::wstring> data = _data;
			std::vector<std::wstring> definition = _definition;

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
			std::vector<std::wstring> dataset;
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
						if (tmp[j] > L'9' || tmp[j] < L'0') {
							if_int = false;
							break;
						}
					}
					if (if_int == true && fh[i][1] == L"char") {
						std::wcout << L"�����˴�����������ͣ�" << fh[i][0] << L"��char�͡�" << std::endl;
						if_TypeError = true;
					}
					else if (if_int == false && fh[i][1] == L"int") {
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
					if (fh[i][1] == L"char") {
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
				/*bool if_PropertyError = false;
				for (unsigned int i = 0; i < data.size(); i++) {
				if(fh[i][2] == L"true")
				}*/

				dataset = data;
			}
			//�ɲ��벿���ֶ�
			else {
				if (data.size() != definition.size()) {
					std::wcout << "���󣡲���������ƥ�䡣" << std::endl;
					return;
				}

				//ֻ�ҳ���һ�������ڵ��ֶ�
				for (unsigned int i = 0; i < definition.size(); i++) {
					bool if_exist = false;
					for (unsigned int j = 0; j < fh.size(); j++) {
						if (fh[j][0] == definition[i]) {
							if_exist = true;
							break;
						}
					}
					if (!if_exist) {
						std::wcout << "�����ֶ�" << definition[i] << "�����ڡ�" << std::endl;
						return;
					}
				}

				bool if_TypeError = false;
				for (unsigned int i = 0; i < data.size(); i++) {
					bool if_int = true;
					std::wstring tmp = data[i];
					for (unsigned int j = 0; j < tmp.length(); j++) {
						if (tmp[j] > L'9' || tmp[j] < L'0') {
							if_int = false;
							break;
						}
					}
					for (unsigned int j = 0; j < fh.size(); j++) {
						if (fh[j][0] == definition[i]) {
							if (fh[j][1] == L"int" && if_int == false) {
								std::wcout << L"�����˴�����������ͣ�" << fh[i][0] << L"��int�͡�" << std::endl;
								if_TypeError = true;
							}
							else if (fh[j][1] == L"char" && if_int == true) {
								std::wcout << L"�����˴�����������ͣ�" << fh[i][0] << L"��char�͡�" << std::endl;
								if_TypeError = true;
							}
						}
					}
				}
				if (if_TypeError) {
					return;
				}

				//���г������Ƽ��
				bool if_LengthError = false;
				for (unsigned int i = 0; i < data.size(); i++) {
					for (unsigned int j = 0; j < fh.size(); j++) {
						if (fh[j][0] == definition[i]) {
							if (fh[i][1] == L"char") {
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

					}
				}
				if (if_LengthError) {
					return;
				}
				for (unsigned int i = 0; i < fh.size(); i++) {
					bool if_occupied = false;
					for (unsigned j = 0; j < definition.size(); j++) {
						if (definition[j] == fh[i][0]) {
							if_occupied = true;
							dataset.push_back(data[j]);
							break;
						}
					}
					if (!if_occupied) {
						dataset.push_back(L"");
					}

				}

			}

			formSet[foundform].Insert(dataset);
		}




		//����ֻʵ��ѡ��һ�ű��е�����
		void Select(
			std::vector<std::wstring> _columns, 
			std::vector<std::wstring> _fromtables, 
			std::vector<std::vector<std::wstring>> _whereClause) {
			

			std::vector<std::wstring> columns = _columns;
			std::vector<std::wstring> fromtables = _fromtables;
			std::vector<std::vector<std::wstring>> whereClause = _whereClause;

			int foundForm = -1;
			for (unsigned int i = 0; i < formSet.size(); i++) {
				if (formSet[i].getFormName() == fromtables[0]) {
					foundForm = i;
					break;
				}
			}
			if (foundForm == -1) {
				std::wcout << L"����û����Ҫ��ѯ�ı�" << std::endl;
				return;
			}

			//selectȫ��
			if (columns.size() == 1 && columns[0] == L"*") {
				formSet[foundForm].Select();
				return;
			}

			//select�����У���������
			else if (whereClause.size() == 0) {
				formSet[foundForm].Select(columns);
				return;
			}

			//select�����У���������
			else {
				
				std::vector<std::vector<std::wstring>> condition;
				for (unsigned int i = 0; i < whereClause.size(); i++) {
					std::vector<std::wstring> s;
					s.push_back(whereClause[i][0]);
					s.push_back(whereClause[i][1]);
					s.push_back(whereClause[i][2]);
					condition.push_back(s);
					s.clear();
				}

				int orderx = 0;
				std::wstring name = L"";

				formSet[foundForm].Select(columns, condition, orderx, name);
				return;
			}

		}



		void Delete(
			std::wstring _tableName, 
			std::vector<std::vector<std::wstring>> _whereClause) {
			
			std::wstring formName = _tableName;
			std::vector<std::vector<std::wstring>> whereClause = _whereClause;
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
			if (whereClause.size() == 0) {
				formSet[foundForm].Delete();
			}
			//ɾ�����ּ�¼(֧�ֶ�����)
			else {
				bool keyflag = false;
	
				int keycolumn = -1;

				std::vector<std::vector<std::wstring>> _copy = formSet[foundForm].getFormHeader();
				
				for (unsigned int i = 0; i < whereClause.size(); i++) {
					keyflag = false;
					for (unsigned int j = 0; j < _copy.size(); j++) {
						if (whereClause[i][0] == _copy[j][0]) {
							keyflag = true;
							break;
						}
					}
					if (!keyflag) {
						std::wcout << L"����" << whereClause[i][0]
							<< L"���Ǳ�" << formSet[foundForm].getFormName()
							<< L"�ĳ�Ա��" << std::endl;
						return;
					}
				}
				if (!keyflag) {
					return;
				}
				

				std::vector<std::vector<std::wstring>> condition;
				for (unsigned int i = 0; i < whereClause.size(); i++) {
					std::vector<std::wstring> s;
					s.push_back(whereClause[i][0]);
					s.push_back(whereClause[i][1]);
					s.push_back(whereClause[i][2]);
					condition.push_back(s);
					s.clear();
				}
			
				std::wstring relation = L"AND";
				int affected = formSet[foundForm].Delete(condition, relation);

				if (affected == 0) {
					std::wcout << L"ɾ��ʧ�ܣ�û�з��������ļ�¼" << std::endl;
				}
			}
		}



		void Update(
			std::wstring _tableName,
			std::vector<std::vector<std::wstring>> _sets,
			std::vector<std::vector<std::wstring>> _whereClause) {
			
			std::wstring tableName = _tableName;
			std::vector<std::vector<std::wstring>> sets = _sets;
			std::vector<std::vector<std::wstring>> whereClause = _whereClause;

			int foundForm = -1;
			for (unsigned int i = 0; i < formSet.size(); i++) {
				if (formSet[i].getFormName() == tableName) {
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
			for (unsigned int i = 0; i < sets.size(); i++) {
				std::vector<std::wstring> s;
				s.push_back(sets[i][0]);
				s.push_back(sets[i][1]);
				set.push_back(s);
				s.clear();
			}

			for (unsigned int i = 0; i < whereClause.size(); i++) {
				std::vector<std::wstring> s;
				s.push_back(whereClause[i][0]);
				s.push_back(whereClause[i][1]);
				s.push_back(whereClause[i][2]);
				condition.push_back(s);
				s.clear();
			}

			formSet[foundForm].Update(set, condition);

		}


		void Use(std::wstring dbName) {

		}


	protected:

	private:
		std::vector<dataprocessor::Form> formSet;
		std::wstring DBName;
	};
}

