cmake_minimum_required(VERSION 3.10)
project(lab_02)

# Kompilator: Włącz ostrzeżenia i debugowanie
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -g")

# Znajdujemy wszystkie pliki .cpp i .h w katalogach src/ i inc/
file(GLOB SRC_FILES "src/*.cpp")
file(GLOB HEADER_FILES "inc/*.h")

# Dodajemy katalog inc/ do ścieżek przeszukiwania plików nagłówkowych
include_directories(${CMAKE_SOURCE_DIR}/inc)

# Znajdujemy wszystkie pliki w katalogu data/
file(GLOB DATA_FILES "data/*")

# Tworzymy plik wykonywalny
add_executable(lab_02 ${SRC_FILES} ${HEADER_FILES})

# Tworzymy katalog docelowy, jeśli nie istnieje
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)

# Kopiujemy wszystkie pliki z katalogu data/ do katalogu binarnego
foreach(DATA_FILE ${DATA_FILES})
    get_filename_component(FILE_NAME ${DATA_FILE} NAME)
    configure_file(${DATA_FILE} ${CMAKE_BINARY_DIR}/data/${FILE_NAME} COPYONLY)
endforeach()

# Dodajemy pliki jako zasoby
add_custom_target(copy_data_files ALL DEPENDS ${DATA_FILES})