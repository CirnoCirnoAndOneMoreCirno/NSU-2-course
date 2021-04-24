#include <iostream>
#include "CSVParser.h"
#include <tuple>

int main()
{
    /*
    tuple<int, string, int> t(1, "test", 2);
    cout << const_cast<const tuple<int, string, int>&>(t);
    */
    string name1 = "C:\\Users\\Andrew\\source\\repos\\CSVParser\\Debug\\test_1.txt";
    CSVParser<int, bool> p1(name1);
    p1.printData();
    cout << endl;
    string name2 = "C:\\Users\\Andrew\\source\\repos\\CSVParser\\Debug\\test_2.txt";
    CSVParser<int, string, bool> p2(name2);
    p2.printData();
    cout << endl;

    string name3  = "C:\\Users\\Andrew\\source\\repos\\CSVParser\\Debug\\test_3.txt";
    CSVParser<char, int> p3(name3, ',');
    p3.printData();
    cout << endl;

    string name4 = "C:\\Users\\Andrew\\source\\repos\\CSVParser\\Debug\\test_4.txt";
    CSVParser<bool, bool, string, int, int> p4(name4, ',', ';');
    p4.printData();
    cout << endl;

    cout << "Iterator test" << endl;
    CSVParser<bool, bool, string, int, int>::Iterator it = p4.begin();
    for (it = p4.begin(); it != p4.end(); it++)
    {
        cout << it.at() << endl;
    }
    cout << endl;

    string name5 = "C:\\Users\\Andrew\\source\\repos\\CSVParser\\Debug\\test_5.txt";
    CSVParser<bool, bool, string, int, int> p5(name5);
    p5.printData();
    cout << endl;

    string name7 = "C:\\Users\\Andrew\\source\\repos\\CSVParser\\Debug\\test_7.txt";
    CSVParser<int, double> p7(name7);
    p7.printData();
    cout << endl;

    string name6 = "C:\\Users\\Andrew\\source\\repos\\CSVParser\\Debug\\test_6.txt";
    CSVParser<bool, bool, string, int, int> p6(name6);
    p6.printData();
    cout << endl;

}

