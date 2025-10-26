#ifndef BIGINT_H
#define BIGINT_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

#define BASE 1000000000   // 10^9
#define BASE_DIGITS 9     

class BigInt {
private:
    vector<int> digits;   // Число в виде кусков по BASE в обратном порядке

    void removeLeadingZeros(); // Убираем ведущие нули (000123 → 123)

public:
    BigInt();                          // Конструктор по умолчанию
    BigInt(const string& number);      // Конструктор из строки

    string toString() const;           // Число в строку

    // Арифметические операции
    BigInt addition(const BigInt& num_b) const;     
    BigInt subtraction(const BigInt& num_b) const; 
    BigInt multiplication(const BigInt& num_b) const;
    BigInt division(const BigInt& num_b) const;

    BigInt mod(const BigInt& num_b) const;         // остаток от деления

    int compare(const BigInt& num_b) const; // -1 если меньше, 0 если равно, 1 если больше

    BigInt NOD(const BigInt& num_b) const;
    BigInt NOK(const BigInt& num_b) const;

    BigInt power(unsigned long long exponent) const;
    BigInt sqrt() const;
};

#endif
