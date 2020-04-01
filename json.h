#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
enum vt {
	object,
	array,
	string,
	real,
	integer,
	boul,
	null
};
class variant {
public:
	void* val;
	vt type;
	std::map<std::wstring, variant>& getObject() {
		if (type == vt::object)
			return *(std::map<std::wstring, variant>*)val;
	}
	std::vector<variant>& getArray() {
		if (type == vt::array)
			return *(std::vector<variant>*)val;
	}
	std::wstring getString() {
		if (type == vt::string)
			return *(std::wstring*)val;
		else if (type == real)
			return std::to_wstring(*(double*)val);
		else if (type == integer)
			return std::to_wstring(*(int*)val);
		else if (type == vt::boul)
			return std::to_wstring(*(bool*)val);
		else if (type == vt::null)
			return std::wstring();
		
	}
	double getReal() {
		if (type == real)
			return *(double*)val;
		else if (type == integer)
			return *(int*)val;
		else if (type == vt::boul)
			return *(bool*)val;
		else if (type == vt::string)
			return std::stod(*(std::wstring*)val);
		else if (type == vt::null)
			return 0.0;
	}
	int getInt() {
		if (type == integer)
			return *(int*)val;
		else if (type == real)
			return (int)*(double*)val;
		else if (type == vt::string)
			return std::stoi(*(std::wstring*)val);
		else if (type == vt::boul)
			return (int)*(bool*)val;
		else if (type == vt::null)
			return 0;
	}
	bool getBool() {
		if (type == vt::boul)
			return *(bool*)val;
		else if (type == integer)
			return (bool)*(int*)val;
		else if (type == real)
			return (bool)*(double*)val;
		else if (type == vt::string)
			return std::stoi(*(std::wstring*)val);
		else if (type == vt::null)
			return false;
	}
	bool isNull() {
		return type == vt::null;
	}
};
#define UNICODE_BOM (0xfeff)
class IReader {
public:

	virtual int getc() = 0;
	virtual void ungetc() = 0;
	virtual void close() = 0;

	bool isError;

	/**
	 * �R���X�g���N�^
	 */
	IReader();

	virtual ~IReader();;

	/**
	 * �s���܂œǂݔ�΂�
	 */
	void toEOL();

	/**
	 * �󔒂ƃR�����g���������Ď��̕�����Ԃ�
	 */
	int next();

	/**
	 * �w�肳�ꂽ���������̕�������擾
	 * @param str ������̊i�[��
	 * @param n ������
	 */
	void next(std::wstring& str, int n);

	void parseObject(variant& var);

	void parseArray(variant& var);

	/**
	 * �N�I�[�g������̃p�[�X
	 * @param quote �N�I�[�g����
	 * @param var �i�[��
	 */
	void parseQuoteString(int quote, variant& var);

	/**
	 * �w�肵�����������l�̂P�����ڂ̍\���v�f���ǂ���
	 */
	bool isNumberFirst(int ch);

	/**
	 * �w�肵�����������l�̍\���v�f���ǂ���
	 */
	bool isNumber(int ch);

	/**
	 * �w�肵��������������̍\���v�f���ǂ���
	 */
	bool isString(int ch);

	/**
	 * �p�[�X�̎��s
	 * @param var ���ʊi�[��
	 */
	void parse(variant& var);
};


class IStringReader : public IReader {

	std::wstring dat;
	const wchar_t* p;
	unsigned long length;
	unsigned long pos;

public:
	IStringReader(const wchar_t* str);

	void close();

	int getc();

	void ungetc();
};
void eval(IReader& file, variant* result);
variant evalJson(std::wstring str);