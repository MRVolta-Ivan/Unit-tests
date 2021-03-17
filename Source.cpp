#include "Diagram.h"
#include <Windows.h>
#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


TEST(Scaner, TestScanIdent)
{
	Scaner sc;
	sc.SetText("number");

	string lex;

	ASSERT_EQ(sc.Scan(lex), Tident);
}

TEST(Scaner, TestScanNumber10)
{
	Scaner sc;
	sc.SetText("10");

	string lex;

	ASSERT_EQ(sc.Scan(lex), TconstInt10);
}

TEST(Scaner, TestScanInt)
{
	Scaner sc;
	sc.SetText("int");

	string lex;

	ASSERT_EQ(sc.Scan(lex), Tint);
}

TEST(Diagram, TestCorrectOneDescription)
{
	string text = "int number = 10;";

	Tree* t = new Tree();
	Diagram d(new Scaner(), t);

	d.SetText(text);

	t->SetCur(t);

	ASSERT_NO_THROW(d.OneDescription(false));
}

TEST(Diagram, TestCorrectAssignment)
{
	string text = "int a;";

	Tree* t = new Tree();
	Diagram d(new Scaner(), t);

	d.SetText(text);

	t->SetCur(t);

	d.OneDescription(false);

	text = "a = 10;";

	d.SetText(text);

	ASSERT_NO_THROW(d.Operator());
}

TEST(Diagram, TestCorrectSwitch)
{
	string text = "switch(10){case 10: break; default: break;}";

	Tree* t = new Tree();
	Diagram d(new Scaner(), t);

	d.SetText(text);

	t->SetCur(t);

	ASSERT_NO_THROW(d.Operator());
}

TEST(Diagram, TestCorrectCompoundOperator)
{
	string text = "int a;";

	Tree* t = new Tree();
	Diagram d(new Scaner(), t);

	d.SetText(text);

	t->SetCur(t);

	d.OneDescription(false);

	text = "{a = 10; {int b; a = 100; {a = 0x1000; b = 0b10; int c = 07654321;}}}";

	d.SetText(text);

	ASSERT_NO_THROW(d.Operator());
}

