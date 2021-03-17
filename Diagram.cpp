#include "Diagram.h"

Diagram::Diagram(Scaner *s, Tree* _tree)
{
	sc = s;
	root = _tree;
}

Diagram::~Diagram()
{
}

void Diagram::Program() //Программа.
{
	int type;
	string lexem = "";
	type = sc->Scan(lexem);
	if (type != Tpublic)
		PrintError("Ожидалось public.", sc->GetLine() + 1, sc->GetUKfromStartString());
	type = sc->Scan(lexem);
	if (type != Tclass)
		PrintError("Ожидалось class.", sc->GetLine() + 1, sc->GetUKfromStartString());
	type = sc->Scan(lexem);
	if (type != TMain)
		PrintError("Ожидалось Main.", sc->GetLine() + 1, sc->GetUKfromStartString());
	type = sc->Scan(lexem);
	if (type != TopenBrace)
		PrintError("Ожидался символ {.", sc->GetLine() + 1, sc->GetUKfromStartString());
	int uk = sc->GetUK();
	int line = sc->GetLine();
	type = sc->Scan(lexem);
	sc->PutLine(line);
	sc->PutUK(uk);
	//Одно описание. Мы попадаем на одно описание, если следующее int, short, long, boolean.
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
		PrintError("Ожидался символ }.", sc->GetLine() + 1, sc->GetUKfromStartString());
}

void Diagram::Function() //Функция.
{
	int type;
	string lexem = "";
	type = sc->Scan(lexem);
	if (type != Tpublic)
		PrintError("Ожидалось public.", sc->GetLine() + 1, sc->GetUKfromStartString());
	type = sc->Scan(lexem);
	if (type != Tstatic)
		PrintError("Ожидалось static.", sc->GetLine() + 1, sc->GetUKfromStartString());
	type = sc->Scan(lexem);
	if (type != Tvoid)
		PrintError("Ожидалось void.", sc->GetLine() + 1, sc->GetUKfromStartString());
	type = sc->Scan(lexem);
	if (type != Tmain)
		PrintError("Ожидалось main.", sc->GetLine() + 1, sc->GetUKfromStartString());
	type = sc->Scan(lexem);
	if (type != TopenBracket)
		PrintError("Ожидался символ (.", sc->GetLine() + 1, sc->GetUKfromStartString());
	type = sc->Scan(lexem);
	if (type != TcloseBracket)
		PrintError("Ожидался символ ).", sc->GetLine() + 1, sc->GetUKfromStartString());
	CompoundOperator();
}

void Diagram::CompoundOperator() //Составной оператор.
{
	int type;
	string lexem = "";
	type = sc->Scan(lexem);
	if (type != TopenBrace)
		PrintError("Ожидался символ {.", sc->GetLine() + 1, sc->GetUKfromStartString());
	
	Tree* save = root->GetCur();

	root->SemInclude(); //Создаём пустую вершину справа от текущего узла. 
	
	int uk = sc->GetUK();
	int line = sc->GetLine();
	type = sc->Scan(lexem);
	sc->PutUK(uk);
	sc->PutLine(line);
	//Одно описание или Оператор.

	cout << "Данные перед входом в блок:" << endl;
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
		PrintError("Ожидался символ }.", sc->GetLine() + 1, sc->GetUKfromStartString());

	cout << "Данные после выхода из блока:" << endl;
	root->GetCur()->PrintDetail();
	cout << endl << endl;

	root->SetCur(save);

}

void Diagram::OneDescription(bool field) //Одно описание.
{
	int type;
	string lexem = "";

	type = TypeData();

	int typeVar = type; //Запоминаем тип переменных.

	do
	{
		Variable(typeVar, field);
		type = sc->Scan(lexem);
	} while (type == Tcomma);
	if (type != Tsemi)
		PrintError("Ожидался символ ;.", sc->GetLine() + 1, sc->GetUKfromStartString());
}

int Diagram::TypeData() //Тип данных.
{
	int type;
	string lexem = "";
	type = sc->Scan(lexem);
	if (!(type >= Tint && type <= Tboolean))
		PrintError("Ожидался тип данных: int, short, long, boolean.", sc->GetLine() + 1, sc->GetUKfromStartString());
	return type;
}

void Diagram::Variable(int typeData, bool field) //Переменная.
{
	int type;
	string lexem = "";
	type = sc->Scan(lexem);
	if (type != Tident)
		PrintError("Ожидался идентификатор.", sc->GetLine() + 1, sc->GetUKfromStartString());

	string nameIdent = lexem; //Запоминаем имя переменной.

	int uk = sc->GetUK();
	int line = sc->GetLine();
	type = sc->Scan(lexem);

	DataValue value;
	if (typeData == Tint)
		value.dataAsInt = 0;
	else
		value.dataAsBoolean = false;

	//Заносим переменную в дерево.
	root->SemInclude(nameIdent, typeData, value, field, false, 0, {});

	//Инициализация переменной.
	if (type == Teval)
	{
		Node* node = Expression();

		root->SetInit(node); //устанавливаем признак инициализации.
		
		cout << "Инициализация:" << endl;
		root->GetCur()->PrintDetail();
		cout << endl << endl;
	}
	//Создание массива.
	else if (type == TopenSquareBracket)
	{
		int countDim = 1; //Начинаем считать количество измерений у массива.

		type = sc->Scan(lexem);
		if (type != TcloseSquareBracket)
			PrintError("Ожидался символ ].", sc->GetLine() + 1, sc->GetUKfromStartString());
		type = sc->Scan(lexem);
		while (type == TopenSquareBracket)
		{
			countDim++;

			type = sc->Scan(lexem);
			if (type != TcloseSquareBracket)
				PrintError("Ожидался символ ].", sc->GetLine() + 1, sc->GetUKfromStartString());
			type = sc->Scan(lexem);
		}

		root->SetNumDimension(countDim); //Устанавливаем кол-во измерений.

		if (type != Teval)
			PrintError("Ожидался символ =.", sc->GetLine() + 1, sc->GetUKfromStartString());
		type = sc->Scan(lexem);
		if (type != Tnew)
			PrintError("Ожидалось new.", sc->GetLine() + 1, sc->GetUKfromStartString());
		type = TypeData();

		Node* node = new Node();
		if (type == Tint)
			node->type = DataType::IINT;
		else
			node->type = DataType::BBOOLEAN;

		vector<int> dim = ArraySize();

		//Устанавливаем начало массива.
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

		root->SetDimension(dim); //Устанавливаем сами измерения.
		root->SetInit(node);

		cout << "Инициализация массива:" << endl;
		root->GetCur()->PrintDetail();
		cout << endl << endl;
	}
	else
	{
		sc->PutUK(uk);
		sc->PutLine(line);
	}
}

vector<int> Diagram::ArraySize() //Размер массива.
{
	int type;
	string lexem = "";
	int uk, line;

	vector<int> dim;

	do
	{
		type = sc->Scan(lexem);
		if (type != TopenSquareBracket)
			PrintError("Ожидался символ [.", sc->GetLine() + 1, sc->GetUKfromStartString());
		
		dim.push_back(Constant());
		
		
		type = sc->Scan(lexem);
		if (type != TcloseSquareBracket)
			PrintError("Ожидался символ ].", sc->GetLine() + 1, sc->GetUKfromStartString());
		uk = sc->GetUK();
		line = sc->GetLine();
		type = sc->Scan(lexem);
		sc->PutUK(uk);
		sc->PutLine(line);
	}
	while (type == TopenSquareBracket);

	return dim; //
}

void Diagram::Operator() //Оператор.
{
	int type;
	string lexem = "";
	int uk = sc->GetUK();
	int line = sc->GetLine();
	type = sc->Scan(lexem);
	//Присваивание.
	if (type == Tident)
	{
		sc->PutUK(uk);
		sc->PutLine(line);
		Assignment();
		type = sc->Scan(lexem);
		if (type != Tsemi)
			PrintError("Ожидался символ ;.", sc->GetLine() + 1, sc->GetUKfromStartString());
	}
	//Оператор break.
	else if (type == Tbreak)
	{
		type = sc->Scan(lexem);
		if (type != Tsemi)
			PrintError("Ожидался символ ;.", sc->GetLine() + 1, sc->GetUKfromStartString());
	}
	//Switch.
	else if (type == Tswitch)
	{
		sc->PutUK(uk);
		sc->PutLine(line);
		Switch();
	}
	//Составной оператор.
	else if (type == TopenBrace)
	{
		sc->PutUK(uk);
		sc->PutLine(line);
		CompoundOperator();
	}
	//Проверяем пустой оператор.
	else if (type != Tsemi)
		PrintError("Ожидался символ ;.", sc->GetLine() + 1, sc->GetUKfromStartString());

}

void Diagram::Assignment() //Присваивание.
{
	int type;
	string lexem = "";

	Node* nodename = Name();
	type = sc->Scan(lexem);
	if (type != Teval)
		PrintError("Ожидался символ =.", sc->GetLine() + 1, sc->GetUKfromStartString());
	
	Node* original;

	original = root->SemGetVar(nodename->lexem, nodename->numberDimension)->GetNode();

	Node* node = Expression();

	if (original->type != node->type)
		root->PrintError("Не соответствие типов",nodename->lexem);

	if (original->numberDimension == 0)
		original->value = node->value;
	else
	{
		if (original->type == DataType::IINT)
			*(nodename->value.addrAsInt) = node->value.dataAsInt;
		else
			*(nodename->value.addrAsBoolean) = node->value.dataAsBoolean;
	}

	cout << "Присваивание:" << endl;
	root->GetCur()->PrintDetail();
	cout << endl << endl;
}

void Diagram::Switch() //Switch.
{
	int type;
	string lexem = "";
	type = sc->Scan(lexem);
	if (type != Tswitch)
		PrintError("Ожидалось switch.", sc->GetLine() + 1, sc->GetUKfromStartString());
	type = sc->Scan(lexem);
	if (type != TopenBracket)
		PrintError("Ожидался символ (.", sc->GetLine() + 1, sc->GetUKfromStartString());
	
	Node* node = Expression();

	type = sc->Scan(lexem);
	if (type != TcloseBracket)
		PrintError("Ожидался символ ).", sc->GetLine() + 1, sc->GetUKfromStartString());
	type = sc->Scan(lexem);
	if (type != TopenBrace)
		PrintError("Ожидался символ {.", sc->GetLine() + 1, sc->GetUKfromStartString());
	Case();
	type = sc->Scan(lexem);
	if (type != TcloseBrace)
		PrintError("Ожидался символ }.", sc->GetLine() + 1, sc->GetUKfromStartString());
}

void Diagram::Case() //Case.
{
	int type;
	string lexem = "";
	int uk = sc->GetUK();
	int line = sc->GetLine();
	type = sc->Scan(lexem);
	//Несколько Case.
	while (type == Tcase)
	{
		Constant();
		type = sc->Scan(lexem);
		if (type != Tcolon)
			PrintError("Ожидался символ :.", sc->GetLine() + 1, sc->GetUKfromStartString());
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
			PrintError("Ожидался символ :.", sc->GetLine() + 1, sc->GetUKfromStartString());
		MultipleOperators();
	}
	//Без default.
	else if (type == TcloseBrace)
	{
		sc->PutUK(uk);
		sc->PutLine(line);
	}
	else
		PrintError("Ожидалось case или default.", sc->GetLine() + 1, sc->GetUKfromStartString());
}

void Diagram::MultipleOperators() //Несколько операторов.
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

Node* Diagram::Name() //Имя.
{
	int type;
	string lexem = "";
	type = sc->Scan(lexem);
	if (type != Tident)
		PrintError("Ожидался идентификатор.", sc->GetLine() + 1, sc->GetUKfromStartString());
	
	string nameident = lexem; //Запоминаем имя.

	int uk = sc->GetUK();
	int line = sc->GetLine();
	type = sc->Scan(lexem);

	int numDim = 0; //Считаем кол-во измерений.

	vector<int> dim;

	//Скобки массива.
	while (type == TopenSquareBracket)
	{
		numDim++;

		Node* node = Expression();

		//Проверяем что возвращаемое значение типа INT.
		if (node->type != DataType::IINT)
			root->PrintError("В скобках массива выражение неверного типа", nameident);

		dim.push_back(node->value.dataAsInt);

		type = sc->Scan(lexem);
		if (type != TcloseSquareBracket)
			PrintError("Ожидался символ ].", sc->GetLine() + 1, sc->GetUKfromStartString());
		uk = sc->GetUK();
		line = sc->GetLine();
		type = sc->Scan(lexem);
	}
	sc->PutUK(uk);
	sc->PutLine(line);

	//Проверяем наличие переменной или массива в дереве.
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

Node* Diagram::Expression() //Выражение.
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

		//Проверка типов.
		if (node->type != node2->type)
		{
			root->PrintError("Не соответствует типы", node->lexem);
		}
		else
		//Выполнение операций неравенства и равенства.
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

Node* Diagram::Inequality() //Неравенство.
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

		//Проверка типов.
		if (node->type != DataType::IINT || node2->type != DataType::IINT)
		{
			if (node->type != DataType::IINT)
				root->PrintError("Не соответствует типу int", node->lexem);
			else
				root->PrintError("Не соответствует типу int", node2->lexem);
		}
		else
			//Выполнение операций меньше, больше, больше и равно, меньше и равно.
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

Node* Diagram::Addendum() //Слагаемое.
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

		//Проверка типов.
		if (node->type != DataType::IINT || node2->type != DataType::IINT)
		{
			if (node->type != DataType::IINT)
				root->PrintError("Не соответствует типу int", node->lexem);
			else
				root->PrintError("Не соответствует типу int", node2->lexem);
		}
		else
			//Выполнение операций плюс и минус.
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

Node* Diagram::Multiplier() //Множитель.
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

		//Проверка типов.
		if (node->type != DataType::IINT || node2->type != DataType::IINT)
		{
			if (node->type != DataType::IINT)
				root->PrintError("Не соответствует типу int", node->lexem);
			else
				root->PrintError("Не соответствует типу int", node2->lexem);
		}
		else
			//Выполнение операций умножить, делить и остаток.
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

Node* Diagram::Prefix() //Префикс.
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
			PrintError("Ожидался идентификатор", uk, line);
		
		if (node->type != DataType::IINT)
		{
			root->PrintError("Не соответствует типу int", node->lexem);
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

Node* Diagram::Postfix() //Постфикс.
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
			PrintError("Ожидался идентификатор", uk, line);

		if (node->type != DataType::IINT)
		{
			root->PrintError("Не соответствует типу int", node->lexem);
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

Node* Diagram::ElementaryExpression() //Элементарное выражение.
{
	Node* node;

	int type;
	string lexem = "";

	int uk = sc->GetUK();
	int line = sc->GetLine();
	type = sc->Scan(lexem);

	//Выражение.
	if (type == TopenBracket)
	{
		node = Expression();
		type = sc->Scan(lexem);
		if (type != TcloseBracket)
			PrintError("Ожидался символ ).", sc->GetLine() + 1, sc->GetUKfromStartString());
	}
	//Константа.
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
	//Имя.
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

int Diagram::Constant() //Константа.
{
	int type;
	string lexem = "";

	type = sc->Scan(lexem);

	if (type != TconstInt10 && type != TconstInt8 &&
		type != TconstInt16 && type != TconstInt2 &&
		type != TconstChar)
		PrintError("Ожидалось константа.", sc->GetLine() + 1, sc->GetUKfromStartString());

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
	cout << "Строка: " << line << ", позиция: " << symbol << "." << endl;
	throw "Error!";
}
