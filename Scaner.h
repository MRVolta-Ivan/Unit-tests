#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include "defs.h"

using namespace std;

class Scaner
{
private:
	string text; //����������� �����.
	int count_string = 0; //������� ������.
	int uk_start_string = 0; //��������� ������ ������� ������.
	int uk = 0; //��������� �� ��������� � ������.

	 map<string, int> keywords = {
	{"int", Tint},
	{"short", Tshort},
	{"long", Tlong},
	{"boolean", Tboolean},
	{"public", Tpublic},
	{"class", Tclass},
	{"Main", TMain},
	{"static", Tstatic},
	{"void", Tvoid},
	{"main", Tmain},
	{"new", Tnew},
	{"switch", Tswitch},
	{"case", Tcase},
	{"default", Tdefault},
	{"break", Tbreak},
	{"true", Ttrue},
	{"false", Tfalse}}; //�������� �����.

	inline bool IsIdent(char symbol); //����������� �� ������ ������ � _.
	inline bool IsNumber10(char symbol); //����������� �� ������ ������ 10 �/�.
	inline bool IsNumber16(char symbol); //����������� �� ������ ������ 16 �/�.
	inline bool IsNumber8(char symbol); //����������� �� ������  ������ 8 �/�.
	inline bool IsNumber2(char symbol); //����������� �� ������ ������ 2 �/�.

public:
	void Reset();
	void PutUK(int i); //���������� ���������.
	int GetUK(); //�������� ��������� ���������.
	int GetUKfromStartString();
	int GetLine();
	void PutLine(int line);
	void PrintError(int i); //���������� ������.
	int Scan(string &l); //������������� ������.
	void GetData(string file_name); //��������� ����� ��� ������� �� �����.

	void SetText(string _text) { text = _text; uk = 0; count_string = 0; }
};
