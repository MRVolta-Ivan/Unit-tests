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

	string lexem; //Идентификатор.
	DataType type; //Тип переменной.
	DataValue value; //Значение
	bool isField; //Это поле/свойство класса.
	bool isInitialization; //Переменная инициализированна.
	int numberDimension; //Число размерностей массива.
	vector<int> dimension; //Размерности массива.
};

class Tree
{
private:
	Node *node;
	Tree *up;
	Tree *left;
	Tree *right;
	static Tree* cur; //Текущий элемент дерева.
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

	//Занести идентификатор в дерево.
	Tree* SemInclude(string lex, int intype, DataValue _value, bool field, bool init, int numDim, vector<int> _dimension);
	//Создать пустой узел.
	Tree* SemInclude();
	//Найти в дереве идентификатор.
	Tree* SemGetVar(string lex, int numD);
	Tree* SemGetVar(string lex, int numD, vector<int> dim);
	//Установить признак инициализации и проверить тип данных.
	void SetInit(Node* _node);
	//Установить число размерностей.
	void SetNumDimension(int numDim);
	//Установить размерности.
	void SetDimension(vector<int> dim);
	//Проверка идентификатора на повторное объявление.
	bool DupControl(Tree* from, string lex, bool field);

	//Установить текущий узел дерева.
	void SetCur(Tree* t);
	//Получить текущий узел дерева.
	Tree* GetCur();

	Node* GetNode();
};

