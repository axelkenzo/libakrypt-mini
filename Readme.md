# libakrypt-mini project
Данный проект представляет собой небольшой фрагмент библиотеки [libakrypt](https://github.com/axelkenzo/libakrypt-0.x),
позволяющий:

 - вычислять значение функций хеширования Стрибог256 и Стрибог512 (согласно ГОСТ Р 34.11-2012),
 - вычислять значения функций HMAC-Streebog512 и HMAC-Streebog256,
   а также реализовывать развертку ключевого вектора из пароля (согласно Р 50.1.111-2016, раздел 4),
 - выполнять выработку производных ключей согласно Р 1323565.1.022-2018,
 - проверять значение электронной подписи согласно ГОСТ Р 34.10-2012
   (используется собственная реализация арифметики больших чисел и реализация операций с точками эллиптических кривых).

Основное предназначение данного кода - использование во встраиваемых системах, а также
программах, не использующих стандартную библиотеку libc или ее аналоги.

Несмотря на сказанное,
текущая версия кода все-таки использует следующие функции стандартной библиотеки

 * `void *memcpy( void *dest, const void *src, size_t n );`
 * `void *memset( void *s, int c, size_t n );`
 * `bswap_64()` (для big endian архитектуры)


## Компиляция

Для сборки тестовых примеров, на Intel архитектуре, можно запустить

    gcc -DLIBAKRYPT_LITTLE_ENDIAN=ON -I./source source/ak_*.c test/test_hash.c -o test_hash

Для сборки на big endian архитектуре [подробнее см. на  ellcc.org](http://ellcc.org) можно запустить

    ecc -target mips32r2-linux -I./source source/ak_*.c test/test_hash.c -o test_hash

Прилагаемый Makefile можно использовать для сборки тестовых примеров следующим образом

    make intel

или

    make mips


Можно также воспользоваться `cmake`

    cmake -DLIBAKRYPT_HASH=ON -DLIBAKRYPT_SIGN=ON -DLIBAKRYPT_TESTS=ON ../libakrypt-mini

При установленной программе [Doxygen](http://www.doxygen.nl/) возможна сборка документации с помощью
вызова команды

    make doc

