/*
 * Компиляция:
 * g++ simp_main.cpp simple.cpp ../Задание1/bigint.cpp -o simp_main
 *
 * Примеры запуска:
 *   ./simp_main --mersenne 13 1000
 *   ./simp_main --file num1.txt 1000
 *   ./simp_main --manual 123456789 1000


 ./simp_main --mersenne 136279841 1000
 */

#include <iostream>
#include <fstream>
#include <chrono>
#include "simple.h"
using namespace std;

// --- Функция записи строки ---
void writeLine(ofstream& out, const string& key, const string& value) {
    out << key << ": " << value << "\n";
}

// --- Основная программа ---
int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage:\n"
             << "  ./simp_main --mersenne <p> <limit>\n"
             << "  ./simp_main --file <filename> <limit>\n"
             << "  ./simp_main --manual <number> <limit>\n";
        return 1;
    }

    string mode = argv[1];
    string sourceInfo;
    BigInt number;
    int limit = stoi(argv[argc - 1]);

    // --- Режимы выбора числа ---
    if (mode == "--mersenne") {
        int p = stoi(argv[2]);
        sourceInfo = "число Мерсенна (p = " + to_string(p) + ")";
        auto start = chrono::high_resolution_clock::now();
        number = BigInt("2").power(p).subtraction(BigInt("1"));
        auto end = chrono::high_resolution_clock::now();
        double seconds = chrono::duration<double>(end - start).count();
        ofstream t("mersenne_time.txt");
        t << "Вычисление 2^" << p << " - 1 заняло " << seconds << " сек\n";
        t.close();
    }
    else if (mode == "--file") {
        ifstream fin(argv[2]);
        string s; fin >> s;
        number = BigInt(s);
        sourceInfo = "из файла " + string(argv[2]);
    }
    else if (mode == "--manual") {
        number = BigInt(argv[2]);
        sourceInfo = "введено вручную";
    }
    else {
        cerr << "Unknown mode.\n";
        return 1;
    }

    // --- Запись результата ---
    ofstream out("results.txt");
    out << "Исходное число: " << number.toString() << "\n";
    out << "Источник: " << sourceInfo << "\n\n";
    out << "--- Проверки ---\n";

    // --- Признаки делимости ---
    writeLine(out, "Чётность", DivBy2(number) ? "чётное" : "нечётное");
    writeLine(out, "Делимость на 5", DivBy5(number) ? "да" : "нет");
    writeLine(out, "Делимость на 3/6/9", DivBy3_6_9(number) ? "да" : "нет");

    // --- Простота перебором ---
    writeLine(out, "Проверка делением", PrimeTrialDiv(number) ? "простое" : "составное");

    // --- Решета ---
    auto primesE = Eratosthenes(limit);
    auto primesA = Atkin(limit);
    writeLine(out, "Решето Эратосфена", "простых до " + to_string(limit) + ": " + to_string(primesE.size()));
    writeLine(out, "Решето Аткина", "простых до " + to_string(limit) + ": " + to_string(primesA.size()));

    // --- Тест Люка–Лемера (только если Мерсенн) ---
    if (mode == "--mersenne") {
        int p = stoi(argv[2]);
        writeLine(out, "Тест Люка–Лемера", lucasLehmerTest(p) ? "простое" : "составное");
    }

    out.close();
    cout << "Результаты сохранены в results.txt\n";
    return 0;
}