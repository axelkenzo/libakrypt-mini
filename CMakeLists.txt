# -------------------------------------------------------------------------------------------------- #
# сборка по умолчанию
# cmake path && make
# где path это путь к каталогу, в котором расположен файл MakeLists.txt

# сборка с использованием с использованием clang
# cmake -D CMAKE_C_COMPILER=clang path && make

# сборка с использованием с использованием TinyCC
# cmake -D CMAKE_C_COMPILER=tcc path && make

# сборка при помощи MSVC из консоли (Visual Studio Console Prompt)
# cmake -G "NMake Makefiles" path && nmake

# сборка при MinGW из консоли MSYS (WINDOWS)
# cmake -G "MinGW Makefiles" -D CMAKE_C_COMPILER=gcc path && mingw32-make
# -------------------------------------------------------------------------------------------------- #
project( libakrypt-mini C )
cmake_minimum_required( VERSION 3.2 FATAL_ERROR )

set( HEAD_VERSION 0 )
set( MAIN_VERSION 2 )
set( MINOR_VERSION 2 )

set( MAJOR_VERSION ${HEAD_VERSION}.${MAIN_VERSION} )
set( FULL_VERSION ${MAJOR_VERSION}.${MINOR_VERSION} )

# -------------------------------------------------------------------------------------------------- #
# для включения опции: cmake -D LIBAKRYPT_SHARED_LIB=ON path
# -------------------------------------------------------------------------------------------------- #
option( LIBAKRYPT_HASH "Build the hash functions" OFF )
option( LIBAKRYPT_SIGN "Build the digital signature functions" OFF )
option( LIBAKRYPT_CIPHER "Build the Kuznechik encryption/decryption procedures" OFF )
option( LIBAKRYPT_KEYS "Build the key derivation functions and procedures" OFF )
option( LIBAKRYPT_STATIC_LIB "Build the static library" OFF )
option( LIBAKRYPT_SHARED_LIB "Build the shared library" OFF )
option( LIBAKRYPT_TESTS "Build the tests and example programs" OFF )

# -------------------------------------------------------------------------------------------------- #
# удовлетворение зависимостей
# -------------------------------------------------------------------------------------------------- #
if( LIBAKRYPT_TESTS )
  set( LIBAKRYPT_SIGN ON )
  set( LIBAKRYPT_CIPHER ON )
  set( LIBAKRYPT_KEYS ON )
endif()
if( LIBAKRYPT_SIGN )
  set( LIBAKRYPT_HASH ON )
endif()
if( LIBAKRYPT_KEYS )
  set( LIBAKRYPT_HASH ON )
endif()

# -------------------------------------------------------------------------------------------------- #
# основной заголовок
# -------------------------------------------------------------------------------------------------- #
set( MAIN_HEADER    source/libakrypt-mini.h )

# -------------------------------------------------------------------------------------------------- #
# код, реализующий операции с функциями хэширования
# -------------------------------------------------------------------------------------------------- #
set( HASH_SOURCES   source/ak_streebog.c
                    source/ak_hmac.c )

# -------------------------------------------------------------------------------------------------- #
# код, реализующий операции с электронной подписью
# -------------------------------------------------------------------------------------------------- #
set( SIGN_SOURCES   source/ak_mpzn.c
                    source/ak_curves.c
                    source/ak_sign.c )

# -------------------------------------------------------------------------------------------------- #
# код, реализующий алгоритм блочного шифрования Кузнечик
# -------------------------------------------------------------------------------------------------- #
set( CIPHER_SOURCES   source/ak_kuznechik.c )

# -------------------------------------------------------------------------------------------------- #
# код, реализующий выработку производных ключей
# -------------------------------------------------------------------------------------------------- #
set( KEYS_SOURCES   source/ak_kdf.c )

# -------------------------------------------------------------------------------------------------- #
# тестовые примеры
# -------------------------------------------------------------------------------------------------- #
set( TESTS          hash
                    hspeed
                    sign
                    pbkdf2
                    keys )

# -------------------------------------------------------------------------------------------------- #
# служебные и информационные файлы
# -------------------------------------------------------------------------------------------------- #
set( OTHERS         CMakeLists.txt
                    Makefile
                    Readme.md
                    example.sh )

# -------------------------------------------------------------------------------------------------- #
# определение перечня собираемых модулей
# -------------------------------------------------------------------------------------------------- #
if( LIBAKRYPT_HASH )
 set( SOURCES ${SOURCES} ${HASH_SOURCES} )
endif()
if( LIBAKRYPT_SIGN )
 set( SOURCES ${SOURCES} ${SIGN_SOURCES} )
endif()
if( LIBAKRYPT_CIPHER )
 set( SOURCES ${SOURCES} ${CIPHER_SOURCES} )
endif()
if( LIBAKRYPT_KEYS )
 set( SOURCES ${SOURCES} ${KEYS_SOURCES} )
endif()
message( "-- List of source files: ${SOURCES}" )

# -------------------------------------------------------------------------------------------------- #
# определение собираемых библиотек (за один раз собирается только одна библиотека)
# -------------------------------------------------------------------------------------------------- #
if( LIBAKRYPT_STATIC_LIB )
  set( LIBAKRYPT_SHARED_LIB OFF )
endif()
if( LIBAKRYPT_SHARED_LIB )
  set( LIBAKRYPT_STATIC_LIB OFF )
endif()

# -------------------------------------------------------------------------------------------------- #
# настройка флагов сборки исходных текстов
# -------------------------------------------------------------------------------------------------- #
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -pedantic-errors -funroll-loops" )

# -------------------------------------------------------------------------------------------------- #
# проверка архитектуры
# -------------------------------------------------------------------------------------------------- #
include( TestBigEndian )
TEST_BIG_ENDIAN( IS_BIG_ENDIAN )
if( IS_BIG_ENDIAN )
  set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLIBAKRYPT_BIG_ENDIAN" )
else()
  set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLIBAKRYPT_LITTLE_ENDIAN" )
endif()

include_directories( "source" )
if( LIBAKRYPT_STATIC_LIB )
  add_library( akrypt-mini STATIC ${MAIN_HEADER} ${SOURCES} )
  set_target_properties( akrypt-mini PROPERTIES VERSION ${MAJOR_VERSION} SOVERSION ${FULL_VERSION} )
  message( "-- Added a build of static library" )
endif()

if( LIBAKRYPT_SHARED_LIB )
  add_library( akrypt-mini SHARED ${MAIN_HEADER} ${SOURCES} )
  set_target_properties( akrypt-mini PROPERTIES VERSION ${MAJOR_VERSION} SOVERSION ${FULL_VERSION} )
  message( "-- Added a build of shared library" )
endif()

# -------------------------------------------------------------------------------------------------- #
# вывод собранных настроек
# -------------------------------------------------------------------------------------------------- #
message( "-- Use flags: ${CMAKE_C_FLAGS}" )

# -------------------------------------------------------------------------------------------------- #
# определение перечня собираемых тестовых примеров
# -------------------------------------------------------------------------------------------------- #
if( LIBAKRYPT_TESTS )
  foreach( programm ${TESTS} )
    add_executable( ${programm} test/test_${programm}.c )
    target_link_libraries( ${programm} akrypt-mini )
  endforeach()
endif()

# -------------------------------------------------------------------------------------------------- #
# генерация файла для сборки архива (только для UNIX)
# -------------------------------------------------------------------------------------------------- #
set( MYDFILE "${CMAKE_BINARY_DIR}/make-dist-${FULL_VERSION}.sh" )
if( CMAKE_HOST_UNIX )
  file( WRITE ${MYDFILE} "#!/bin/bash\n" )

  # создаем каталог и копируем файлы с исходными текстами
  file( APPEND ${MYDFILE} "mkdir -p libakrypt-mini-${FULL_VERSION}/source\n")
  file( APPEND ${MYDFILE} "cp -fL --preserve=all ${CMAKE_SOURCE_DIR}/source/* libakrypt-mini-${FULL_VERSION}/source\n")
  file( APPEND ${MYDFILE} "mkdir -p libakrypt-mini-${FULL_VERSION}/test\n")
  file( APPEND ${MYDFILE} "cp -fL --preserve=all ${CMAKE_SOURCE_DIR}/test/* libakrypt-mini-${FULL_VERSION}/test\n")
  foreach( file ${OTHERS} )
    file( APPEND ${MYDFILE} "cp -fL --preserve=all ${CMAKE_SOURCE_DIR}/${file} libakrypt-mini-${FULL_VERSION}\n")
  endforeach()

  # собираем дистрибутив
  file( APPEND ${MYDFILE} "tar -cjvf libakrypt-mini-${FULL_VERSION}.tar.bz2 libakrypt-mini-${FULL_VERSION}\n")
  file( APPEND ${MYDFILE} "rm -R libakrypt-mini-${FULL_VERSION}\n")
  # file( APPEND ${MYDFILE} "akrypt i -r -o libakrypt-mini-${FULL_VERSION}.streebog256 libakrypt-mini-${FULL_VERSION}.tar.bz2\n")

  message("-- Creating a ${MYDFILE} file - done ")
  execute_process( COMMAND chmod +x ${MYDFILE} )
  add_custom_target( dist ${MYDFILE} )
endif()

