#pragma once
#include "Key.hpp"

namespace naiveDB {
	namespace dataprocessor {
		class StringKey : public Key {
		public:
			StringKey(
				std::wstring _keyName,
				std::wstring _typeName,
				bool _ifPrimaryKey,
				bool _ifNotNull,
				int _lengthLimit,
				std::wstring _data) 
				: Key(_keyName, _typeName, _ifPrimaryKey, _ifNotNull) {
				lengthLimit = _lengthLimit;
				data = _data;
			}

			~StringKey() {
				delete &keyName;
				delete &typeName;
				delete &data;
			}

			std::wstring getData() {
				return data;
			}

			void setData(std::wstring _data) {
				data = _data;
			}

			int getLengthLimit() {
				return lengthLimit;
			}


			void setLengthLimit(int x) {
				lengthLimit = x;
			}

		protected:

		private:
			std::wstring data;
			int lengthLimit;
		};
	}
}
