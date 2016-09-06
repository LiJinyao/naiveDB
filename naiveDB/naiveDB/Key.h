#pragma once
#include <string>
namespace naiveDB {
	namespace dataprocessor {
		class Key {
		public:
			Key(std::wstring _keyName,
				std::wstring _typeName,
				bool _ifPrimaryKey,
				bool _ifNotNull);
			~Key();

			std::wstring getKeyName();
			std::wstring getTypeName();
			bool getPrimaryStatus();
			bool getNotNullStatus();


		protected:
			bool ifPrimaryKey;
			bool ifNotNull;
			std::wstring keyName;
			std::wstring typeName;


		private:

		};
	}
}


