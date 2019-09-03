FLAGS =  -Wall --pedantic-errors -Os

all:
	@echo "use \"make intel\" or \"make mips\""

intel:
	gcc -DLIBAKRYPT_LITTLE_ENDIAN=ON -I./source source/ak_streebog.c test/test_hash.c $(FLAGS) -o hash
	gcc -DLIBAKRYPT_LITTLE_ENDIAN=ON -I./source source/ak_*.c test/test_sign.c $(FLAGS) -o sign
	gcc -DLIBAKRYPT_LITTLE_ENDIAN=ON -I./source source/ak_streebog.c source/ak_hmac.c test/test_pbkdf2.c $(FLAGS) -o pbkdf2
	strip -s test_*

mips:
	ecc -target mips32r2-linux -I./source source/ak_streebog.c test/test_hash.c $(FLAGS) -o hash
	ecc -target mips32r2-linux -I./source source/ak_*.c test/test_sign.c $(FLAGS) -o sign
	ecc -target mips32r2-linux -I./source source/ak_streebog.c source/ak_hmac.c test/test_pbkdf2.c $(FLAGS) -o pbkdf2

clean:
	rm -f ./hash ./sign ./pbkdf2 ./hspeed ./keys
