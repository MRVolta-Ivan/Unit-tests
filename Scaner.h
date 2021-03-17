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
	string text; //Сканирующий текст.
	int count_string = 0; //Текущая строка.
	int uk_start_string = 0; //Указатель начала текущей строки.
	int uk = 0; //Указатель на положение в тексте.

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
	{"false", Tfalse}}; //Ключевые слова.

	inline bool IsIdent(char symbol); //Принадлежит ли символ буквам и _.
	inline bool IsNumber10(char symbol); //Принадлижит ли символ цифрам 10 с/с.
	inline bool IsNumber16(char symbol); //Принадлежит ли символ цифрам 16 с/с.
	inline bool IsNumber8(char symbol); //Принадлежит ли символ  цифрам 8 с/с.
	inline bool IsNumber2(char symbol); //Принадлежит ли символ цифрам 2 с/с.

public:
	void Reset();
	void PutUK(int i); //Установить указатель.
	int GetUK(); //Получить положение указателя.
	int GetUKfromStartString();
	int GetLine();
	void PutLine(int line);
	void PrintError(int i); //Напечатать ошибку.
	int Scan(string &l); //Сканированние текста.
	void GetData(string file_name); //Прочитать текст для сканера из файла.

	void SetText(string _text) { text = _text; uk = 0; count_string = 0; }
};
