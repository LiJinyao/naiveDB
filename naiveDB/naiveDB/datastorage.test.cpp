#include "stdafx.h"
#include "DataBase.hpp"

using namespace naiveDB::dataprocessor;
using namespace naiveDB;

void datastorageTest() {

	//IntKey* intkey1 = new IntKey(L"test_intkey1", L"int", false, false, 1);
	//StringKey* stringkey1 = new StringKey(L"test_stringkey1", L"string", false, true, 111, L"first");
	//IntKey* intkey2 = new IntKey(L"test_intkey2", L"int", true, false, 2);
	//StringKey* stringkey2 = new StringKey(L"test_stringkey2", L"string", true, true, 222, L"second");

	//Record record1;
	//record1.setId(1);
	//std::vector<Key*> v1;
	//v1.push_back(intkey1);
	//v1.push_back(stringkey1);
	//record1.setRecord(v1);

	//Record record2;
	//record2.setId(2);
	//std::vector<Key*> v2;
	//v2.push_back(intkey2);
	//v2.push_back(stringkey2);
	//record2.setRecord(v2);

	//Form form;
	//form.setFormName(L"test_form");
	//form.setIdTotal(233);
	//form.setKeyTotal(666);
	//form.setFull(false);
	//form.setNull(false);
	//std::map<int, Record> m;
	//m.insert(std::pair<int, Record>(1, record1));
	//m.insert(std::pair<int, Record>(2, record2));
	//form.setForm(m);
	//std::vector<std::vector<std::wstring>> vouter;
	//std::vector<std::wstring> vinner1;
	//std::vector<std::wstring> vinner2;
	//vinner1.push_back(L"BAD");
	//vinner1.push_back(L"ASS");
	//vinner2.push_back(L"MOTHER");
	//vinner2.push_back(L"FUCKER");
	//vouter.push_back(vinner1);
	//vouter.push_back(vinner2);
	//form.setFormHeader(vouter);

	Form form;
	form.setFormName(L"test_form");
	form.setIdTotal(233);
	form.setKeyTotal(666);
	form.setFull(false);
	form.setNull(false);
	std::map<int, Record> m;
	for (int i = 1; i <= 10000; i++) {
		Record record;
		record.setId(i);
		std::vector<Key*> record_v;
		for (int j = 1; j <= 10; j++) {
			if (j <= 5) {
				IntKey* intkey1 = new IntKey(L"test_intkey1", L"int", false, false, 1);
				record_v.push_back(intkey1);
			}
			else {
				StringKey* stringkey2 = new StringKey(L"test_stringkey2", L"string", true, true, 222, L"second");
				record_v.push_back(stringkey2);
			}
		}
		record.setRecord(record_v);
		m.insert(std::pair<int, Record>(i, record));
	}
	form.setForm(m);
	std::vector<std::vector<std::wstring>> vouter;
	std::vector<std::wstring> vinner1;
	std::vector<std::wstring> vinner2;
	vinner1.push_back(L"BAD");
	vinner1.push_back(L"ASS");
	vinner2.push_back(L"MOTHER");
	vinner2.push_back(L"FUCKER");
	vouter.push_back(vinner1);
	vouter.push_back(vinner2);
	form.setFormHeader(vouter);

	DataBase db = DataBase(L"naiveDB");

	db.saveFormName();
	db.saveForm(form);


	Form new_form;
	db.loadFormName();
	db.loadForm(form.getFormName());

	int i = 0;
}
