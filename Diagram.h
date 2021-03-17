#pragma once
#include "Scaner.h"
#include "Semant.h"

class Diagram
{
private:
	Scaner *sc;
	Tree* root;

public:
	void Function(); //�������.
	void CompoundOperator(); //��������� ��������.
	void OneDescription(bool field); //���� ��������.
	int TypeData(); //��� ������.
	void Variable(int typeData, bool field); //����������.
	vector<int> ArraySize(); //������ �������.
	void Operator(); //��������.
	void Assignment(); //������������.
	void Switch(); //Switch.
	void Case(); //Case.
	void MultipleOperators(); //��������� ����������.
	Node* Name(); //���.
	Node* Expression(); //���������.
	Node* Inequality(); //�����������.
	Node* Addendum(); //��������.
	Node* Multiplier(); //���������.
	Node* Prefix(); //�������.
	Node* Postfix(); //��������.
	Node* ElementaryExpression(); //������������ ���������.
	int Constant(); //���������.

	Diagram(Scaner *s, Tree* tree);
	~Diagram();
	void Program(); //���������.
	void PrintError(string description, int line, int symbol);

	void SetText(string text) { sc->SetText(text); }
};

