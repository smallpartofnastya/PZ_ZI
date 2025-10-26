/*
g++ generator.cpp -lgmp -lgmpxx -o generator
./generator 1900 5700
*/
#include <gmp.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <random>

using namespace std;

int main(int argc, char* argv[]) {

    if (argc < 3) {
        cerr << "Использование: ./generator MIN_DIGITS MAX_DIGITS" << endl;
        return 1;
    }
    
    const size_t MAX_DIGITS = stoull(argv[2]); // лимит для чисел
    const size_t MIN_DIGITS = stoull(argv[1]);

    mpz_t num1, num2;
    gmp_randstate_t state;

    //mpz_t — это тип GMP для больших чисел
    mpz_init(num1);
    mpz_init(num2);
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(nullptr));

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<size_t> dist(MIN_DIGITS, MAX_DIGITS);
    size_t digits1 = dist(gen);
    size_t digits2 = dist(gen);

    size_t bits1 = digits1 * 3.322;
    size_t bits2 = digits2 * 3.322;


    mpz_urandomb(num1, state, bits1);
    mpz_urandomb(num2, state, bits2);

     // Записываем в файлы
    ofstream f1("num1.txt");
    ofstream f2("num2.txt");

    if (!f1 || !f2) {
        cerr << "Ошибка открытия файлов!\n";
        return 1;
    }

    string s1 = mpz_get_str(nullptr, 10, num1);
    string s2 = mpz_get_str(nullptr, 10, num2);

    f1 << s1;
    f2 << s2;

    f1.close();
    f2.close();

    cout << "Первое число: " << s1.size() << " цифр\n";
    cout << "Второе число: " << s2.size() << " цифр\n";

    mpz_clear(num1);
    mpz_clear(num2);
    gmp_randclear(state);

    return 0;
}
