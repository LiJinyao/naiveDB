#include "Form.hpp"
namespace naiveDB {
	class DataBase {
	public:
		DataBase() {

		}

		DataBase(std::wstring _dbName) {
			DBName = _dbName;
		}


		~DataBase() {

		}

		//建表
		void Create(
			std::vector<std::vector<std::wstring>> _formDefine, 
			std::wstring _formName) {
			std::wstring formName = _formName;
			std::vector<std::vector<std::wstring>> formDefine = _formDefine;
			if (formDefine.size() == 0) {
				std::wcout << L"错误！不能创建空表，请给出表的定义。" << std::endl;
				return;
			}
			for (unsigned int i = 0; i < formSet.size(); i++) {
				if (formSet[i].getFormName() == formName) {
					std::wcout << L"名为" << formName << L"的表已经存在，请选用其他名称。" << std::endl;
					return;
				}
			}

			naiveDB::dataprocessor::Form f = naiveDB::dataprocessor::Form(formDefine, formName);
			formSet.push_back(f);
			std::wcout << L"成功创建一张名为" << formName << L"的表。" << std::endl;
			return;
		}




		//注意这里只实现了 INSERT INTO 表名称 VALUES (值1, 值2,....) 一种句式
		//该句式要求键值对数不多不少，且各方面符合表定义
		void Insert(
			std::wstring _formName, 
			std::vector<std::wstring> _data, 
			std::vector<std::wstring> _definition) {
			std::wstring formName = _formName;
			std::vector<std::wstring> data = _data;
			std::vector<std::wstring> definition = _definition;

			bool if_found = false;
			int foundform = -1;
			//先判断有没有这张表
			for (unsigned int i = 0; i < formSet.size(); i++) {
				if (formSet[i].getFormName() == formName) {
					if_found = true;
					foundform = i;
					break;
				}
			}
			if (!if_found) {
				std::wcout << L"表" << formName << L"不存在，请在建表后进行插入操作。" << std::endl;
				return;
			}

			std::vector<std::vector<std::wstring>> fh = formSet[foundform].getFormHeader();
			std::vector<std::wstring> dataset;
			//必须插入完整记录
			if (definition.size() == 0) {
				if (data.size() != fh.size()) {
					std::wcout << L"错误！参数数量不匹配。" << std::endl;
					return;
				}
				//进行数据类型检查
				bool if_TypeError = false;
				for (unsigned int i = 0; i < data.size(); i++) {
					bool if_int = true;
					std::wstring tmp = data[i];
					for (unsigned int j = 0; j < tmp.length(); j++) {
						if (tmp[j] > L'9' || tmp[j] < L'0') {
							if_int = false;
							break;
						}
					}
					if (if_int == true && fh[i][1] == L"char") {
						std::wcout << L"插入了错误的数据类型，" << fh[i][0] << L"是char型。" << std::endl;
						if_TypeError = true;
					}
					else if (if_int == false && fh[i][1] == L"int") {
						std::wcout << L"插入了错误的数据类型，" << fh[i][0] << L"是int型。" << std::endl;
						if_TypeError = true;
					}
				}
				if (if_TypeError) {
					return;
				}

				//进行长度限制检查
				bool if_LengthError = false;
				for (unsigned int i = 0; i < data.size(); i++) {
					if (fh[i][1] == L"char") {
						std::wstringstream ss;
						int limit;
						ss << fh[i][4];
						ss >> limit;
						if (data[i].length() > limit) {
							std::wcout << L"错误！字段" << fh[i][0] << L"超出长度限制。" << std::endl;
							if_LengthError = true;
						}
					}
				}
				if (if_LengthError) {
					return;
				}

				//进行属性限制检查
				/*bool if_PropertyError = false;
				for (unsigned int i = 0; i < data.size(); i++) {
				if(fh[i][2] == L"true")
				}*/

				dataset = data;
			}
			//可插入部分字段
			else {
				if (data.size() != definition.size()) {
					std::wcout << "错误！参数数量不匹配。" << std::endl;
					return;
				}

				//只找出第一个不存在的字段
				for (unsigned int i = 0; i < definition.size(); i++) {
					bool if_exist = false;
					for (unsigned int j = 0; j < fh.size(); j++) {
						if (fh[j][0] == definition[i]) {
							if_exist = true;
							break;
						}
					}
					if (!if_exist) {
						std::wcout << "错误！字段" << definition[i] << "不存在。" << std::endl;
						return;
					}
				}

				bool if_TypeError = false;
				for (unsigned int i = 0; i < data.size(); i++) {
					bool if_int = true;
					std::wstring tmp = data[i];
					for (unsigned int j = 0; j < tmp.length(); j++) {
						if (tmp[j] > L'9' || tmp[j] < L'0') {
							if_int = false;
							break;
						}
					}
					for (unsigned int j = 0; j < fh.size(); j++) {
						if (fh[j][0] == definition[i]) {
							if (fh[j][1] == L"int" && if_int == false) {
								std::wcout << L"插入了错误的数据类型，" << fh[i][0] << L"是int型。" << std::endl;
								if_TypeError = true;
							}
							else if (fh[j][1] == L"char" && if_int == true) {
								std::wcout << L"插入了错误的数据类型，" << fh[i][0] << L"是char型。" << std::endl;
								if_TypeError = true;
							}
						}
					}
				}
				if (if_TypeError) {
					return;
				}

				//进行长度限制检查
				bool if_LengthError = false;
				for (unsigned int i = 0; i < data.size(); i++) {
					for (unsigned int j = 0; j < fh.size(); j++) {
						if (fh[j][0] == definition[i]) {
							if (fh[i][1] == L"char") {
								std::wstringstream ss;
								int limit;
								ss << fh[i][4];
								ss >> limit;
								if (data[i].length() > limit) {
									std::wcout << L"错误！字段" << fh[i][0] << L"超出长度限制。" << std::endl;
									if_LengthError = true;
								}
							}
						}

					}
				}
				if (if_LengthError) {
					return;
				}
				for (unsigned int i = 0; i < fh.size(); i++) {
					bool if_occupied = false;
					for (unsigned j = 0; j < definition.size(); j++) {
						if (definition[j] == fh[i][0]) {
							if_occupied = true;
							dataset.push_back(data[j]);
							break;
						}
					}
					if (!if_occupied) {
						dataset.push_back(L"");
					}

				}

			}

			formSet[foundform].Insert(dataset);
			saveFormName();
			saveForm(formSet[foundform]);
		}




		//这里只实现选择一张表中的内容
		void Select(
			std::vector<std::wstring> _columns, 
			std::vector<std::wstring> _fromtables, 
			std::vector<std::vector<std::wstring>> _whereClause,
		    std::wstring _orderBy,
			std::wstring _order) {
			

			std::vector<std::wstring> columns = _columns;
			std::vector<std::wstring> fromtables = _fromtables;
			std::vector<std::vector<std::wstring>> whereClause = _whereClause;

			int foundForm = -1;
			for (unsigned int i = 0; i < formSet.size(); i++) {
				if (formSet[i].getFormName() == fromtables[0]) {
					foundForm = i;
					break;
				}
			}
			if (foundForm == -1) {
				std::wcout << L"错误！没有所要查询的表。" << std::endl;
				return;
			}

			//select全表
			if (columns.size() == 1 && columns[0] == L"*") {
				formSet[foundForm].Select();
				return;
			}

			//select部分列（无条件）
			else if (whereClause.size() == 0) {
				formSet[foundForm].Select(columns);
				return;
			}

			//select部分列（多条件）
			else {
				
				std::vector<std::vector<std::wstring>> condition;
				for (unsigned int i = 0; i < whereClause.size(); i++) {
					std::vector<std::wstring> s;
					s.push_back(whereClause[i][0]);
					s.push_back(whereClause[i][1]);
					s.push_back(whereClause[i][2]);
					condition.push_back(s);
					s.clear();
				}

				int orderx = 0;
				if (_order == L"ASC") {
					orderx = 1;
				}
				else if (_order == L"DESC") {
					orderx = 2;
				}
				std::wstring name = _orderBy;

				formSet[foundForm].Select(columns, condition, orderx, name);
				return;
			}

		}



		void Delete(
			std::wstring _tableName, 
			std::vector<std::vector<std::wstring>> _whereClause) {
			
			std::wstring formName = _tableName;
			std::vector<std::vector<std::wstring>> whereClause = _whereClause;
			int foundForm = -1;
			//检查表是否存在
			bool if_found = false;
			for (unsigned int i = 0; i < formSet.size(); i++) {
				if (formSet[i].getFormName() == formName) {
					if_found = true;
					foundForm = i;
					break;
				}
			}
			if (!if_found) {
				std::wcout << L"所要删除的表不存在。" << std::endl;
				return;
			}

			//删除全表
			if (whereClause.size() == 0) {
				formSet[foundForm].Delete();
			}
			//删除部分记录(支持多条件)
			else {
				bool keyflag = false;
	
				int keycolumn = -1;

				std::vector<std::vector<std::wstring>> _copy = formSet[foundForm].getFormHeader();
				
				for (unsigned int i = 0; i < whereClause.size(); i++) {
					keyflag = false;
					for (unsigned int j = 0; j < _copy.size(); j++) {
						if (whereClause[i][0] == _copy[j][0]) {
							keyflag = true;
							break;
						}
					}
					if (!keyflag) {
						std::wcout << L"错误！" << whereClause[i][0]
							<< L"不是表" << formSet[foundForm].getFormName()
							<< L"的成员。" << std::endl;
						return;
					}
				}
				if (!keyflag) {
					return;
				}
				

				std::vector<std::vector<std::wstring>> condition;
				for (unsigned int i = 0; i < whereClause.size(); i++) {
					std::vector<std::wstring> s;
					s.push_back(whereClause[i][0]);
					s.push_back(whereClause[i][1]);
					s.push_back(whereClause[i][2]);
					condition.push_back(s);
					s.clear();
				}
			
				std::wstring relation = L"AND";
				int affected = formSet[foundForm].Delete(condition, relation);

				if (affected == 0) {
					std::wcout << L"删除失败，没有符合条件的记录" << std::endl;
				}
			}
		}



		void Update(
			std::wstring _tableName,
			std::vector<std::vector<std::wstring>> _sets,
			std::vector<std::vector<std::wstring>> _whereClause) {
			
			std::wstring tableName = _tableName;
			std::vector<std::vector<std::wstring>> sets = _sets;
			std::vector<std::vector<std::wstring>> whereClause = _whereClause;

			int foundForm = -1;
			for (unsigned int i = 0; i < formSet.size(); i++) {
				if (formSet[i].getFormName() == tableName) {
					foundForm = i;
					break;
				}
			}

			if (foundForm == -1) {
				std::wcout << L"错误！所要操作的表不存在。" << std::endl;
				return;
			}

			std::vector<std::vector<std::wstring>> set;
			std::vector<std::vector<std::wstring>> condition;
			for (unsigned int i = 0; i < sets.size(); i++) {
				std::vector<std::wstring> s;
				s.push_back(sets[i][0]);
				s.push_back(sets[i][1]);
				set.push_back(s);
				s.clear();
			}

			for (unsigned int i = 0; i < whereClause.size(); i++) {
				std::vector<std::wstring> s;
				s.push_back(whereClause[i][0]);
				s.push_back(whereClause[i][1]);
				s.push_back(whereClause[i][2]);
				condition.push_back(s);
				s.clear();
			}

			formSet[foundForm].Update(set, condition);

		}


		void Use(std::wstring dbName) {

		}

		bool saveForm(dataprocessor::Form form) {
			std::wstring fileName = L".\\";
			fileName = fileName + DBName + L"\\" + form.getFormName();

			std::ofstream fout(wcscat((wchar_t*)fileName.data(), L".dat"), std::ios::binary);
			boost::archive::binary_oarchive oa(fout);

			oa << form.getFormName();
			oa << form.getIdTotal();
			oa << form.getKeyTotal();
			oa << form.getNull();
			oa << form.getFull();
			oa << form.getFormHeader().size();
			for (int i = 0; i < form.getFormHeader().size(); i++) {
				oa << form.getFormHeader().at(i);
			}

			std::map<int, dataprocessor::Record> records = form.getForm();
			oa << records.size();
			std::map<int, dataprocessor::Record>::iterator map_it;
			for (map_it = records.begin(); map_it != records.end(); map_it++) {//start of map
				int temp = map_it->first;
				oa << temp;
				dataprocessor::Record	record = map_it->second;
				oa << record.getId();

				std::vector<dataprocessor::Key*> singleRecord = record.getRecord();
				oa << singleRecord.size();
				std::vector<dataprocessor::Key*>::iterator vector_it;
				for (vector_it = singleRecord.begin(); vector_it != singleRecord.end(); vector_it++) {//start of vector
					dataprocessor::Key* temp_key = *vector_it;
					oa << temp_key->getPrimaryStatus();
					oa << temp_key->getNotNullStatus();
					oa << temp_key->getKeyName();
					std::wstring typeName = temp_key->getTypeName();
					oa << typeName;
					oa << temp_key->isEmpty;// save Key isEmpty;
					if (typeName == L"int") {// IntKey
						dataprocessor::IntKey* temp_intkey = (dataprocessor::IntKey*)temp_key;
						oa << temp_intkey->getData();
					}
					else if (typeName == L"string") {// StringKey
						dataprocessor::StringKey* temp_stringkey = (dataprocessor::StringKey*)temp_key;
						oa << temp_stringkey->getData();
						oa << temp_stringkey->getLengthLimit();
					}
				}//end of vector
				fout.flush();
			}// end of map
			fout.close();
			return true;
		}

		dataprocessor::Form loadForm(std::wstring formName) {

			std::wstring fileName = L".\\";
			fileName = fileName + DBName + L"\\" + formName;

			dataprocessor::Form form;
			std::ifstream fin;
			fin.open(wcscat((wchar_t*)fileName.data(), L".dat"), std::ios::binary);
			boost::archive::binary_iarchive ia(fin);
			int temp_int;
			std::wstring temp_wstring;
			bool temp_bool;
			ia >> temp_wstring;
			form.setFormName(temp_wstring); //set Form formName
			ia >> temp_int;
			form.setIdTotal(temp_int);//set Form idTotal
			ia >> temp_int;
			form.setKeyTotal(temp_int);//set Form keyTotal
			ia >> temp_bool;
			form.setNull(temp_bool);//set Form ifNull
			ia >> temp_bool;
			form.setFull(temp_bool);//set Form ifFull

			int formHeaderSize;
			ia >> formHeaderSize;
			std::vector<std::vector<std::wstring>> formHeader;
			for (int i = 0; i < formHeaderSize; i++) {
				std::vector<std::wstring> singleFormHeader;
				ia >> singleFormHeader;
				formHeader.push_back(singleFormHeader);
			}
			form.setFormHeader(formHeader);//set Form formHeader

			int recordsSize;
			std::map<int, dataprocessor::Record> records;
			ia >> recordsSize;
			for (int i = 0; i < recordsSize; i++) {
				int map_id = 0;
				ia >> map_id;
				dataprocessor::Record record;
				int record_id;
				ia >> record_id;
				record.setId(record_id);// set Record id
				int singleRecordSize;
				std::vector<dataprocessor::Key*> singleRecord;
				ia >> singleRecordSize;
				for (int i = 0; i < singleRecordSize; i++) {
					bool key_iPK;
					bool key_iNN;
					bool key_iE;
					std::wstring key_keyName;
					std::wstring key_typeName;
					ia >> key_iPK; // set Key ifPrimaryKey
					ia >> key_iNN; // set Key ifNotNull
					ia >> key_keyName; // set Key keyName
					ia >> key_typeName;// set Key typeName
					ia >> key_iE; // set Key isEmpty
					if (key_typeName == L"int") {
						int intkey_data;
						ia >> intkey_data;// set IntKey data
						dataprocessor::IntKey* intkey = new dataprocessor::IntKey(
							key_keyName,
							key_typeName,
							key_iPK, key_iNN,
							intkey_data);
						intkey->isEmpty = key_iE;
						singleRecord.push_back(intkey);// push_back IntKey* to Record vector<Key*>
					}
					else if (key_typeName == L"string") {
						std::wstring stringkey_data;
						int stringkey_LengthLimit;

						ia >> stringkey_data;// set StringKey data
						ia >> stringkey_LengthLimit;// set StringKey LengthLimit
						dataprocessor::StringKey* stringkey = new dataprocessor::StringKey(
							key_keyName,
							key_typeName,
							key_iPK, key_iNN,
							stringkey_LengthLimit,
							stringkey_data);
						stringkey->isEmpty = key_iE;
						singleRecord.push_back(stringkey);// push_back StringKey* to Record vector<Key*>
					}
				}
				record.setRecord(singleRecord);// set Record vector<Key*>
				records.insert(std::pair<int, dataprocessor::Record>(map_id, record));// insert Record to Form map<int, Record>
			}
			form.setForm(records);//set Form form

			return form;
		}

		bool saveFormName() {
			std::wstring commandName = L"md .\\";
			commandName = commandName + DBName;
			_wsystem(commandName.data());

			std::wstring formNameList = L".\\";
			formNameList = formNameList + DBName + L"\\formNameList.dat";

			std::ofstream fout;
			fout.open(formNameList, std::ios::binary);
			boost::archive::binary_oarchive oa(fout);
			oa << formSet.size();
			std::vector<dataprocessor::Form>::iterator it;
			for (it = formSet.begin(); it != formSet.end(); it++) {
				oa << it->getFormName();
			}
			fout.close();
			return true;
		}

		std::vector<std::wstring> loadFormName() {
			std::wstring formNameList = L".\\";
			formNameList = formNameList + DBName + L"\\formNameList.dat";

			std::ifstream fin;
			fin.open(formNameList, std::ios::binary);
			boost::archive::binary_iarchive ia(fin);
			int formSetSize;
			ia >> formSetSize;
			std::wstring wstr;
			std::vector<std::wstring> v;
			for (int i = 0; i < formSetSize; i++) {
				ia >> wstr;
				v.push_back(wstr);
			}
			return v;
		}

	protected:

	private:
		std::vector<dataprocessor::Form> formSet;
		std::wstring DBName;
	};
}

