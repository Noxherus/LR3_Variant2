#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

// Лабораторная работа № 3. Вариант 2 (средний уровень и дополнительные задания).
class Date2 {
private:
    int day_;
    int month_;
    int year_;
    static int createdCount_;

    static bool isLeapYear(int year) {
        return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
    }

    static int daysInMonth(int month, int year) {
        const int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        if (month == 2 && isLeapYear(year)) {
            return 29;
        }
        return days[month - 1];
    }

    static bool isValidDate(int day, int month, int year) {
        return year >= 1 && year <= 9999 && month >= 1 && month <= 12 &&
               day >= 1 && day <= daysInMonth(month, year);
    }

public:
    // Конструкторы и деструктор.
    Date2() : day_(1), month_(1), year_(2000) {
        ++createdCount_;
    }

    Date2(int day, int month, int year) : day_(day), month_(month), year_(year) {
        if (!isValidDate(day, month, year)) {
            throw std::invalid_argument("Invalid date");
        }
        ++createdCount_;
    }

    Date2(const Date2& other)
        : day_(other.day_), month_(other.month_), year_(other.year_) {
        ++createdCount_;
    }

    ~Date2() {
        std::cout << "Destroyed Date2 object: " << *this << '\n';
    }

    // Геттеры и сеттеры с проверкой правильности даты.
    int getDay() const { return day_; }
    int getMonth() const { return month_; }
    int getYear() const { return year_; }

    void setDate(int day, int month, int year) {
        if (!isValidDate(day, month, year)) {
            throw std::invalid_argument("Invalid date");
        }
        day_ = day;
        month_ = month;
        year_ = year;
    }

    void setDay(int day) { setDate(day, month_, year_); }
    void setMonth(int month) { setDate(day_, month, year_); }
    void setYear(int year) { setDate(day_, month_, year); }

    // Задание 2: увеличение года на один.
    void addOneYear() {
        if (year_ == 9999) {
            throw std::out_of_range("Year cannot exceed 9999");
        }
        const int newYear = year_ + 1;
        const int lastDay = daysInMonth(month_, newYear);
        if (day_ > lastDay) {
            day_ = lastDay; // 29 февраля становится 28 февраля в невисокосном году.
        }
        year_ = newYear;
    }

    // Задание 2: уменьшение даты на два календарных дня.
    void subtractTwoDays() {
        if (year_ == 1 && month_ == 1 && day_ <= 2) {
            throw std::out_of_range("Date is outside supported range");
        }
        for (int i = 0; i < 2; ++i) {
            if (day_ > 1) {
                --day_;
            } else {
                if (month_ == 1) {
                    month_ = 12;
                    --year_;
                } else {
                    --month_;
                }
                day_ = daysInMonth(month_, year_);
            }
        }
    }

    // Дополнительное задание 1: статический счётчик созданных объектов.
    static int getCreatedCount() { return createdCount_; }

    // Информация об объекте; дополнительное задание 2: оператор <<.
    std::string toString() const {
        std::string d = (day_ < 10 ? "0" : "") + std::to_string(day_);
        std::string m = (month_ < 10 ? "0" : "") + std::to_string(month_);
        return d + "." + m + "." + std::to_string(year_);
    }

    friend std::ostream& operator<<(std::ostream& out, const Date2& date) {
        return out << date.toString();
    }

    // Дополнительное задание 4: сохранение и загрузка из текстового файла.
    bool saveToFile(const std::string& filename) const {
        std::ofstream out(filename);
        if (!out) {
            return false;
        }
        out << day_ << ' ' << month_ << ' ' << year_ << '\n';
        out.flush();
        return static_cast<bool>(out);
    }

    bool loadFromFile(const std::string& filename) {
        std::ifstream in(filename);
        int day, month, year;
        if (!(in >> day >> month >> year)) {
            return false;
        }
        std::string extra;
        if (in >> extra || !isValidDate(day, month, year)) {
            return false;
        }
        setDate(day, month, year);
        return true;
    }
};

int Date2::createdCount_ = 0;

int main() {
    try {
        std::cout << "LAB 3 - VARIANT 2 - DATE2\n\n";

        Date2 fixed(29, 2, 2024);
        Date2 nextYear(fixed);  // Конструктор копирования.
        nextYear.addOneYear();
        std::cout << "Fixed date: " << fixed << '\n';
        std::cout << "After adding one year: " << nextYear << '\n';

        Date2 twoDaysEarlier(2, 3, 2024);
        twoDaysEarlier.subtractTwoDays();
        std::cout << "March 2, 2024 minus two days: " << twoDaysEarlier << "\n\n";

        // Дополнительное задание 3: массив объектов.
        Date2 dates[3] = {
            Date2(31, 12, 2023),
            Date2(1, 1, 2024),
            Date2(29, 2, 2024)
        };
        std::cout << "Array of Date2 objects:\n";
        for (int i = 0; i < 3; ++i) {
            std::cout << "  [" << i << "] " << dates[i] << '\n';
        }

        std::cout << "\nEnter date (day month year), for example 10 5 2024: ";
        int day, month, year;
        if (!(std::cin >> day >> month >> year)) {
            std::cerr << "Input error. Enter three integers.\n";
            return 1;
        }
        Date2 entered(day, month, year);
        std::cout << "Entered date: " << entered << '\n';
        std::cout << "Using getters: " << entered.getDay() << '/'
                  << entered.getMonth() << '/' << entered.getYear() << '\n';

        Date2 changed(entered);
        try {
            changed.addOneYear();
            std::cout << "After adding one year: " << changed << '\n';
        } catch (const std::out_of_range& ex) {
            std::cout << "Cannot add a year: " << ex.what() << '\n';
        }
        changed.setDate(entered.getDay(), entered.getMonth(), entered.getYear());
        try {
            changed.subtractTwoDays();
            std::cout << "After subtracting two days: " << changed << '\n';
        } catch (const std::out_of_range& ex) {
            std::cout << "Cannot subtract two days: " << ex.what() << '\n';
        }

        // Проверка сеттеров и сохранения/загрузки данных.
        Date2 restored;
        restored.setDate(1, 1, 2001);
        restored.setDay(2);
        restored.setMonth(2);
        restored.setYear(2002);
        std::cout << "Date changed using setters: " << restored << '\n';
        try {
            restored.setMonth(13);
        } catch (const std::invalid_argument&) {
            std::cout << "Invalid month was rejected.\n";
        }

        if (!entered.saveToFile("date2.txt")) {
            std::cerr << "Cannot save file date2.txt\n";
            return 1;
        }
        if (!restored.loadFromFile("date2.txt")) {
            std::cerr << "Cannot load file date2.txt\n";
            return 1;
        }
        std::cout << "Loaded from date2.txt: " << restored << '\n';
        std::cout << "Total Date2 objects created: " << Date2::getCreatedCount() << '\n';
        std::cout << "Program completed successfully.\n";
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}
