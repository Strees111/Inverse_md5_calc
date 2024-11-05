cmake_minimum_required(VERSION 3.29)

project(KS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -DOPENSSL_API_COMPAT=30000")
set(CMAKE_CXX_STANDARD 14)

# Download Google TEST
include(FetchContent)

FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)

FetchContent_Declare(
        openssl
        URL https://www.openssl.org/source/openssl-1.1.1k.tar.gz  # Версию можно изменить
)

# Добавляем флаг для Windows API
if (WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)  # Опционально, чтобы исключить неиспользуемые заголовки Windows
endif()

FetchContent_MakeAvailable(openssl)

find_package(OpenSSL REQUIRED)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(OpenSSL REQUIRED)

add_executable(KS main.cpp MD5.cpp)
add_executable(tests MD5.cpp tests.cpp)

target_link_libraries(tests gtest ssl crypto gtest_main pthread)
target_link_libraries(KS ssl crypto)