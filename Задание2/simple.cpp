#include "simple.h"
#include <cmath>
#include <fstream>

// --- Проверка делимости на 2 ---
bool DivBy2(const BigInt& n) {
    string s = n.toString();
    return ((s.back() - '0') % 2 == 0);
}

// --- Проверка делимости на 5 ---
bool DivBy5(const BigInt& n) {
    char last = n.toString().back();
    return (last == '0' || last == '5');
}

// --- Проверка делимости на 3, 6, 9 ---
bool DivBy3_6_9(const BigInt& n) {
    string s = n.toString();
    int sum = 0;
    for (char c : s) sum += (c - '0');
    return (sum % 3 == 0 || sum % 6 == 0 || sum % 9 == 0);
}

// --- Проверка простоты делением до sqrt(n) ---
bool PrimeTrialDiv(const BigInt& n) {
    if (n.toString() == "0" || n.toString() == "1") return false;
    if (DivBy2(n) && n.toString() != "2") return false;

    BigInt i("3");
    BigInt zero("0");
    BigInt sqrtn = n.sqrt();

    while (i.compare(sqrtn) <= 0) {
        if (n.mod(i).compare(zero) == 0) return false;
        i = i.addition(BigInt("2"));
    }
    return true;
}

// --- Решето Эратосфена ---
vector<int> Eratosthenes(int limit) {
    vector<bool> prime(limit + 1, true);
    prime[0] = prime[1] = false;
    for (int i = 2; i * i <= limit; i++)
        if (prime[i])
            for (int j = i * i; j <= limit; j += i)
                prime[j] = false;

    vector<int> res;
    for (int i = 2; i <= limit; i++)
        if (prime[i]) res.push_back(i);
    return res;
}

// --- Решето Аткина ---
vector<int> Atkin(int limit) {
    vector<bool> sieve(limit + 1, false);
    int sqrtLimit = sqrt(limit);

    for (int x = 1; x <= sqrtLimit; x++)
        for (int y = 1; y <= sqrtLimit; y++) {
            int n = 4*x*x + y*y;
            if (n <= limit && (n % 12 == 1 || n % 12 == 5)) sieve[n] = !sieve[n];
            n = 3*x*x + y*y;
            if (n <= limit && n % 12 == 7) sieve[n] = !sieve[n];
            n = 3*x*x - y*y;
            if (x > y && n <= limit && n % 12 == 11) sieve[n] = !sieve[n];
        }

    for (int r = 5; r <= sqrtLimit; r++)
        if (sieve[r])
            for (int i = r*r; i <= limit; i += r*r)
                sieve[i] = false;

    vector<int> res = {2, 3};
    for (int i = 5; i <= limit; i++)
        if (sieve[i]) res.push_back(i);
    return res;
}

// --- Тест Люка–Лемера для числа Мерсенна ---
bool lucasLehmerTest(const BigInt& M_p, int p) {
    if (p == 2) return true;
    BigInt s("4");
    for (int i = 0; i < p - 2; i++) {
        s = s.multiplication(s).subtraction(BigInt("2"));
        s = s.mod(M_p);
    }
    return (s.toString() == "0");
}