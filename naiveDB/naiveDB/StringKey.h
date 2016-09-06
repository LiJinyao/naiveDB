#pragma once
#include "Key.h"

namespace naiveDB {
	namespace dataprocessor {
		class StringKey : public Key {
		public:
			StringKey(
				std::wstring key,
				std::wstring type,
				bool _ifPrimaryKey,
				bool _ifNotNull,
				int _lengthLimit,
				std::wstring _data);

			~StringKey();

			std::wstring getData();

			void setData(std::wstring _data);

			int getLengthLimit();

		protected:

		private:
			std::wstring data;
			int lengthLimit;
		};
	}
}
