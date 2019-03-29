all:

intel:
	gcc -DLIBAKRYPT_LITTLE_ENDIAN=ON -I./ ak_streebog.c test_hash.c -Os -Wall -o test_hash

mips:
	ecc -target mips32r2-linux -I./ ak_streebog.c test_hash.c -Os -Wall -o test_hash

clean:
	rm ./test_hash
