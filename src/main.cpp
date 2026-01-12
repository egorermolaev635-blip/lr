#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "cipher.h"
#include "json_parser.h"
#include "logger.h"

using namespace std;

vector<map<string, JsonValue>> currentData;
Logger logger("data/operations.log");
string currentInputFile;

void printHeader() {
    cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    cout << "║ ШИФР ЦЕЗАРЯ - ИТОГОВАЯ ЛАБОРАТОРНАЯ РАБОТА                    ║\n";
    cout << "║ Вариант 4                                                      ║\n";
    cout << "╚════════════════════════════════════════════════════════════════╝\n\n";
}

void printHelp() {
    cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    cout << "║ СПРАВКА ПО ПРОГРАММЕ                                           ║\n";
    cout << "╚════════════════════════════════════════════════════════════════╝\n\n";
    cout << "ИСПОЛЬЗОВАНИЕ:\n";
    cout << " caesar_cipher [OPTIONS]\n\n";
    cout << "ОПЦИИ КОМАНДНОЙ СТРОКИ:\n";
    cout << " --help                     Показать эту справку\n";
    cout << " --mode ENC|DEC             Режим: enc (шифрование), dec (дешифрование)\n";
    cout << " --key N                    Ключ сдвига (1-25 для англ., 1-32 для русс.)\n";
    cout << " --key random               Использовать случайный ключ\n";
    cout << " --input FILE               Входной JSON файл\n";
    cout << " --output FILE              Выходной JSON файл\n";
    cout << " --ids ID1,ID2,ID3          Обработать только эти ID (опционально)\n\n";
    cout << "ПРИМЕРЫ:\n";
    cout << " caesar_cipher\n";
    cout << "   Запустить интерактивное меню\n\n";
    cout << " caesar_cipher --mode enc --key 7 --input input.json --output output.json\n";
    cout << "   Зашифровать с ключом 7\n\n";
}

void printMenu() {
    cout << "\n┌────────────────────────────────┐\n";
    cout << "│ ГЛАВНОЕ МЕНЮ                   │\n";
    cout << "├────────────────────────────────┤\n";
    cout << "│ 1 - Загрузить JSON файл        │\n";
    cout << "│ 2 - Зашифровать текст          │\n";
    cout << "│ 3 - Расшифровать текст         │\n";
    cout << "│ 4 - Показать логи операций     │\n";
    cout << "│ 5 - Сохранить результаты       │\n";
    cout << "│ 0 - Выход                      │\n";
    cout << "└────────────────────────────────┘\n";
    cout << "\nВыберите пункт (0-5): ";
}

bool loadJsonData(const string& filename) {
    try {
        currentInputFile = filename;
        JsonValue data = loadJsonFile(filename);
        currentData.clear();
        
        if (data.type == JsonType::Array) {
            for (const auto& item : data.arrayValue) {
                if (item.type == JsonType::Object) {
                    map<string, JsonValue> record;
                    for (const auto& [key, value] : item.objectValue) {
                        record[key] = value;
                    }
                    currentData.push_back(record);
                }
            }
        }
        cout << "✓ Загружено " << currentData.size() << " записей из " << filename << "\n";
        return true;
    } catch (const exception& e) {
        cout << "✗ Ошибка при загрузке файла: " << e.what() << "\n";
        return false;
    }
}

void processEncryption(int key, char lang, bool isEncryption) {
    if (currentData.empty()) {
        cout << "✗ Сначала загрузите данные (пункт 1)\n";
        return;
    }

    string operation = isEncryption ? "encrypt" : "decrypt";
    string operationRu = isEncryption ? "Шифрование" : "Расшифрование";
    
    cout << "\n" << operationRu << " (ключ = " << key << "):\n";
    cout << "─────────────────────────────────────────────────────────────\n";

    int successCount = 0;
    for (auto& record : currentData) {
        if (record.find("content") == record.end()) {
            cout << "⚠ Пропущена запись без поля 'content'\n";
            continue;
        }

        try {
            string originalContent = record["content"].asString();
            string processedContent;
            
            if (isEncryption) {
                processedContent = encryptCaesar(originalContent, key, lang);
            } else {
                processedContent = decryptCaesar(originalContent, key, lang);
            }

            int recordId = record["id"].type == JsonType::Number ?
                static_cast<int>(record["id"].asNumber()) : -1;

            record["processed_content"] = JsonValue(processedContent);
            record["key_used"] = JsonValue(static_cast<double>(key));
            record["operation"] = JsonValue(operation);

            logger.log(operation, key, recordId, "успешно", "");

            cout << "ID " << recordId << ": " << originalContent.substr(0, 40);
            if (originalContent.length() > 40) cout << "...";
            cout << "\n → " << processedContent.substr(0, 40);
            if (processedContent.length() > 40) cout << "...";
            cout << "\n";

            successCount++;
        } catch (const exception& e) {
            cout << "✗ Ошибка: " << e.what() << "\n";
            logger.log(operation, key, -1, "ошибка", e.what());
        }
    }

    cout << "─────────────────────────────────────────────────────────────\n";
    cout << "✓ Обработано " << successCount << " из " << currentData.size() << " записей\n";
}

void saveResults() {
    if (currentData.empty()) {
        cout << "✗ Нет данных для сохранения\n";
        return;
    }

    cout << "Введите имя файла для сохранения (или Enter для \"output.json\"): ";
    string filename;
    getline(cin, filename);
    if (filename.empty()) {
        filename = "output.json";
    }

    try {
        JsonValue arr;
        arr.type = JsonType::Array;
        for (const auto& record : currentData) {
            JsonValue obj;
            obj.type = JsonType::Object;
            for (const auto& [key, value] : record) {
                obj.objectValue[key] = value;
            }
            arr.arrayValue.push_back(obj);
        }

        saveJsonFile(filename, arr, true);
        cout << "✓ Результаты сохранены в " << filename << "\n";
    } catch (const exception& e) {
        cout << "✗ Ошибка при сохранении: " << e.what() << "\n";
    }
}

void interactiveMenu() {
    printHeader();
    
    while (true) {
        printMenu();
        string choice;
        getline(cin, choice);

        if (choice == "0") {
            cout << "\nСохранить логи перед выходом? (y/n): ";
            string answer;
            getline(cin, answer);
            if (answer == "y" || answer == "Y") {
                if (logger.saveToFile()) {
                    cout << "✓ Логи сохранены\n";
                }
            }
            cout << "\nДо свидания!\n\n";
            break;
        } else if (choice == "1") {
            cout << "\nВведите путь к JSON файлу: ";
            string filepath;
            getline(cin, filepath);
            if (!filepath.empty()) {
                loadJsonData(filepath);
            }
        } else if (choice == "2") {
            if (currentData.empty()) {
                cout << "✗ Сначала загрузите данные\n";
                continue;
            }

            cout << "\nВыберите язык (E - английский, R - русский): ";
            string lang;
            getline(cin, lang);
            if (lang != "E" && lang != "R" && lang != "e" && lang != "r") {
                cout << "✗ Неверный выбор языка\n";
                continue;
            }

            cout << "Введите ключ (1-25 для англ., 1-32 для русс., или 'random'): ";
            string keyStr;
            getline(cin, keyStr);
            int key;
            if (keyStr == "random") {
                key = generateRandomKey(lang[0]);
                cout << "Сгенерирован случайный ключ: " << key << "\n";
            } else {
                try {
                    key = stoi(keyStr);
                } catch (...) {
                    cout << "✗ Некорректный ключ\n";
                    continue;
                }
            }

            if (!isValidKey(key, lang[0])) {
                cout << "✗ " << getKeyRangeError(lang[0]) << "\n";
                continue;
            }

            processEncryption(key, lang[0], true);
        } else if (choice == "3") {
            if (currentData.empty()) {
                cout << "✗ Сначала загрузите данные\n";
                continue;
            }

            cout << "\nВыберите язык (E - английский, R - русский): ";
            string lang;
            getline(cin, lang);
            if (lang != "E" && lang != "R" && lang != "e" && lang != "r") {
                cout << "✗ Неверный выбор языка\n";
                continue;
            }

            cout << "Введите ключ (1-25 для англ., 1-32 для русс.): ";
            string keyStr;
            getline(cin, keyStr);
            int key;
            try {
                key = stoi(keyStr);
            } catch (...) {
                cout << "✗ Некорректный ключ\n";
                continue;
            }

            if (!isValidKey(key, lang[0])) {
                cout << "✗ " << getKeyRangeError(lang[0]) << "\n";
                continue;
            }

            processEncryption(key, lang[0], false);
        } else if (choice == "4") {
            logger.printToConsole();
        } else if (choice == "5") {
            saveResults();
        } else {
            cout << "✗ Неверный выбор\n";
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        string arg = argv[1];
        if (arg == "--help" || arg == "-h") {
            printHelp();
            return 0;
        }
    }

    interactiveMenu();
    return 0;
}
