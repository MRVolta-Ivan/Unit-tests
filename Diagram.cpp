#include "Diagram.h"

Diagram::Diagram(Scaner *s, Tree* _tree)
{
	sc = s;
	root = _tree;
}

Diagram::~Diagram()
{
}

void Diagram::Program() //���������.
{
	int type;
	string lexem = "";
	type = sc->Scan(lexem);
	if (type != Tpublic)
		PrintError("��������� public.", sc->GetLine() + 1, sc->GetUKfromStartString());
	type = sc->Scan(lexem);
	if (type != Tclass)
		PrintError("��������� class.", sc->GetLine() + 1, sc->GetUKfromStartString());
	type = sc->Scan(lexem);
	if (type != TMain)
		PrintError("��������� Main.", sc->GetLine() + 1, sc->GetUKfromStartString());
	type = sc->Scan(lexem);
	if (type != TopenBrace)
		PrintError("�������� ������ {.", sc->GetLine() + 1, sc->GetUKfromStartString());
	int uk = sc->GetUK();
	int line = sc->GetLine();
	type = sc->Scan(lexem);
	sc->PutLine(line);
	sc->PutUK(uk);
	//���� ��������. �� �������� �� ���� ��������, ���� ��������� int, short, long, boolean.
	while (type >= Tint && type <= Tboolean)
	{
		OneDescription(true);
		uk = sc->GetUK();
		line = sc->GetLine();
		type = sc->Scan(lexem);
		sc->PutUK(uk);
		sc->PutLine(line);
	}
	Function();
	type = sc->Scan(lexem);
	if (type != TcloseBrace)
		PrintError("�������� ������ }.", sc->GetLine() + 1, sc->GetUKfromStartString());
}

void Diagram::Function() //�������.
{
	int type;
	string lexem = "";
	type = sc->Scan(lexem);
	if (type != Tpublic)
		PrintError("��������� public.", sc->GetLine() + 1, sc->GetUKfromStartString());
	type = sc->Scan(lexem);
	if (type != Tstatic)
		PrintError("��������� static.", sc->GetLine() + 1, sc->GetUKfromStartString());
	type = sc->Scan(lexem);
	if (type != Tvoid)
		PrintError("��������� void.", sc->GetLine() + 1, sc->GetUKfromStartString());
	type = sc->Scan(lexem);
	if (type != Tmain)
		PrintError("��������� main.", sc->GetLine() + 1, sc->GetUKfromStartString());
	type = sc->Scan(lexem);
	if (type != TopenBracket)
		PrintError("�������� ������ (.", sc->GetLine() + 1, sc->GetUKfromStartString());
	type = sc->Scan(lexem);
	if (type != TcloseBracket)
		PrintError("�������� ������ ).", sc->GetLine() + 1, sc->GetUKfromStartString());
	CompoundOperator();
}

void Diagram::CompoundOperator() //��������� ��������.
{
	int type;
	string lexem = "";
	type = sc->Scan(lexem);
	if (type != TopenBrace)
		PrintError("�������� ������ {.", sc->GetLine() + 1, sc->GetUKfromStartString());
	
	Tree* save = root->GetCur();

	root->SemInclude(); //������ ������ ������� ������ �� �������� ����. 
	
	int uk = sc->GetUK();
	int line = sc->GetLine();
	type = sc->Scan(lexem);
	sc->PutUK(uk);
	sc->PutLine(line);
	//���� �������� ��� ��������.

	cout << "������ ����� ������ � ����:" << endl;
	root->GetCur()->PrintDetail();
	cout << endl << endl;

	while (type != TcloseBrace && type < Terror)
	{
		if (type >= Tint && type <= Tboolean)
			OneDescription(false);
		else
			Operator();
		uk = sc->GetUK();
		line = sc->GetLine();
		type = sc->Scan(lexem);
		sc->PutUK(uk);
		sc->PutLine(line);

	}

	type = sc->Scan(lexem);
	if (type != TcloseBrace)
		PrintError("�������� ������ }.", sc->GetLine() + 1, sc->GetUKfromStartString());

	cout << "������ ����� ������ �� �����:" << endl;
	root->GetCur()->PrintDetail();
	cout << endl << endl;

	root->SetCur(save);

}

void Diagram::OneDescription(bool field) //���� ��������.
{
	int type;
	string lexem = "";

	type = TypeData();

	int typeVar = type; //���������� ��� ����������.

	do
	{
		Variable(typeVar, field);
		type = sc->Scan(lexem);
	} while (type == Tcomma);
	if (type != Tsemi)
		PrintError("�������� ������ ;.", sc->GetLine() + 1, sc->GetUKfromStartString());
}

int Diagram::TypeData() //��� ������.
{
	int type;
	string lexem = "";
	type = sc->Scan(lexem);
	if (!(type >= Tint && type <= Tboolean))
		PrintError("�������� ��� ������: int, short, long, boolean.", sc->GetLine() + 1, sc->GetUKfromStartString());
	return type;
}

void Diagram::Variable(int typeData, bool field) //����������.
{
	int type;
	string lexem = "";
	type = sc->Scan(lexem);
	if (type != Tident)
		PrintError("�������� �������������.", sc->GetLine() + 1, sc->GetUKfromStartString());

	string nameIdent = lexem; //���������� ��� ����������.

	int uk = sc->GetUK();
	int line = sc->GetLine();
	type = sc->Scan(lexem);

	DataValue value;
	if (typeData == Tint)
		value.dataAsInt = 0;
	else
		value.dataAsBoolean = false;

	//������� ���������� � ������.
	root->SemInclude(nameIdent, typeData, value, field, false, 0, {});

	//������������� ����������.
	if (type == Teval)
	{
		Node* node = Expression();

		root->SetInit(node); //������������� ������� �������������.
		
		cout << "�������������:" << endl;
		root->GetCur()->PrintDetail();
		cout << endl << endl;
	}
	//�������� �������.
	else if (type == TopenSquareBracket)
	{
		int countDim = 1; //�������� ������� ���������� ��������� � �������.

		type = sc->Scan(lexem);
		if (type != TcloseSquareBracket)
			PrintError("�������� ������ ].", sc->GetLine() + 1, sc->GetUKfromStartString());
		type = sc->Scan(lexem);
		while (type == TopenSquareBracket)
		{
			countDim++;

			type = sc->Scan(lexem);
			if (type != TcloseSquareBracket)
				PrintError("�������� ������ ].", sc->GetLine() + 1, sc->GetUKfromStartString());
			type = sc->Scan(lexem);
		}

		root->SetNumDimension(countDim); //������������� ���-�� ���������.

		if (type != Teval)
			PrintError("�������� ������ =.", sc->GetLine() + 1, sc->GetUKfromStartString());
		type = sc->Scan(lexem);
		if (type != Tnew)
			PrintError("��������� new.", sc->GetLine() + 1, sc->GetUKfromStartString());
		type = TypeData();

		Node* node = new Node();
		if (type == Tint)
			node->type = DataType::IINT;
		else
			node->type = DataType::BBOOLEAN;

		vector<int> dim = ArraySize();

		//������������� ������ �������.
		int allSize = 1;
		for (int i = 0; i < dim.size(); i++)
		{
			allSize *= dim[i];
		}

		if (typeData == Tint)
		{
			int* arr = new int[allSize];
			for (int i = 0; i < allSize; i++)
				arr[i] = 0;
			node->value.addrAsInt = arr;
		}
		else
		{
			bool* arr = new bool[allSize];
			for (int i = 0; i < allSize; i++)
				arr[i] = false;
			node->value.addrAsBoolean = arr;
		}

		root->SetDimension(dim); //������������� ���� ���������.
		root->SetInit(node);

		cout << "������������� �������:" << endl;
		root->GetCur()->PrintDetail();
		cout << endl << endl;
	}
	else
	{
		sc->PutUK(uk);
		sc->PutLine(line);
	}
}

vector<int> Diagram::ArraySize() //������ �������.
{
	int type;
	string lexem = "";
	int uk, line;

	vector<int> dim;

	do
	{
		type = sc->Scan(lexem);
		if (type != TopenSquareBracket)
			PrintError("�������� ������ [.", sc->GetLine() + 1, sc->GetUKfromStartString());
		
		dim.push_back(Constant());
		
		
		type = sc->Scan(lexem);
		if (type != TcloseSquareBracket)
			PrintError("�������� ������ ].", sc->GetLine() + 1, sc->GetUKfromStartString());
		uk = sc->GetUK();
		line = sc->GetLine();
		type = sc->Scan(lexem);
		sc->PutUK(uk);
		sc->PutLine(line);
	}
	while (type == TopenSquareBracket);

	return dim; //
}

void Diagram::Operator() //��������.
{
	int type;
	string lexem = "";
	int uk = sc->GetUK();
	int line = sc->GetLine();
	type = sc->Scan(lexem);
	//������������.
	if (type == Tident)
	{
		sc->PutUK(uk);
		sc->PutLine(line);
		Assignment();
		type = sc->Scan(lexem);
		if (type != Tsemi)
			PrintError("�������� ������ ;.", sc->GetLine() + 1, sc->GetUKfromStartString());
	}
	//�������� break.
	else if (type == Tbreak)
	{
		type = sc->Scan(lexem);
		if (type != Tsemi)
			PrintError("�������� ������ ;.", sc->GetLine() + 1, sc->GetUKfromStartString());
	}
	//Switch.
	else if (type == Tswitch)
	{
		sc->PutUK(uk);
		sc->PutLine(line);
		Switch();
	}
	//��������� ��������.
	else if (type == TopenBrace)
	{
		sc->PutUK(uk);
		sc->PutLine(line);
		CompoundOperator();
	}
	//��������� ������ ��������.
	else if (type != Tsemi)
		PrintError("�������� ������ ;.", sc->GetLine() + 1, sc->GetUKfromStartString());

}

void Diagram::Assignment() //������������.
{
	int type;
	string lexem = "";

	Node* nodename = Name();
	type = sc->Scan(lexem);
	if (type != Teval)
		PrintError("�������� ������ =.", sc->GetLine() + 1, sc->GetUKfromStartString());
	
	Node* original;

	original = root->SemGetVar(nodename->lexem, nodename->numberDimension)->GetNode();

	Node* node = Expression();

	if (original->type != node->type)
		root->PrintError("�� ������������ �����",nodename->lexem);

	if (original->numberDimension == 0)
		original->value = node->value;
	else
	{
		if (original->type == DataType::IINT)
			*(nodename->value.addrAsInt) = node->value.dataAsInt;
		else
			*(nodename->value.addrAsBoolean) = node->value.dataAsBoolean;
	}

	cout << "������������:" << endl;
	root->GetCur()->PrintDetail();
	cout << endl << endl;
}

void Diagram::Switch() //Switch.
{
	int type;
	string lexem = "";
	type = sc->Scan(lexem);
	if (type != Tswitch)
		PrintError("��������� switch.", sc->GetLine() + 1, sc->GetUKfromStartString());
	type = sc->Scan(lexem);
	if (type != TopenBracket)
		PrintError("�������� ������ (.", sc->GetLine() + 1, sc->GetUKfromStartString());
	
	Node* node = Expression();

	type = sc->Scan(lexem);
	if (type != TcloseBracket)
		PrintError("�������� ������ ).", sc->GetLine() + 1, sc->GetUKfromStartString());
	type = sc->Scan(lexem);
	if (type != TopenBrace)
		PrintError("�������� ������ {.", sc->GetLine() + 1, sc->GetUKfromStartString());
	Case();
	type = sc->Scan(lexem);
	if (type != TcloseBrace)
		PrintError("�������� ������ }.", sc->GetLine() + 1, sc->GetUKfromStartString());
}

void Diagram::Case() //Case.
{
	int type;
	string lexem = "";
	int uk = sc->GetUK();
	int line = sc->GetLine();
	type = sc->Scan(lexem);
	//��������� Case.
	while (type == Tcase)
	{
		Constant();
		type = sc->Scan(lexem);
		if (type != Tcolon)
			PrintError("�������� ������ :.", sc->GetLine() + 1, sc->GetUKfromStartString());
		MultipleOperators();
		uk = sc->GetUK();
		line = sc->GetLine();
		type = sc->Scan(lexem);
	}

	//default.
	if (type == Tdefault)
	{
		type = sc->Scan(lexem);
		if (type != Tcolon)
			PrintError("�������� ������ :.", sc->GetLine() + 1, sc->GetUKfromStartString());
		MultipleOperators();
	}
	//��� default.
	else if (type == TcloseBrace)
	{
		sc->PutUK(uk);
		sc->PutLine(line);
	}
	else
		PrintError("��������� case ��� default.", sc->GetLine() + 1, sc->GetUKfromStartString());
}

void Diagram::MultipleOperators() //��������� ����������.
{
	int type;
	string lexem = "";
	int uk = sc->GetUK();
	int line = sc->GetLine();
	type = sc->Scan(lexem);
	sc->PutUK(uk);
	sc->PutLine(line);

	while (type != Tcase && type != Tdefault && type != TcloseBrace && 
		type != Terror && type != Tend)
	{
		Operator();
		uk = sc->GetUK();
		line = sc->GetLine();
		type = sc->Scan(lexem);
		sc->PutUK(uk);
		sc->PutLine(line);
	}
}

Node* Diagram::Name() //���.
{
	int type;
	string lexem = "";
	type = sc->Scan(lexem);
	if (type != Tident)
		PrintError("�������� �������������.", sc->GetLine() + 1, sc->GetUKfromStartString());
	
	string nameident = lexem; //���������� ���.

	int uk = sc->GetUK();
	int line = sc->GetLine();
	type = sc->Scan(lexem);

	int numDim = 0; //������� ���-�� ���������.

	vector<int> dim;

	//������ �������.
	while (type == TopenSquareBracket)
	{
		numDim++;

		Node* node = Expression();

		//��������� ��� ������������ �������� ���� INT.
		if (node->type != DataType::IINT)
			root->PrintError("� ������� ������� ��������� ��������� ����", nameident);

		dim.push_back(node->value.dataAsInt);

		type = sc->Scan(lexem);
		if (type != TcloseSquareBracket)
			PrintError("�������� ������ ].", sc->GetLine() + 1, sc->GetUKfromStartString());
		uk = sc->GetUK();
		line = sc->GetLine();
		type = sc->Scan(lexem);
	}
	sc->PutUK(uk);
	sc->PutLine(line);

	//��������� ������� ���������� ��� ������� � ������.
	Node* node2 = root->SemGetVar(nameident, numDim, dim)->GetNode();

	Node* result = new  Node();

	result->typenode = node2->typenode;
	result->lexem = node2->lexem;
	result->numberDimension = node2->numberDimension;
	result->type = node2->type;
	result->dimension = dim;

	if (numDim > 0)
	{
		int index = 0;

		for (int i = 0; i < dim.size() - 1; i++)
		{
			index += dim[i];
			index *= node2->dimension[i + 1];
		}

		index += dim[dim.size() - 1];

		if (node2->type == DataType::IINT)
		{
			int * arr = node2->value.addrAsInt;

			result->value.addrAsInt = arr + index;
		}
		else
		{
			bool* arr = node2->value.addrAsBoolean;

			result->value.addrAsBoolean = arr + index;
		}
	}
	else
		result->value = node2->value;

	return result;
}

Node* Diagram::Expression() //���������.
{
	int type;
	string lexem = "";

	Node* node = Inequality();

	int uk = sc->GetUK();
	int line = sc->GetLine();
	int operation = type = sc->Scan(lexem);
	while (type == Tuneq || type == Teq)
	{
		Node* node2;

		node2 = Inequality();
		uk = sc->GetUK();
		line = sc->GetLine();

		//�������� �����.
		if (node->type != node2->type)
		{
			root->PrintError("�� ������������� ����", node->lexem);
		}
		else
		//���������� �������� ����������� � ���������.
		{
			if (operation == Tuneq)
			{
				if (node->type == DataType::BBOOLEAN)
					node->value.dataAsBoolean = node->value.dataAsBoolean != node2->value.dataAsBoolean;
				else
				{
					node->value.dataAsBoolean = node->value.dataAsInt != node2->value.dataAsInt;
				}
			}
			else
			{
				if (node->type == DataType::BBOOLEAN)
					node->value.dataAsBoolean = node->value.dataAsBoolean == node2->value.dataAsBoolean;
				else
				{
					node->value.dataAsBoolean = node->value.dataAsInt == node2->value.dataAsInt;
				}
			}
			node->type = DataType::BBOOLEAN;
			node->typenode = TypeNode::CCONSTANT;
		}

		operation = type = sc->Scan(lexem);
	}
	sc->PutUK(uk);
	sc->PutLine(line);

	return node;
}

Node* Diagram::Inequality() //�����������.
{
	int type;
	string lexem = "";

	Node* node = Addendum();

	int uk = sc->GetUK();
	int line = sc->GetLine();
	int operation = type = sc->Scan(lexem);
	while (type == Tless || type == Tmore || type == TmoreOrEq || type == TlessOrEq)
	{
		Node* node2;

		node2 = Addendum();
		uk = sc->GetUK();
		line = sc->GetLine();

		//�������� �����.
		if (node->type != DataType::IINT || node2->type != DataType::IINT)
		{
			if (node->type != DataType::IINT)
				root->PrintError("�� ������������� ���� int", node->lexem);
			else
				root->PrintError("�� ������������� ���� int", node2->lexem);
		}
		else
			//���������� �������� ������, ������, ������ � �����, ������ � �����.
		{
			if (operation == Tless)
			{
				node->value.dataAsBoolean = node->value.dataAsInt < node2->value.dataAsInt;
			}
			else if (operation == Tmore)
			{
				node->value.dataAsBoolean = node->value.dataAsInt > node2->value.dataAsInt;
			}
			else if (operation == TmoreOrEq)
			{
				node->value.dataAsBoolean = node->value.dataAsInt >= node2->value.dataAsInt;
			}
			else
			{
				node->value.dataAsBoolean = node->value.dataAsInt <= node2->value.dataAsInt;
			}
			node->type = DataType::BBOOLEAN;
			node->typenode = TypeNode::CCONSTANT;
		}

		operation = type = sc->Scan(lexem);
	}
	sc->PutUK(uk);
	sc->PutLine(line);

	return node;
}

Node* Diagram::Addendum() //���������.
{
	int type;
	string lexem = "";

	Node* node = Multiplier();

	int uk = sc->GetUK();
	int line = sc->GetLine();
	int operation = type = sc->Scan(lexem);
	while (type == Tminus || type == Tplus)
	{
		Node* node2;

		node2 = Multiplier();
		uk = sc->GetUK();
		line = sc->GetLine();

		//�������� �����.
		if (node->type != DataType::IINT || node2->type != DataType::IINT)
		{
			if (node->type != DataType::IINT)
				root->PrintError("�� ������������� ���� int", node->lexem);
			else
				root->PrintError("�� ������������� ���� int", node2->lexem);
		}
		else
			//���������� �������� ���� � �����.
		{
			if (operation == Tminus)
			{
				node->value.dataAsInt = node->value.dataAsInt - node2->value.dataAsInt;
			}
			else
			{
				node->value.dataAsInt = node->value.dataAsInt + node2->value.dataAsInt;
			}
			node->type = DataType::IINT;
			node->typenode = TypeNode::CCONSTANT;
		}

		operation = type = sc->Scan(lexem);
	}
	sc->PutUK(uk);
	sc->PutLine(line);

	return node;
}

Node* Diagram::Multiplier() //���������.
{
	int type;
	string lexem = "";

	Node* node = Prefix();

	int uk = sc->GetUK();
	int line = sc->GetLine();
	int operation = type = sc->Scan(lexem);
	while (type == Tremain || type == Tdiv || type == Tmul)
	{
		Node* node2;

		node2 = Prefix();
		uk = sc->GetUK();
		line = sc->GetLine();

		//�������� �����.
		if (node->type != DataType::IINT || node2->type != DataType::IINT)
		{
			if (node->type != DataType::IINT)
				root->PrintError("�� ������������� ���� int", node->lexem);
			else
				root->PrintError("�� ������������� ���� int", node2->lexem);
		}
		else
			//���������� �������� ��������, ������ � �������.
		{
			if (operation == Tremain)
			{
				node->value.dataAsInt = node->value.dataAsInt % node2->value.dataAsInt;
			}
			else if (operation == Tdiv)
			{
				node->value.dataAsInt = node->value.dataAsInt / node2->value.dataAsInt;
			}
			else
			{
				node->value.dataAsInt = node->value.dataAsInt * node2->value.dataAsInt;
			}
			node->type = DataType::IINT;
			node->typenode = TypeNode::CCONSTANT;
		}

		operation = type = sc->Scan(lexem);
	}
	sc->PutUK(uk);
	sc->PutLine(line);

	return node;
}

Node* Diagram::Prefix() //�������.
{
	int type;
	string lexem = "";

	int uk = sc->GetUK();
	int line = sc->GetLine();
	int operation = type = sc->Scan(lexem);
	if (type != Tplus && type != Tincrement &&
		type != Tminus && type != Tdecrement)
	{
		sc->PutUK(uk);
		sc->PutLine(line);
	}
	Node* node = Postfix();

	if (type == Tplus || type == Tincrement || type == Tminus || type == Tdecrement)
	{
		if (node->typenode == TypeNode::CCONSTANT)
			PrintError("�������� �������������", uk, line);
		
		if (node->type != DataType::IINT)
		{
			root->PrintError("�� ������������� ���� int", node->lexem);
		}
		else
		{
			if (operation == Tminus)
			{
				node->value.dataAsInt = -node->value.dataAsInt;

			}
			else if (operation == Tincrement || operation == Tdecrement)
			{
				Node* original;

				original = root->SemGetVar(node->lexem, node->numberDimension)->GetNode();

				if (original->numberDimension == 0)
					if (operation == Tincrement)
						original->value.dataAsInt = node->value.dataAsInt + 1;
					else
						original->value.dataAsInt = node->value.dataAsInt - 1;
				else
				{
					int index = 0;

					for (int i = 0; i < node->dimension.size() - 1; i++)
					{
						index += node->dimension[i];
						index *= original->dimension[i + 1];
					}

					index += node->dimension[node->dimension.size() - 1];

					if (original->type == DataType::IINT)
						if (operation == Tincrement)
							*(original->value.addrAsInt + index) = node->value.dataAsInt + 1;
						else
							*(original->value.addrAsInt + index) = node->value.dataAsInt - 1;
				}

				if (operation == Tincrement)
				{
					++node->value.dataAsInt;
				}
				else
				{
					--node->value.dataAsInt;
				}
			}

			node->type = DataType::IINT;
			node->typenode = TypeNode::CCONSTANT;
		}
	}

	return node;
}

Node* Diagram::Postfix() //��������.
{
	int type;
	string lexem = "";

	Node* node;

	node = ElementaryExpression();

	int uk = sc->GetUK();
	int line = sc->GetLine();
	int operation = type = sc->Scan(lexem);
	if (type != Tincrement && type != Tdecrement)
	{
		sc->PutUK(uk);
		sc->PutLine(line);
	}
	else 
	{
		if (node->typenode == TypeNode::CCONSTANT)
			PrintError("�������� �������������", uk, line);

		if (node->type != DataType::IINT)
		{
			root->PrintError("�� ������������� ���� int", node->lexem);
		}
		else
		{
			Node* original;

			original = root->SemGetVar(node->lexem, node->numberDimension)->GetNode();

			if (original->numberDimension == 0)
				if (operation == Tincrement)
					original->value.dataAsInt = node->value.dataAsInt + 1;
				else
					original->value.dataAsInt = node->value.dataAsInt - 1;
			else
			{
				int index = 0;

				for (int i = 0; i < node->dimension.size() - 1; i++)
				{
					index += node->dimension[i];
					index *= original->dimension[i + 1];
				}

				index += node->dimension[node->dimension.size() - 1];

				if (original->type == DataType::IINT)
					if (operation == Tincrement)
						*(original->value.addrAsInt + index) = node->value.dataAsInt + 1;
					else
						*(original->value.addrAsInt + index) = node->value.dataAsInt - 1;
			}

			node->type = DataType::IINT;
			node->typenode = TypeNode::CCONSTANT;
		}
	}

	return node;
}

Node* Diagram::ElementaryExpression() //������������ ���������.
{
	Node* node;

	int type;
	string lexem = "";

	int uk = sc->GetUK();
	int line = sc->GetLine();
	type = sc->Scan(lexem);

	//���������.
	if (type == TopenBracket)
	{
		node = Expression();
		type = sc->Scan(lexem);
		if (type != TcloseBracket)
			PrintError("�������� ������ ).", sc->GetLine() + 1, sc->GetUKfromStartString());
	}
	//���������.
	else if (type >= TconstInt10 && type <= TconstChar)
	{
		node = new Node();

		sc->PutUK(uk);
		sc->PutLine(line);

		int number = Constant();

		node->typenode = TypeNode::CCONSTANT;
		node->lexem = to_string(number);
		node->type = DataType::IINT;
		node->value.dataAsInt = number;
	}
	//���.
	else if (type != Ttrue && type != Tfalse)
	{
		sc->PutUK(uk);
		sc->PutLine(line);
		Node* original;
		node = Name();

		if (node->numberDimension != 0)
			if (node->type == DataType::IINT)
			{
				int* res = node->value.addrAsInt;
				node->value.dataAsInt = *res;
			}
			else
			{
				bool* res = node->value.addrAsBoolean;
				node->value.dataAsBoolean = *res;
			}
	}
	else
	{
		node = new Node();

		node->typenode = TypeNode::CCONSTANT;
		node->lexem = lexem;
		node->type = DataType::BBOOLEAN;
		if (type == Ttrue)
			node->value.dataAsBoolean = true;
		else
			node->value.dataAsBoolean = false;
	}

	return node;
}

int Diagram::Constant() //���������.
{
	int type;
	string lexem = "";

	type = sc->Scan(lexem);

	if (type != TconstInt10 && type != TconstInt8 &&
		type != TconstInt16 && type != TconstInt2 &&
		type != TconstChar)
		PrintError("��������� ���������.", sc->GetLine() + 1, sc->GetUKfromStartString());

	if (type == TconstInt10)
		return stoi(lexem);
	else if (type == TconstInt8)
		return stoi(lexem, nullptr, 8);
	else if (type == TconstInt16)
		return stoi(lexem, nullptr, 16);
	else if (type == TconstInt2)
	{
		lexem.erase(lexem.begin());
		lexem.erase(lexem.begin());
		return stoi(lexem, nullptr, 2);
	}
	else
		return lexem[1];
}

void Diagram::PrintError(string description, int line, int symbol)
{
	cout << description << endl;
	cout << "������: " << line << ", �������: " << symbol << "." << endl;
	throw "Error!";
}
