cmake_minimum_required(VERSION 3.10)
project(CaesarCipher CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Основная программа
add_executable(caesar_cipher
    main.cpp
    cipher.cpp
    json_parser.cpp
    logger.cpp
)

# Тесты
add_executable(test_cipher
    test_cipher.cpp
    cipher.cpp
    json_parser.cpp
    logger.cpp
)

add_executable(test_json
    test_json.cpp
    json_parser.cpp
)

# Включаемые директории
target_include_directories(caesar_cipher PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(test_cipher PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(test_json PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Флаги компилятора для разных компиляторов
if(MSVC)
    target_compile_options(caesar_cipher PRIVATE /W4)
    target_compile_options(test_cipher PRIVATE /W4)
    target_compile_options(test_json PRIVATE /W4)
else()
    target_compile_options(caesar_cipher PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(test_cipher PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(test_json PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Включить тестирование
enable_testing()

add_test(NAME CipherTests COMMAND test_cipher)
add_test(NAME JsonTests COMMAND test_json)
