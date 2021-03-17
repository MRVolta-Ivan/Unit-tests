#include "Semant.h"

Tree* Tree::cur = nullptr;

Node::Node(TypeNode _typenode, string lex, DataType _type, DataValue _value, bool field, bool init, int numDim, vector<int> dim)
{
	typenode = _typenode;
	lexem = lex; 
	type = _type;
	value = _value;
	isField = field; 
	isInitialization = init; 
	numberDimension = numDim; 
	dimension = dim; 
}

Node::Node()
{
	lexem = "";
	type = DataType::UNKNOWN;
	value.dataAsInt = 0;
}

Tree::Tree(Tree * u, Tree * l, Tree * r, Node * data)
{
	up = u;
	left = l;
	right = r;
	node = data;
}

Tree::Tree()
{
	up = nullptr;
	left = nullptr;
	right = nullptr;
	node = new Node();
}

Tree::~Tree()
{
}

void Tree::SetLeft(Node * data)
{
	Tree* a = new Tree(this, nullptr, nullptr, data);
	left = a;
}

void Tree::SetRight(Node * data)
{
	Tree* a = new Tree(this, nullptr, nullptr, data);
	right = a;
}

Tree * Tree::FindUp(Tree * from, string lex)
{
	Tree* i = from;
	while (i != nullptr)
	{
		if (lex == i->node->lexem)
			break;
		else
			i = i->up;
	}
	return i;
}

Tree * Tree::FindUp(string lex)
{
	return FindUp(this, lex);
}

Tree * Tree::FindRightLeft(Tree * from, string lex)
{
	Tree* i = from->right;
	while (i != nullptr)
	{
		if (lex == i->node->lexem)
			break;
		else
			i = i->left;
	}
	return i;
}

Tree * Tree::FindRightLeft(string lex)
{
	return FindRightLeft(this, lex);
}

void Tree::Print()
{
	cout << "Вершина с данными " << node->lexem << " ----->" << endl;
	if (left != nullptr)
		cout << "	слева данные " << left->node->lexem << endl;
	if (right != nullptr)
		cout << "	справа данные " << right->node->lexem << endl;
	if (left != nullptr) left->Print();
	if (right != nullptr) right->Print();
}

void Tree::PrintDetail()
{
	if (node->type != DataType::UNKNOWN)
	{
		string typeStr;
		if (node->type == DataType::IINT)
			typeStr = "int";
		else
			typeStr = "boolean";

		cout << "Тип: " << typeStr << ", идентификатор: " << node->lexem;
		if (node->numberDimension > 0)
		{
			int all = 1;

			for (int i = 0; i < node->dimension.size(); i++)
				all *= node->dimension[i];

			cout << " - массив, ";
			if (node->type == DataType::IINT)
			{
				cout << "адрес массива = " << node->value.addrAsInt;
				cout << endl << "Значения записанные в массиве: | ";
				for (int i = 0; i < all; i++)
				{
					cout << node->value.addrAsInt[i] << " | ";
				}

			}
			else
			{
				cout << "адрес массива = " << node->value.addrAsBoolean;
				cout << endl << "Значения записанные в массиве: | ";
				for (int i = 0; i < all; i++)
				{
					if (node->value.addrAsBoolean[i])
						cout << "true | ";
					else
						cout << "false | ";
				}

			}

			
		}
		else
		{
			cout << " - переменная, ";
			if (node->type == DataType::IINT)
				cout << "значение = " << node->value.dataAsInt;
			else
				if (node->value.dataAsBoolean)
					cout << "значение = true";
				else
					cout << "значение = false";
		}
		cout << endl;
	}

	if (up != nullptr)
		up->PrintDetail();
}

void Tree::PrintError(string description, string lex)
{
	cout << "Ошибка! " << description << ", идентификатор: " << lex << "." << endl;
	throw "Error!";
}

void Tree::PrintError(string description, string lex, DataType typeData, int number)
{
	string errorString = description;
	switch (cur->node->type)
	{
	case DataType::IINT:
		errorString += " int";
		break;
	case DataType::BBOOLEAN:
		errorString += " boolean";
		break;
	}

	for (int i = 0; i < number; i++)
		errorString += "[]";

	cout << "Ошибка! " << errorString << ", идентификатор: " << lex << "." << endl;
	throw "Error!";
}

Tree * Tree::SemInclude(string lex, int intype, DataValue _value, bool field, bool init, int numDim, vector<int> _dimension)
{
	if (DupControl(cur, lex, field))
		PrintError("Повторное описание", lex);

	DataType _type;

	if (intype == Tint)
		_type = DataType::IINT;
	else
		_type = DataType::BBOOLEAN;

	Node* n = new Node(TypeNode::VVAR, lex, _type, _value, field, init, numDim, _dimension);

	cur->SetLeft(n);
	cur = cur->left;
	return cur;

}

Tree * Tree::SemInclude()
{
	Node *n = new Node();
	cur->SetRight(n);
	cur = cur->right;
	return cur;
}

Tree * Tree::SemGetVar(string lex, int numD)
{
	Tree* v = FindUp(cur, lex);

	if (v == nullptr)
		PrintError("Отсутствует описание идентификатора", lex);

	if (v->node->numberDimension != numD)
		PrintError("Не соответствует типу", v->node->lexem, v->node->type, v->node->numberDimension);

	return v;
}

Tree * Tree::SemGetVar(string lex, int numD, vector<int> dim)
{
	Tree* v = FindUp(cur, lex);

	if (v == nullptr)
		PrintError("Отсутствует описание идентификатора", lex);

	if (v->node->numberDimension != numD)
		PrintError("Не соответствует типу", v->node->lexem, v->node->type, v->node->numberDimension);

	for (int i = 0; i < dim.size(); i++)
	{
		if (dim[i] >= v->node->dimension[i])
			PrintError("Выход за пределы массива", v->node->lexem);
	}

	return v;
}

void Tree::SetInit(Node* _node)
{
	if (cur->node->type != _node->type)
	{
		PrintError("Ожидался тип ", cur->node->lexem, cur->node->type, 0);
	}
	cur->node->isInitialization = true;
	cur->node->value = _node->value;
}

void Tree::SetNumDimension(int numDim)
{
	cur->node->numberDimension = numDim;
}

void Tree::SetDimension(vector<int> dim)
{
	if (cur->node->numberDimension != dim.size())
		PrintError("Ожидался тип ", cur->node->lexem, cur->node->type, cur->node->numberDimension);
	cur->node->dimension = dim;
}

Tree * Tree::FindUpOneBlock(Tree * from, string lex, bool field)
{
	Tree* i = from;
	
	while ((i != nullptr))
	{
		if (i->node->lexem == lex)
		{
			if (field)
			{
				return i;
			}
			else
			{
				if (i->node->isField)
					break;
				else
					return i;
			}
		}
		i = i->up;
	}
	return nullptr;
}

bool Tree::DupControl(Tree * from, string lex, bool field)
{
	if (FindUpOneBlock(from, lex, field) == nullptr)
		return false;
	else
		return true;
}

void Tree::SetCur(Tree * t)
{
	cur = t;
}

Tree * Tree::GetCur()
{
	return cur;
}

Node * Tree::GetNode()
{
	return this->node;
}
