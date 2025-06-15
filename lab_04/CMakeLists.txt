cmake_minimum_required(VERSION 3.10)
project(flowshop)

# Ustaw standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Wszystkie pliki źródłowe
set(SOURCES
        src/main.cpp
        src/flowshop.cpp
)

set(HEADERS
        src/flowshop.h
)

# Dodaj executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Ustaw katalogi includów
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Skopiuj pliki instancji do katalogu wynikowego
file(COPY ${CMAKE_SOURCE_DIR}/instances DESTINATION ${CMAKE_BINARY_DIR})

# Ustaw katalog wyjściowy dla pliku wykonywalnego
set_target_properties(${PROJECT_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)