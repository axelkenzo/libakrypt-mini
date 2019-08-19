#!/bin/bash
gcc -DLIBAKRYPT_LITTLE_ENDIAN=ON -fomit-frame-pointer -Isource source/ak_streebog.c test/test_hash.c -o hash -Wl,--strip-all
