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

			//删除全部记录
			void Delete();

			//删除符合多种条件的记录(A&&B,A||B)
			int Delete(std::vector<std::vector<std::wstring>> condition, std::wstring relation);

			//打印整个表
			void Select();

			//打印表的部分列
			void Select(std::vector<std::wstring> keyNames);

			//打印符合条件的记录
			int Select(std::vector<std::wstring> keyNames, std::wstring condition[2]);

			int Update(std::vector<std::vector<std::wstring>> set, std::vector<std::vector<std::wstring>> condition);

			std::wstring getFormName();

			std::map<int, Record> getForm();

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
