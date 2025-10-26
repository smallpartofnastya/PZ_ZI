/*
g++ main.cpp bigint.cpp -o main
./main 10
(степень возведения)
*/
#include "bigint.h"
#include <fstream>
#include <iostream>
using namespace std;

string readFromFile(const string& filename) {
    ifstream fin(filename);
    string number;
    fin >> number;
    return number;
}

void writeToFile(const string& filename, const string& result) {
    ofstream fout(filename);
    fout << result;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: ./main <power>" << endl;
        return 1;
    }
    unsigned long long exponent = stoull(argv[1]);

    BigInt a(readFromFile("num1.txt"));
    BigInt b(readFromFile("num2.txt"));

    writeToFile("sum.txt", a.addition(b).toString());
    cout << "sum completed" << endl;
    writeToFile("sub.txt", a.subtraction(b).toString());
    cout << "sub completed" << endl;
    writeToFile("mul.txt", a.multiplication(b).toString());
    cout << "mul completed" << endl;
    writeToFile("div.txt", a.division(b).toString());
    cout << "div completed" << endl;

    int cmp = a.compare(b);
    writeToFile("cmp.txt", cmp == 0 ? "a = b" : (cmp < 0 ? "a < b" : "a > b"));
    cout << "compare completed" << endl;

    writeToFile("NOD.txt", a.NOD(b).toString());
    cout << "NOD completed" << endl;
    writeToFile("NOK.txt", a.NOK(b).toString());
    cout << "NOK completed" << endl;

    writeToFile("pow1.txt", a.power(exponent).toString());
    cout << "a^exp completed" << endl;
    writeToFile("pow2.txt", b.power(exponent).toString());
    cout << "b^exp completed" << endl;

    cout << "Complete" << endl;

    return 0;
}
