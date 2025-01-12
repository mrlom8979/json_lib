# Поиск всех .cpp и .h файлов
file(GLOB_RECURSE LIB_SOURCES "include/*.cpp")
file(GLOB_RECURSE LIB_HEADERS "include/*.h")

# Создание библиотеки
add_library(json STATIC ${LIB_SOURCES} ${LIB_HEADERS})

# Подключение предкомпилированного заголовка
target_precompile_headers(json PRIVATE include/pch.h)

# Установка директорий для поиска заголовков
target_include_directories(json PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Настройка платформо-зависимых флагов
if (UNIX)
    target_compile_definitions(json PRIVATE __linux__)
    target_compile_options(json PRIVATE -Wall -Wextra -pedantic)
elseif(WIN32)
    target_compile_definitions(json PRIVATE _WIN32)
    target_compile_options(json PRIVATE /W4)
endif()

