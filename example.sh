#!/bin/bash
gcc -DLIBAKRYPT_LITTLE_ENDIAN=ON -fomit-frame-pointer -Isource source/ak_streebog.c source/ak_kuznechik.c source/ak_hmac.c source/ak_kdf.c test/test_keys.c -o keys -Wl,--strip-all
