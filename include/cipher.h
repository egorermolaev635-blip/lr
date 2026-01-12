#ifndef CIPHER_H
#define CIPHER_H

#include <string>
#include <stdexcept>
#include <random>

/**
 * @brief Шифрует текст (англ/русский) с сохранением регистра.
 * Спецсимволы, цифры и пробелы не трогаются.
 * 
 * @param text Исходный текст
 * @param key Ключ сдвига (1-25 для англ., 1-32 для русс.)
 * @param lang 'E' для английского, 'R' для русского
 * @return Зашифрованный текст
 * @throw std::invalid_argument если ключ вне диапазона
 */
std::string encryptCaesar(const std::string& text, int key, char lang);

/**
 * @brief Дешифрует текст, зашифрованный шифром Цезаря.
 * 
 * @param text Зашифрованный текст
 * @param key Ключ сдвига
 * @param lang 'E' для английского, 'R' для русского
 * @return Расшифрованный текст
 */
std::string decryptCaesar(const std::string& text, int key, char lang);

/**
 * @brief Проверка допустимости ключа (1-25 для англ., 1-32 для русск.).
 * 
 * @param key Значение ключа
 * @param lang 'E' для английского, 'R' для русского
 * @return true если ключ валиден, false иначе
 */
bool isValidKey(int key, char lang);

/**
 * @brief Генерация случайного ключа.
 * 
 * @param lang 'E' для английского, 'R' для русского
 * @return Случайный ключ
 */
int generateRandomKey(char lang);

/**
 * @brief Размер алфавита: 26 для англ., 33 для русск.
 * 
 * @param lang 'E' для английского, 'R' для русского
 * @return Размер алфавита
 */
int getAlphabetSize(char lang);

/**
 * @brief Получить сообщение об ошибке диапазона ключа.
 * 
 * @param lang 'E' для английского, 'R' для русского
 * @return Строка с диапазоном
 */
std::string getKeyRangeError(char lang);

#endif // CIPHER_H
