#pragma once
#include "Key.hpp"

namespace naiveDB {
	namespace dataprocessor {
		class IntKey : public Key {
		public:
			IntKey() {

			}

			IntKey(std::wstring _keyName,
				std::wstring _typeName,
				bool _ifPrimaryKey,
				bool _ifNotNull,
				int _data) 
				: Key(_keyName, _typeName, _ifPrimaryKey, _ifNotNull) {
				data = _data;
			}

			~IntKey() {
			
			}

			int getData() {
				return data;
			}

			void setData(int _data) {
				data = _data;
			}

		protected:

		private:
			int data;
		};
	}
}

