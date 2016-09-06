#pragma once
#include "Key.h"

namespace naiveDB {
	namespace dataprocessor {
		class IntKey : public Key {
		public:
			IntKey(std::wstring _keyName,
				std::wstring _typeName,
				bool _ifPrimaryKey,
				bool _ifNotNull,
				int _data);

			~IntKey();

			int getData();

			void setData(int _data);

		protected:

		private:
			int data;
		};
	}
}

