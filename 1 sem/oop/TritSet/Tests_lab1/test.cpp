#include "pch.h"
#include "../TritSet/TritSet.h"
TEST(TestCaseName, TestName) {
	
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}

TEST(exampleTest, test1)
{
	//резерв памяти для хранения 1000 тритов
	TritSet set(1000);
	// length of internal array
	size_t allocLength = set.capacity();
	EXPECT_TRUE(allocLength >= 1000 * 2 / 8 / sizeof(uint));
	// 1000*2 - min bits count
	// 1000*2 / 8 - min bytes count
	// 1000*2 / 8 / sizeof(uint) - min uint[] size

	//не выделяет никакой памяти
	set[1000'000'000] = Unknown;
	EXPECT_TRUE(allocLength == set.capacity());

	// false, but no exception or memory allocation
	if (set[2'000'000'000] == True) {}
	EXPECT_TRUE(allocLength == set.capacity());

	//выделение памяти
	set[1000'000'000] = True;
	EXPECT_TRUE(allocLength < set.capacity());

	//no memory operations
	allocLength = set.capacity();
	set[1000'000'000] = Unknown;
	set[1000'000] = False;
	EXPECT_TRUE(allocLength == set.capacity());

	//освобождение памяти до начального значения или 
	//до значения необходимого для хранения последнего установленного трита
	//в данном случае для трита 1000’000
	set.shrink();
	EXPECT_TRUE(allocLength > set.capacity());

}

TEST(exampleTest, test2)
{
	TritSet setA(1000);
	TritSet setB(2000);
	TritSet setC = setA & setB;
	assert(setC.capacity() == setB.capacity());
}

TEST(TritOperators, SimpleOperators)
{
    Trit u = Unknown, f = False, t = True;

    EXPECT_EQ(t & t, True);
    EXPECT_EQ(f & t, False);
    EXPECT_EQ(t & u, Unknown);
    EXPECT_EQ(f & u, False);



    EXPECT_EQ(t | t, True);
    EXPECT_EQ(f | t, True);
    EXPECT_EQ(t | u, True);
    EXPECT_EQ(f | u, Unknown);


    EXPECT_EQ(!False, True);
    EXPECT_EQ(!True, False);
    EXPECT_EQ(!Unknown, Unknown);
}

TEST(TritOperators, TritsFromTritSet)
{
    TritSet a;
    TritSet b;
    a[1] = True;
    b[2] = False;

    EXPECT_TRUE((a[1]&b[2]) == False);
    EXPECT_TRUE((a[1] & b[3]) == Unknown);
    EXPECT_TRUE((b[2] & a[0]) == False);
}


TEST(TritOperators, TritSetOperator1)
{
	TritSet a;
	TritSet b;
	a[1] = True;
	a[2] = False;
	b[2] = False;

	TritSet c = a | b;

	EXPECT_TRUE((c[1]) == True);
	EXPECT_TRUE(c[2] == False);
	EXPECT_TRUE(c[3]== Unknown);

	EXPECT_EQ(a.length(), c.length());	
	EXPECT_EQ(b.length(), c.length());

}

TEST(TritOperators, TritSetOperator2)
{
	TritSet a;
	TritSet b;
	
	a[0] = True;
	b[0] = True;
	a[1] = True;
	a[2] = False;
	b[2] = False;

	TritSet c = a & b;

	EXPECT_TRUE(c[0] == True);
	EXPECT_TRUE((c[1]) == Unknown);
	EXPECT_TRUE(c[2] == False);
	EXPECT_TRUE(c[3] == Unknown);

	EXPECT_EQ(a.length(), c.length());
	EXPECT_EQ(b.length(), c.length());

}

TEST(TritOperators, TritSetOperator3)
{
	TritSet a;

	a[0] = True;
	a[1] = Unknown;
	a[2] = False;

	TritSet c = !a;

	EXPECT_TRUE(c[0] == False);
	EXPECT_TRUE((c[1]) == Unknown);
	EXPECT_TRUE(c[2] == True);
	EXPECT_TRUE(c[3] == Unknown);

	EXPECT_EQ(a.length(), c.length());

}
TEST(cardinality, test1)
{
	TritSet a(100);
	TritSet b(200);
	for (int i = 0; i < 33; i++)
	{
		a[i] = True;
		b[i] = False;
	}
	for (int i = 33; i < 66; i++)
	{
		a[i] = True;
	}
	for (int i = 66; i < 100; i++)
	{
		b[i] = False;
	}
	TritSet c = a & b;

	EXPECT_EQ(c.capacity(), b.capacity());

	EXPECT_EQ(c.cardinality(False), 67);
	EXPECT_EQ(c.cardinality(True), 0);
	EXPECT_EQ(c.cardinality(Unknown), 133);

	EXPECT_EQ(b.cardinality(False), 67);
	EXPECT_EQ(b.cardinality(True), 0);
	EXPECT_EQ(b.cardinality(Unknown), 133);

	EXPECT_EQ(a.cardinality(False), 0);
	EXPECT_EQ(a.cardinality(True), 66);
	EXPECT_EQ(a.cardinality(Unknown), 34);
}

TEST(cardinality, test2)
{
	TritSet a(100);
	TritSet b(200);
	for (int i = 0; i < 33; i++)
	{
		a[i] = True;
		b[i] = False;
	}
	for (int i = 33; i < 66; i++)
	{
		a[i] = True;
	}
	for (int i = 66; i < 100; i++)
	{
		b[i] = False;
	}
	TritSet c = a & b;

	std::unordered_map<Trit, int, std::hash<int>> mp = c.cardinality();

	EXPECT_EQ(mp[False], 67);
	EXPECT_EQ(mp[True], 0);
	EXPECT_EQ(mp[Unknown], 133);
}

TEST(TrimTest, test1)
{
	TritSet a(100);
	for (int i = 0; i < 33; i++)
	{
		a[i] = True;
	}
	for (int i = 33; i < 66; i++)
	{
		a[i] = False;
	}
	a[0] = False;
	a[1] = False;
	a.trim(10);

	EXPECT_EQ(a.cardinality(False), 2);
	EXPECT_EQ(a.cardinality(True), 8);
	EXPECT_EQ(a.cardinality(Unknown), 90);

	EXPECT_EQ(a.length(), 10);
}

TEST(TrimTest, test2)
{
	TritSet a(100);
	for (int i = 0; i < 33; i++)
	{
		a[i] = True;
	}
	for (int i = 33; i < 66; i++)
	{
		a[i] = False;
	}
	a[0] = False;
	a[1] = False;

	a.trim(1000);

	EXPECT_EQ(a.cardinality(False), 35);
	EXPECT_EQ(a.cardinality(True), 31);
	EXPECT_EQ(a.cardinality(Unknown), 34);

	EXPECT_EQ(a.length(), 66);
}

TEST(LengthTests, test1)
{
	TritSet set(1000);
	set[1] = False;
	set[2] = False;
	set[3] = True;
	EXPECT_EQ(set.length(), 4);
}

TEST(LengthTests, test2)
{
	TritSet set(1000);
	set[2] = False;
	set[999] = True;
	EXPECT_EQ(set.length(), 1000);
	set[999] = Unknown;
	EXPECT_EQ(set.length(), 3);
}

TEST(LengthTests, test3)
{
	TritSet set;
	EXPECT_EQ(0, set.length());
	EXPECT_EQ(0, set.capacity());
}


TEST(AssigmentOperator, test1)
{
	TritSet a(100);
	for (int i = 0; i < 100; i++)
	{
		a[i] = (Trit)(i%3);
	}

	TritSet b;
	b = a;
	
	for (int i = 0; i < 100; i++)
	{
		EXPECT_TRUE(a[i] == b[i]);
	}
}

TEST(AssigmentOperator, test2)
{
	TritSet a(100);
	TritSet b;
	for (int i = 0; i < 100; i++)
	{
		a[i] = (Trit)(i % 3);
		b[i] = a[i];
	}


	for (int i = 0; i < 100; i++)
	{
		EXPECT_TRUE(a[i] == b[i]);
	}

}

TEST(AssigmentOperator, test4)
{
	TritSet a(100);
	a[0] = True;
	a[1] = a[0];
	EXPECT_TRUE(a[0] == a[1]);
}
TEST(AssigmentOperator, test3)
{
	TritSet a(100);
	for (int i = 0; i < 100; i++)
	{
		a[i] = (Trit)(i % 3);
	}
	TritSet b(a);
	for (int i = 0; i < 100; i++)
	{
		EXPECT_TRUE(a[i] == b[i]);
	}

}

TEST(ShrinkTest, test1)
{
	TritSet a(100);
	size_t cap = a.capacity();
	a[96] = True;
	a.shrink();
	EXPECT_EQ(cap, a.capacity());
}

TEST(ShrinkTest, test2)
{
	TritSet a(100);
	size_t cap = a.capacity();
	a[95] = True;
	a.shrink();
	EXPECT_EQ(cap-16, a.capacity());
}

TEST(ShrinkTest, test3)
{
	TritSet a(100);
	a.shrink();
	EXPECT_EQ(0, a.capacity());
}

TEST(MemoryRealloc, test1)
{
	TritSet a;
	for (int i = 0; i < 1000; i++)
	{
		a[i] = True;
		EXPECT_EQ(16*((i / 16) + 1), a.capacity());
	}
}

TEST(MemoryRealloc, test2)
{
	TritSet a(100);
	int cap = a.capacity();

	a[1] = True;
	a[100] = Unknown;
	a[100000000] = Unknown;
	
	EXPECT_EQ(cap, a.capacity());
}

TEST(UnknownTrits, test1)
{
	TritSet a(10);

	for (int i = 10; i <= 100; i++)
		EXPECT_TRUE(a[i] == Unknown);
}

TEST(TrimAndShrink, test1)
{
	TritSet a(1000);
	a[999] = True;
	a[500] = False;
	a[0] = True;

	a.trim(0);
	a.shrink();

	EXPECT_EQ(0, a.length());
	EXPECT_EQ(0, a.capacity());
	EXPECT_EQ((Trit)a[999], Unknown);
	EXPECT_EQ((Trit)a[0], Unknown);
	EXPECT_EQ((Trit)a[500], Unknown);
}
