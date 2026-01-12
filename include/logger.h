#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>
#include "json_parser.h"

/**
 * @brief Запись логирования операции
 */
struct LogEntry {
    std::string timestamp;      // формат: YYYY-MM-DDTHH:MM:SS
    std::string operation;      // "encrypt" или "decrypt"
    int key;                    // используемый ключ
    int id;                     // ID записи
    std::string status;         // "успешно" или "ошибка"
    std::string message;        // доп. информация

    JsonValue toJson() const;
    static LogEntry fromJson(const JsonValue& value);
};

/**
 * @brief Класс для логирования операций шифрования
 */
class Logger {
private:
    std::vector<LogEntry> entries;
    std::string logFile;

public:
    explicit Logger(const std::string& filename = "operations.log");

    /**
     * @brief Логирует операцию
     */
    void log(const std::string& operation, int key, int id,
             const std::string& status, const std::string& message = "");

    /**
     * @brief Сохраняет логи в файл
     */
    bool saveToFile();

    /**
     * @brief Загружает логи из файла
     */
    bool loadFromFile();

    /**
     * @brief Получить все записи логов
     */
    const std::vector<LogEntry>& getEntries() const { return entries; }

    /**
     * @brief Очистить логи
     */
    void clear() { entries.clear(); }

    /**
     * @brief Получить количество записей
     */
    size_t size() const { return entries.size(); }

    /**
     * @brief Вывести логи в консоль
     */
    void printToConsole() const;
};

/**
 * @brief Получить текущее время в формате ISO 8601
 */
std::string getCurrentTimestamp();

#endif // LOGGER_H
