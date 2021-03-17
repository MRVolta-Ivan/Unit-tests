#pragma once
#include "Scaner.h"
#include "Semant.h"

class Diagram
{
private:
	Scaner *sc;
	Tree* root;

public:
	void Function(); //Функция.
	void CompoundOperator(); //Составной оператор.
	void OneDescription(bool field); //Одно описание.
	int TypeData(); //Тип данных.
	void Variable(int typeData, bool field); //Переменная.
	vector<int> ArraySize(); //Размер массива.
	void Operator(); //Оператор.
	void Assignment(); //Присваивание.
	void Switch(); //Switch.
	void Case(); //Case.
	void MultipleOperators(); //Несколько операторов.
	Node* Name(); //Имя.
	Node* Expression(); //Выражение.
	Node* Inequality(); //Неравенство.
	Node* Addendum(); //Слагамое.
	Node* Multiplier(); //Множитель.
	Node* Prefix(); //префикс.
	Node* Postfix(); //Постфикс.
	Node* ElementaryExpression(); //Элементарное выражение.
	int Constant(); //Константа.

	Diagram(Scaner *s, Tree* tree);
	~Diagram();
	void Program(); //Программа.
	void PrintError(string description, int line, int symbol);

	void SetText(string text) { sc->SetText(text); }
};

