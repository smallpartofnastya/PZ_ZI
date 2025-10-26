#ifndef SIMPLE_H
#define SIMPLE_H

#include <vector>
#include <string>
#include "/home/anastasia/Univer/Защита_информации/Задание1/bigint.h"
using namespace std;

class BigInt;

// Простые проверки
bool DivBy2(const BigInt& n);
bool DivBy5(const BigInt& n);
bool DivBy3_6_9(const BigInt& n);

// Проверка простоты делением
bool PrimeTrialDiv(const BigInt& n);

// Решета
vector<int> Eratosthenes(const BigInt& n);
vector<int> Atkin(const BigInt& n);

// Тест Люка–Лемера для числа Мерсенна
bool lucasLehmerTest(int p);

#endif