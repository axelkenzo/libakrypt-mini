FLAGS =  -Wall --pedantic-errors -Os

all:
	@echo "use \"make intel\" or \"make mips\""

intel:
	gcc -DLIBAKRYPT_LITTLE_ENDIAN=ON -I./source source/ak_streebog.c test/test_hash.c $(FLAGS) -o test_hash
	gcc -DLIBAKRYPT_LITTLE_ENDIAN=ON -I./source source/ak_*.c test/test_sign.c $(FLAGS) -o test_sign
	gcc -DLIBAKRYPT_LITTLE_ENDIAN=ON -I./source source/ak_streebog.c source/ak_hmac.c test/test_pbkdf2.c $(FLAGS) -o test_pbkdf2
	strip -s test_*

mips:
	ecc -target mips32r2-linux -I./source source/ak_streebog.c test/test_hash.c $(FLAGS) -o test_hash
	ecc -target mips32r2-linux -I./source source/ak_*.c test/test_sign.c $(FLAGS) -o test_sign
	ecc -target mips32r2-linux -I./source source/ak_streebog.c source/ak_hmac.c test/test_pbkdf2.c $(FLAGS) -o test_pbkdf2

clean:
	rm ./test_hash ./test_sign ./test_pbkdf2
