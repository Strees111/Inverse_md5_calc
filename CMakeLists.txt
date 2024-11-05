cmake_minimum_required(VERSION 3.20)

project(inverse_md5_calc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOPENSSL_API_COMPAT=30000")
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)

# Download Google TEST
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
        DOWNLOAD_EXTRACT_TIMESTAMP true
)

# Download OpenSSL
FetchContent_Declare(
        openssl
        URL https://www.openssl.org/source/openssl-1.1.1k.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP true
)

FetchContent_MakeAvailable(openssl)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(OpenSSL REQUIRED)

# Добавляем флаг для Windows API
if (WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)  # Опционально, чтобы исключить неиспользуемые заголовки Windows
endif()

add_executable(inverse_md5_calc main.cpp MD5.cpp)
add_executable(tests MD5.cpp tests.cpp)

target_include_directories(inverse_md5_calc PRIVATE ${OPENSSL_INCLUDE_DIR})
target_include_directories(tests PRIVATE ${OPENSSL_INCLUDE_DIR})

target_link_libraries(tests gtest OpenSSL::SSL OpenSSL::Crypto gtest_main pthread)
target_link_libraries(inverse_md5_calc OpenSSL::SSL OpenSSL::Crypto)