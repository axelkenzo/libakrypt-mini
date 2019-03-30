all:
	@echo "use \"make intel\" or \"make mips\""

intel:
	gcc -DLIBAKRYPT_LITTLE_ENDIAN=ON -I./source source/ak_streebog.c test/test_hash.c -Os -Wall -o test_hash
	gcc -DLIBAKRYPT_LITTLE_ENDIAN=ON -I./source source/ak_*.c test/test_sign.c -Os -Wall -o test_sign

mips:
	ecc -target mips32r2-linux -I./source source/ak_streebog.c test/test_hash.c -Os -Wall -o test_hash
	ecc -target mips32r2-linux -I./source source/ak_*.c test/test_sign.c -Os -Wall -o test_sign

clean:
	rm ./test_hash ./test_sign
