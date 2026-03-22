#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

// ---------- RLE Encode ----------
std::string rle_encode(const std::string& input) {
    if (input.empty()) return "";

    std::string output;
    size_t count = 1;

    for (size_t i = 1; i <= input.size(); ++i) {
        if (i < input.size() && input[i] == input[i - 1]) {
            ++count;
        } else {
            output += std::to_string(count);
            output += input[i - 1];
            count = 1;
        }
    }
    return output;
}

// ---------- RLE Decode ----------
std::string rle_decode(const std::string& input) {
    std::string output;
    int count = 0;

    for (char c : input) {
        if (std::isdigit(c)) {
            count = count * 10 + (c - '0');
        } else {
            if (count == 0) count = 1;
            output.append(count, c);
            count = 0;
        }
    }
    return output;
}

// ---------- File Read ----------
bool read_file(const std::string& filename, std::string& content) {
    std::ifstream file(filename);
    if (!file) return false;

    content.assign(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
    return true;
}

// ---------- File Write ----------
bool write_file(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file) return false;

    file << content;
    return true;
}

// ---------- Main ----------
int main() {
    while (true) {
        std::cout << "\n=== RLE Архиватор ===\n"
                  << "1. Сжать файл\n"
                  << "2. Распаковать файл\n"
                  << "0. Выход\n"
                  << "Выбор: ";

        int choice;
        std::cin >> choice;

        if (choice == 0) break;

        if (choice != 1 && choice != 2) {
            std::cout << "Ошибка: неверный выбор\n";
            continue;
        }

        std::string filename;
        std::cout << "Имя файла: ";
        std::cin >> filename;

        std::string content;
        if (!read_file(filename, content)) {
            std::cout << "Не удалось открыть файл\n";
            continue;
        }

        if (content.empty()) {
            std::cout << "Файл пуст\n";
            continue;
        }

        std::string result =
            (choice == 1) ? rle_encode(content)
                          : rle_decode(content);

        if (!write_file(filename, result)) {
            std::cout << "Ошибка записи файла\n";
            continue;
        }

        std::cout << "Готово!\n"
                  << "До: " << content.size()
                  << " | После: " << result.size() << "\n";
    }

    return 0;
}
