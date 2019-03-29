all:

intel:
	gcc -DLIBAKRYPT_LITTLE_ENDIAN=ON -I./source source/ak_streebog.c test/test_hash.c -Os -Wall -o test_hash

mips:
	ecc -target mips32r2-linux -I./source source/ak_streebog.c test/test_hash.c -Os -Wall -o test_hash

clean:
	rm ./test_hash
