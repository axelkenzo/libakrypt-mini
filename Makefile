FLAGS =  -Wall --pedantic-errors -O2  -funroll-loops

all:
	@echo "use \"make intel\" or \"make mips\""

intel:
	gcc -DLIBAKRYPT_LITTLE_ENDIAN=ON -I./source source/ak_tools.c source/ak_streebog.c test/test_hash.c $(FLAGS) -o hash
	gcc -DLIBAKRYPT_LITTLE_ENDIAN=ON -I./source source/ak_tools.c source/ak_streebog.c test/test_hspeed.c $(FLAGS) -o hspeed
	gcc -DLIBAKRYPT_LITTLE_ENDIAN=ON -I./source source/ak_*.c test/test_sign.c $(FLAGS) -o sign
	gcc -DLIBAKRYPT_LITTLE_ENDIAN=ON -I./source source/ak_tools.c source/ak_streebog.c source/ak_hmac.c test/test_pbkdf2.c $(FLAGS) -o pbkdf2

mips:
	mips64-linux-gnuabi64-gcc -I./source source/ak_tools.c source/ak_streebog.c test/test_hash.c $(FLAGS) -o hash
	mips64-linux-gnuabi64-gcc -I./source source/ak_*.c test/test_sign.c $(FLAGS) -o sign
	mips64-linux-gnuabi64-gcc -I./source source/ak_tools.c source/ak_streebog.c source/ak_hmac.c test/test_pbkdf2.c $(FLAGS) -o pbkdf2

clean:
	rm -f ./hash ./sign ./pbkdf2 ./hspeed ./keys
