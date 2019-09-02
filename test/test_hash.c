 #include <stdio.h>
 #include <stdlib.h>
 #include <libakrypt-mini.h>

 int main( void )
{
  struct hash ctx;
  int i = 0, res = EXIT_SUCCESS;
  ak_uint8 out[32], sout[32] = {
   0xC6, 0x00, 0xFD, 0x9D, 0xD0, 0x49, 0xCF, 0x8A, 0xBD, 0x2F, 0x5B, 0x32, 0xE8, 0x40, 0xD2, 0xCB,
   0x0E, 0x41, 0xEA, 0x44, 0xDE, 0x1C, 0x15, 0x5D, 0xCD, 0x88, 0xDC, 0x84, 0xFE, 0x58, 0xA8, 0x55 };

 /* встроенный тест */
 if( ak_hash_test_streebog256( )) printf("Testing streebog256 Ok\n");
   else printf("Testing streebog256 wrong\n");
 if( ak_hash_test_streebog512( )) printf("Testing streebog512 Ok\n");
   else printf("Testing streebog512 wrong\n");

 /* тестовые значения из libcrypto */
  ak_uint8 HASH_SHORT_MESSAGE[63] = {
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
        0x30, 0x31, 0x32
  };
  ak_uint8 SHORT_MESSAGE_HASH256[32] = {
        0x9d, 0x15, 0x1e, 0xef, 0xd8, 0x59, 0x0b, 0x89,
        0xda, 0xa6, 0xba, 0x6c, 0xb7, 0x4a, 0xf9, 0x27,
        0x5d, 0xd0, 0x51, 0x02, 0x6b, 0xb1, 0x49, 0xa4,
        0x52, 0xfd, 0x84, 0xe5, 0xe5, 0x7b, 0x55, 0x00
  };

 /* создаем контекст для алгоритма Стрибог256 */
  ak_hash_context_create_streebog256( &ctx );

 /* вычисляем хэш для фиксированной области памяти,
    результат помещается в out */
  ak_hash_context_ptr( &ctx, HASH_SHORT_MESSAGE, sizeof( HASH_SHORT_MESSAGE ), out );

 /* закрываем контекст алгоритма хеширования */
  ak_hash_context_destroy( &ctx );

 /* проверка результата */
  printf("hash: " );
  for( i = 0; i < 32; i++ ) {
     printf("%02X", out[i] );
     if( out[i] != SHORT_MESSAGE_HASH256[i] ) res = EXIT_FAILURE;
  }
  if( res == EXIT_SUCCESS ) printf(" Ok\n"); else printf(" No\n");

 /* теперь реализация тестового примера из libcrypto */
 /* самая простая выработка хеша из пароля
    должна работать на любой архитектуре  (разделение реализаций в hash.c)  */

  ak_hash_context_create_streebog256( &ctx );
  ak_hash_context_ptr( &ctx, "hello world", 11, out );
  ak_hash_context_destroy( &ctx );

 /* проверка */
    printf("hash: ");
    for( i = 0; i < 32; i++ ) {
       printf("%02X", out[i] );
       if( out[i] != sout[i] ) res = EXIT_FAILURE;
    }
    if( res == EXIT_SUCCESS ) printf(" Ok\n"); else printf(" No\n");
 return res;
}
