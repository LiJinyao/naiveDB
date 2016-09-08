#pragma once
#include "stdafx.h"

namespace naiveDB {
	namespace dataprocessor {
		class Key {

		public:
			Key() {

			}

			Key(std::wstring _keyName,
				std::wstring _typeName,
				bool _ifPrimaryKey,
				bool _ifNotNull) {
				keyName = _keyName;
				typeName = _typeName;
				ifPrimaryKey = _ifPrimaryKey;
				ifNotNull = _ifNotNull;
			}
			~Key() {
		
			}

			std::wstring getKeyName() {
				return keyName;
			}

			std::wstring getTypeName() {
				return typeName;
			}

			bool getPrimaryStatus() {
				return ifPrimaryKey;
			}

			bool getNotNullStatus() {
				return ifNotNull;
			}

			void setPrimaryStatus(bool x) {
				ifPrimaryKey = x;
			}

			void setNotNullStatus(bool x) {
				ifNotNull = x;
			}

			void setKeyName(std::wstring x) {
				keyName = x;
			}

			void setTypeName(std::wstring x) {
				typeName = x;
			}

			bool isEmpty = false;
		protected:
			bool ifPrimaryKey;
			bool ifNotNull;
			std::wstring keyName;
			std::wstring typeName;

		private:
		};
	}
}


