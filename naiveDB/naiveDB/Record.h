#pragma once
#include "Key.h"
#include "IntKey.h"
#include "StringKey.h"
#include <vector>

namespace naiveDB {
	namespace dataprocessor {
		class Record {
		public:

			//����һ���¼
			Record(int _id, std::vector<std::vector<std::wstring>> formDefine);

			~Record();

			int getId();

			void printRecord();

			Key* findKey(std::wstring _name);

			std::vector<Key *> getRecord();

			void setId(int x);

			void setRecord(std::vector<Key *> x);

		protected:

		private:

			std::vector<Key*> record;
			int id;
		};
	}
}
