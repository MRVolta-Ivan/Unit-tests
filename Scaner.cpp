#include "Scaner.h"

inline bool Scaner::IsIdent(char symbol)
{
	return symbol >= 'a' && symbol <= 'z' ||
		symbol >= 'A' && symbol <= 'Z' ||
		symbol == '_';
}

inline bool Scaner::IsNumber10(char symbol)
{
	return symbol >= '0' && symbol <= '9';
}

inline bool Scaner::IsNumber16(char symbol)
{
	return symbol >= '0' && symbol <= '9' ||
		symbol >= 'A' && symbol <= 'F' ||
		symbol >= 'a' && symbol <= 'f';
}

inline bool Scaner::IsNumber8(char symbol)
{
	return symbol >= '0' && symbol <= '7';
}

inline bool Scaner::IsNumber2(char symbol)
{
	return symbol >= '0' && symbol <= '1';
}

void Scaner::Reset()
{
	uk = 0;
	uk_start_string = uk;
	count_string = 0;
}

void Scaner::PutUK(int i)
{
	uk = i;
}

int Scaner::GetUK()
{
	return uk;
}

int Scaner::GetUKfromStartString()
{
	return uk - uk_start_string;
}

int Scaner::GetLine()
{
	return count_string;
}

void Scaner::PutLine(int line)
{
	count_string = line;
	uk_start_string = uk;
}

void Scaner::PrintError(int i)
{
	//Печатает ошибки по коду.
	switch (i)
	{
	case 1:
		cout << "Ошибка. Слишком много знаков в символьной константе." << endl;
		break;

	case 2:
		cout << "Ошибка. Не правильная операция !=." << endl;
		break;

	case 3:
		cout << "Ошибка. Недопустимое шестнадцатеричное число." << endl;
		break;

	case 4:
		cout << "Ошибка. Недопустимое двоичное число." << endl;
		break;

	case 5:
		cout << "Ошибка. Непонятный символ." << endl;
		break;
	}

	cout << "Строка - " << count_string + 1 << ", позиция - " << uk - uk_start_string << "." << endl;
}

int Scaner::Scan(string &l)
{
	int typ;
	l.clear();

	bool all_ignor = true;

	//Игнорируем символы пробела табуляции и конец строки.
	//Игнорируем комментарии однострочные и многострочные.
	while (all_ignor)
	{
		if (uk == text.size())
			return Tend;

		if (text[uk] == '\t' ||
			text[uk] == ' ')
			uk++;
		else if (text[uk] == '\n')
		{
			count_string++;
			uk++;
			uk_start_string = uk;
		}
		else if (text[uk] == '/')
		{
			if (text[uk + 1] == '/' || text[uk + 1] == '*')
			{
				uk++;
				if (text[uk] == '/')
				{
					uk++;
					while (text[uk] != '\n')
					{
						uk++;
						if (uk == text.size())
							return Tend;
					}
				}
				else
				{
					uk++;
					bool is_comment = true;
					while (is_comment)
					{
						if (uk == text.size())
							return Tend;
						if (text[uk] == '*')
						{
							uk++;
							if (text[uk] == '/')
								is_comment = false;
						}
						uk++;
					}
				}
			}
			else
				all_ignor = false;
		}
		else
			all_ignor = false;
	}

	//Проверяем лексему.
	switch (text[uk])
	{
	case '\'':
		l.push_back(text[uk++]);
		if (text[uk] == '\'')
			return TconstChar;
		else
		{
			l.push_back(text[uk++]);
			if (text[uk] == '\'')
			{
				l.push_back(text[uk++]);
				return TconstChar;
			}
			else
			{
				PrintError(1);
				return Terror;
			}
		}

	case '=':
		l.push_back(text[uk++]);
		if (text[uk] == '=')
		{
			l.push_back(text[uk++]);
			return Teq;
		}
		else
			return Teval;

	case '!':
		l.push_back(text[uk++]);
		if (text[uk] == '=')
		{
			l.push_back(text[uk++]);
			return Tuneq;
		}
		else
		{
			PrintError(2);
			return Terror;
		}

	case '>':
		l.push_back(text[uk++]);
		if (text[uk] == '=')
		{
			l.push_back(text[uk++]);
			return TmoreOrEq;
		}
		else
			return Tmore;

	case '<':
		l.push_back(text[uk++]);
		if (text[uk] == '=')
		{
			l.push_back(text[uk++]);
			return TlessOrEq;
		}
		else
			return Tless;

	case '+':
		l.push_back(text[uk++]);
		if (text[uk] == '+')
		{
			l.push_back(text[uk++]);
			return Tincrement;
		}
		else
			return Tplus;

	case '-':
		l.push_back(text[uk++]);
		if (text[uk] == '-')
		{
			l.push_back(text[uk++]);
			return Tdecrement;
		}
		else
			return Tminus;

	case '*':
		l.push_back(text[uk++]);
		return Tmul;

	case '/':
		l.push_back(text[uk++]);
		return Tdiv;

	case '%':
		l.push_back(text[uk++]);
		return Tremain;

	case ',':
		l.push_back(text[uk++]);
		return Tcomma;

	case ';':
		l.push_back(text[uk++]);
		return Tsemi;

	case '(':
		l.push_back(text[uk++]);
		return TopenBracket;

	case ')':
		l.push_back(text[uk++]);
		return TcloseBracket;

	case '{':
		l.push_back(text[uk++]);
		return TopenBrace;

	case '}':
		l.push_back(text[uk++]);
		return TcloseBrace;

	case '[':
		l.push_back(text[uk++]);
		return TopenSquareBracket;

	case ']':
		l.push_back(text[uk++]);
		return TcloseSquareBracket;

	case ':':
		l.push_back(text[uk++]);
		return  Tcolon;

	default:
		//проверяем идентификатор.
		if (IsIdent(text[uk]))
		{
			l.push_back(text[uk++]);
			while (IsIdent(text[uk]))
				l.push_back(text[uk++]);

			map<string, int>::iterator it = keywords.find(l);

			if (it == keywords.end())
				return Tident;
			else
				return keywords[l];
		}
		else
		//Проверяем константы целого типа.
		{
			if (text[uk] == '0')
			{
				l.push_back(text[uk++]);
				if (text[uk] == 'x')
				{
					l.push_back(text[uk++]);
					while (IsNumber16(text[uk]))
					{
						l.push_back(text[uk++]);
					}
					if (l[l.size()-1] == 'x')
					{
						PrintError(3);
						return Terror;
					}
					else
						return TconstInt16;
				}
				else if (text[uk] == 'b')
				{
					l.push_back(text[uk++]);
					while (IsNumber2(text[uk]))
					{
						l.push_back(text[uk++]);
					}
					if (l[l.size() - 1] == 'b')
					{
						PrintError(4);
						return Terror;
					}
					else
						return TconstInt2;
				}
				else
				{
					if (IsNumber8(text[uk]))
					{
						l.push_back(text[uk++]);
						while (IsNumber8(text[uk]))
							l.push_back(text[uk++]);
						return TconstInt8;
					}
					else
						return TconstInt10;
				}
			}
			else
			{
				if (IsNumber10(text[uk]))
				{
					l.push_back(text[uk++]);
					while (IsNumber10(text[uk]))
						l.push_back(text[uk++]);

					return TconstInt10;
				}
				else
				{
					l.push_back(text[uk++]);
					PrintError(5);
					return Terror;
				}
			}
		}
	}
}

void Scaner::GetData(string file_name)
{
	ifstream input_file(file_name);

	text = "";

	char sign;

	while (!input_file.eof())
	{
		sign = input_file.get();
		text += sign;
	}

	text.pop_back();

	uk = 0;
	uk_start_string = uk;
	count_string = 0;

	input_file.close();

}
