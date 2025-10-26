#include "bigint.h"
#include <sstream>
#include <algorithm>

// Конструктор по умолчанию
BigInt::BigInt() {}

// Конструктор из строки (разбиваем число на куски по BASE_DIGITS)
BigInt::BigInt(const string& number) {
    for (int i = number.size(); i > 0; i -= BASE_DIGITS) {
        int start = max(0, i - BASE_DIGITS);
        int len = i - start;
        digits.push_back(stoi(number.substr(start, len)));
    }
    removeLeadingZeros();
}

// Перевод числа в строку
string BigInt::toString() const {
    if (digits.empty()) return "0";
    ostringstream oss;
    oss << digits.back();
    for (int i = (int)digits.size() - 2; i >= 0; i--) {
        oss.width(BASE_DIGITS);
        oss.fill('0');
        oss << digits[i];
    }
    return oss.str();
}

// Удаляем ведущие нули
void BigInt::removeLeadingZeros() {
    while (!digits.empty() && digits.back() == 0) {
        digits.pop_back();
    }
    if (digits.empty()) digits.push_back(0);
}

// Сложение: num_a + num_b
BigInt BigInt::addition(const BigInt& num_b) const {
    BigInt result;
    int carry = 0;
    size_t n = max(digits.size(), num_b.digits.size());
    result.digits.resize(n);

    for (size_t i = 0; i < n; i++) {
        long long sum = carry;
        if (i < digits.size()) sum += digits[i];
        if (i < num_b.digits.size()) sum += num_b.digits[i];
        carry = sum >= BASE;
        if (carry) sum -= BASE;
        result.digits[i] = sum;
    }
    if (carry) result.digits.push_back(carry);
    return result;
}

// Вычитание: |num_a - num_b|
BigInt BigInt::subtraction(const BigInt& num_b) const {
    if (this->compare(num_b) < 0) {
        // Если num_a < num_b: num_b - num_a
        return num_b.subtraction(*this);
    }

    BigInt result;
    result.digits.resize(digits.size());
    int carry = 0;
    for (size_t i = 0; i < digits.size(); i++) {
        long long diff = digits[i] - carry - (i < num_b.digits.size() ? num_b.digits[i] : 0);
        carry = diff < 0;
        if (carry) diff += BASE;
        result.digits[i] = diff;
    }
    result.removeLeadingZeros();
    return result;
}

/*
// Умножение двух чисел: num_a * num_b
BigInt BigInt::multiplication(const BigInt& num_b) const {
    // Выбираем число с меньшим количеством блоков для внешнего цикла
    const BigInt& smaller = (digits.size() < num_b.digits.size()) ? *this : num_b;
    const BigInt& larger  = (digits.size() < num_b.digits.size()) ? num_b : *this;

    BigInt result;
    result.digits.assign(smaller.digits.size() + larger.digits.size(), 0);

    for (size_t i = 0; i < smaller.digits.size(); i++) {
        long long carry = 0;
        for (size_t j = 0; j < larger.digits.size() || carry; j++) {
            long long cur = result.digits[i + j] +
                            1LL * smaller.digits[i] * (j < larger.digits.size() ? larger.digits[j] : 0) +
                            carry;
            result.digits[i + j] = int(cur % BASE);
            carry = int(cur / BASE);
        }
    }

    result.removeLeadingZeros();
    return result;
}

/// Деление: num_a / num_b
BigInt BigInt::division(const BigInt& num_b) const {
    // Если num_b = 0 ошибка
    if (num_b.digits.size() == 1 && num_b.digits[0] == 0) {
        throw runtime_error("Division by zero");
    }

    // Если num_a < num_b
    if (this->compare(num_b) < 0) {
        return BigInt("0");
    }

    BigInt result, curValue;
    result.digits.resize(digits.size());

    for (int i = (int)digits.size() - 1; i >= 0; i--) {
        // Сдвигаем "остаток"
        curValue.digits.insert(curValue.digits.begin(), digits[i]);
        curValue.removeLeadingZeros();

        // Подбираем максимальное x num_b * x <= curValue - бинарный поиск
        int x = 0, left = 0, right = BASE - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            BigInt cur = num_b.multiplication(BigInt(to_string(mid)));
            if (cur.compare(curValue) <= 0) {
                x = mid;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        result.digits[i] = x;
        curValue = curValue.subtraction(num_b.multiplication(BigInt(to_string(x))));
    }

    result.removeLeadingZeros();
    return result;
}

*/

// Быстрое умножение (O(n²), но с 64-битами)
BigInt BigInt::multiplication(const BigInt& num_b) const {
    BigInt res;
    res.digits.assign(digits.size() + num_b.digits.size(), 0);
    for (size_t i = 0; i < digits.size(); i++) {
        unsigned long long carry = 0;
        for (size_t j = 0; j < num_b.digits.size() || carry; j++) {
            unsigned long long cur = res.digits[i + j] +
                1ULL * digits[i] * (j < num_b.digits.size() ? num_b.digits[j] : 0ULL) + carry;
            res.digits[i + j] = int(cur % BASE);
            carry = cur / BASE;
        }
    }
    res.removeLeadingZeros();
    return res;
}

// Быстрое деление (с нормализацией)
BigInt BigInt::division(const BigInt& num_b) const {
    if (num_b.compare(BigInt("0")) == 0) throw runtime_error("Division by zero");
    if (compare(num_b) < 0) return BigInt("0");

    BigInt res, cur;
    res.digits.resize(digits.size());

    for (int i = (int)digits.size() - 1; i >= 0; i--) {
        cur.digits.insert(cur.digits.begin(), digits[i]);
        cur.removeLeadingZeros();

        int l = 0, r = BASE - 1, x = 0;
        while (l <= r) {
            int m = (l + r) / 2;
            BigInt prod = num_b.multiplication(BigInt(to_string(m)));
            if (prod.compare(cur) <= 0) { x = m; l = m + 1; } else r = m - 1;
        }
        res.digits[i] = x;
        cur = cur.subtraction(num_b.multiplication(BigInt(to_string(x))));
    }

    res.removeLeadingZeros();
    return res;
}

// Остаток: num_a % num_b
BigInt BigInt::mod(const BigInt& num_b) const {
// Если num_b = 0 → ошибка (деление на ноль)
    if (num_b.digits.size() == 1 && num_b.digits[0] == 0) {
        throw runtime_error("Division by zero");
    }

    // Если num_a < num_b → результат равен 0
    if (this->compare(num_b) < 0) {
        return *this;
    }

    BigInt result, curValue;
    result.digits.resize(digits.size());

    for (int i = (int)digits.size() - 1; i >= 0; i--) {
        // Сдвигаем "остаток"
        curValue.digits.insert(curValue.digits.begin(), digits[i]);
        curValue.removeLeadingZeros();

        // Подбираем максимальное x такое, что num_b * x <= curValue
        int x = 0, left = 0, right = BASE - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            BigInt cur = num_b.multiplication(BigInt(to_string(mid)));
            if (cur.compare(curValue) <= 0) {
                x = mid;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        result.digits[i] = x;
        curValue = curValue.subtraction(num_b.multiplication(BigInt(to_string(x))));
    }

    result.removeLeadingZeros();
    return curValue;
}


// Сравнение: -1 если num_a < num_b, 0 если равно, 1 если больше
int BigInt::compare(const BigInt& num_b) const {
    if (digits.size() != num_b.digits.size())
        return digits.size() < num_b.digits.size() ? -1 : 1;
    for (int i = (int)digits.size() - 1; i >= 0; i--) {
        if (digits[i] != num_b.digits[i])
            return digits[i] < num_b.digits[i] ? -1 : 1;
    }
    return 0;
}

// НОД (Алгоритм Евклида)
//Разделить большее на меньшее, большее заменяется на меньшее и меньшее - на остаток
BigInt BigInt::NOD(const BigInt& num_b) const {
    BigInt a = *this, b = num_b;
    while (!(b.digits.size() == 1 && b.digits[0] == 0)) {
        BigInt r = a.mod(b);
        a = b;
        b = r;
    }
    return a;
}

// НОК: num_a * num_b / NOD(num_a, num_b)
BigInt BigInt::NOK(const BigInt& num_b) const {
    BigInt nod = this->NOD(num_b);
    BigInt mul = this->multiplication(num_b);
    return mul.division(nod);
}

// Возведение в степень: num_a ^ exponent
//показатель степени в двоичном виде и пошаговое возведение в квадрат и умножение на основание
BigInt BigInt::power(unsigned long long exponent) const {
    BigInt result("1");
    BigInt base = *this;
    while (exponent > 0) {
        if (exponent & 1) result = result.multiplication(base);
        base = base.multiplication(base);
        exponent >>= 1;
    }
    return result;
}

BigInt BigInt::sqrt() const {
    // Если число = 0 или 1, возвращаем его же
    if (compare(BigInt("0")) == 0 || compare(BigInt("1")) == 0)
        return *this;

    BigInt left("1");
    BigInt right(*this);
    BigInt mid;
    BigInt result("0");

    while (left.compare(right) <= 0) {
        mid = left.addition(right).division(BigInt("2"));
        BigInt square = mid.multiplication(mid);

        // если mid^2 <= исходное число → запоминаем mid и сдвигаем границу вверх
        if (square.compare(*this) <= 0) {
            result = mid;
            left = mid.addition(BigInt("1"));
        } 
        else {
            // иначе двигаем границу вниз
            right = mid.subtraction(BigInt("1"));
        }
    }
    return result; 
}
