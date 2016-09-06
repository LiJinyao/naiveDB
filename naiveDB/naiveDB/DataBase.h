#pragma once
#include <vector>
#include <string>
#include "Form.h"

namespace naiveDB {
	class DataBase {
	public:

		DataBase(std::wstring _DBName);
		~DataBase();

		void Create(naiveDB::parser::CreateStatement _cs);
		void Insert(naiveDB::parser::InsertStatement _is);
		void Select(naiveDB::parser::SelectStatement _ss);

	protected:

	private:
		std::vector<dataprocessor::Form> formSet;
		std::wstring DBName;
	};
}


