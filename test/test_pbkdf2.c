 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <libakrypt.h>

 int main( void )
{
  int i, j, error = ak_error_ok;
  ak_uint8 password[11] = "lababmba31e";
  ak_uint8 salt[10] = "1234567890";
  ak_uint8 out[64];

 /* встроенный тест */
 if( !ak_hmac_test_pbkdf2()) printf("Testing Ok\n"); else printf("Testing wrong\n");

 /* вычисляем значение функции, зависящее от пароля и соли */
 if(( error = ak_hmac_context_pbkdf2_streebog512(
       password,                  /* пароль */
       sizeof( password ),  /* длина пароля */
       salt,      /* открытые данные (соль) */
       sizeof( salt ),        /* длина соли */
       2000,      /* число итераций в цикле */
       64, /* сколько байт нужно выработать */
       out      /* куда поместить результат */
    )) != ak_error_ok ) printf("PBKDF2 wrong\n"); else printf("PBKDF2 Ok\n");

  printf("result: \n");
  for( j = 0; j < 4; j++ ) {
    for( i = 0; i < 16; i++ ) printf(" %02x", out[16*j + i] );
    printf("\n");
  }

 return EXIT_SUCCESS;
}
