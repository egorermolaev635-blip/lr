#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <string>
#include <vector>
#include <map>
#include <memory>

/**
 * @brief Простой JSON-парсер для нужд лабораторной работы
 */
enum class JsonType {
    Null,
    Boolean,
    Number,
    String,
    Array,
    Object
};

struct JsonValue {
    JsonType type;
    bool boolValue;
    double numberValue;
    std::string stringValue;
    std::vector<JsonValue> arrayValue;
    std::map<std::string, JsonValue> objectValue;

    JsonValue() : type(JsonType::Null), boolValue(false), numberValue(0.0) {}
    JsonValue(bool b) : type(JsonType::Boolean), boolValue(b), numberValue(0.0) {}
    JsonValue(double n) : type(JsonType::Number), boolValue(false), numberValue(n) {}
    JsonValue(const std::string& s)
        : type(JsonType::String), boolValue(false), numberValue(0.0), stringValue(s) {}

    std::string asString() const;
    double asNumber() const;
    bool asBoolean() const;
};

/**
 * @brief Парсинг JSON-строки
 */
JsonValue parseJson(const std::string& jsonStr);

/**
 * @brief Преобразование JsonValue в строку
 */
std::string jsonToString(const JsonValue& value, bool pretty = true);

/**
 * @brief Загрузка JSON из файла
 */
JsonValue loadJsonFile(const std::string& filename);

/**
 * @brief Сохранение JSON в файл
 */
void saveJsonFile(const std::string& filename, const JsonValue& value, bool pretty = true);

/**
 * @brief Проверка валидности JSON
 */
bool isValidJson(const std::string& jsonStr, std::string& errorMsg);

#endif // JSON_PARSER_H
