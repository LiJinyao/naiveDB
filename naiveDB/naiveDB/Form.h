#pragma once
#include <string>
#include <vector>
#include <map>
#include "AVL.hpp"
#include "Record.h"

namespace naiveDB {
	namespace dataprocessor {
		class Form {
		public:
			Form(std::vector<std::vector<std::wstring>> formDefine, std::wstring _formName);

			~Form();


			void Insert(std::vector<std::wstring> formDefine);

			//ɾ��ȫ����¼
			void Delete();

			//ɾ�����϶��������ļ�¼(A&&B,A||B)
			int Delete(std::vector<std::vector<std::wstring>> condition, std::wstring relation);

			//��ӡ������
			void Select();

			//��ӡ��Ĳ�����
			void Select(std::vector<std::wstring> keyNames);

			//��ӡ���������ļ�¼
			int Select(std::vector<std::wstring> keyNames, std::wstring condition[2]);

			int Update(std::vector<std::vector<std::wstring>> set, std::vector<std::vector<std::wstring>> condition);

			std::wstring getFormName();

			std::map<int, Record> getForm();

			std::vector<AVL<std::wstring>*> getWstringHeader();

			std::vector<AVL<int>*> getIntHeader();

			int getIdTotal();

			int getKeyTotal();

			std::vector<std::vector<std::wstring>> getFormHeader();

			bool getFull();

			bool getNull();

			void setFormName(std::wstring x);

			void setForm(std::map<int,Record>x);

			void setWstringHeader(std::vector<AVL<std::wstring>*> x);

			void setIntHeader(std::vector<AVL<int>*>x);

			void setIdTotal(int x);

			void setKeyTotal(int x);

			void setFormHeader(std::vector<std::vector<std::wstring>>x);

			void setFull(bool x);

			void setNull(bool x);
		protected:

		private:
			std::wstring formName;
			std::vector<AVL<std::wstring>*> wstringHeader;
			std::vector<AVL<int>*> intHeader;
			int idTotal;
			int keyTotal;
			std::vector<std::vector<std::wstring>> formHeader;//��ͷ
			std::map<int, Record> records;//����Record��'<'

			bool ifNull;
			bool ifFull;
		};
	}
}
