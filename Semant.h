#pragma once
#include "defs.h"
#include <string>
#include <iostream>
#include <vector>

using namespace std;

enum TypeNode
{
	VVAR,
	CCONSTANT
};

enum DataType 
{
	IINT,
	BBOOLEAN,
	UNKNOWN
};

union DataValue
{
	int dataAsInt;
	bool dataAsBoolean;
	int* addrAsInt;
	bool* addrAsBoolean;
};

class Node
{
public:
	Node(TypeNode _typenode, string lex, DataType _type, DataValue _value, bool field, bool init, int numDim, vector<int> dim);
	Node();

	TypeNode typenode;

	string lexem; //�������������.
	DataType type; //��� ����������.
	DataValue value; //��������
	bool isField; //��� ����/�������� ������.
	bool isInitialization; //���������� �����������������.
	int numberDimension; //����� ������������ �������.
	vector<int> dimension; //����������� �������.
};

class Tree
{
private:
	Node *node;
	Tree *up;
	Tree *left;
	Tree *right;
	static Tree* cur; //������� ������� ������.
public:
	Tree(Tree *u, Tree *l, Tree *r, Node *data);
	Tree();
	~Tree();

	void SetLeft(Node *data);
	void SetRight(Node *data);
	Tree* FindUp(Tree *from, string lex);
	Tree* FindUp(string lex);
	Tree* FindRightLeft(Tree* from, string lex);
	Tree* FindRightLeft(string lex);
	Tree* FindUpOneBlock(Tree* from, string lex, bool field);
	void Print();

	void PrintDetail();

	void PrintError(string description, string lex);
	void PrintError(string description, string lex, DataType typeData, int number);

	//������� ������������� � ������.
	Tree* SemInclude(string lex, int intype, DataValue _value, bool field, bool init, int numDim, vector<int> _dimension);
	//������� ������ ����.
	Tree* SemInclude();
	//����� � ������ �������������.
	Tree* SemGetVar(string lex, int numD);
	Tree* SemGetVar(string lex, int numD, vector<int> dim);
	//���������� ������� ������������� � ��������� ��� ������.
	void SetInit(Node* _node);
	//���������� ����� ������������.
	void SetNumDimension(int numDim);
	//���������� �����������.
	void SetDimension(vector<int> dim);
	//�������� �������������� �� ��������� ����������.
	bool DupControl(Tree* from, string lex, bool field);

	//���������� ������� ���� ������.
	void SetCur(Tree* t);
	//�������� ������� ���� ������.
	Tree* GetCur();

	Node* GetNode();
};

