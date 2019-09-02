 #include <time.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <libakrypt-mini.h>

 #define total_key_count (1000)

 int main( void )
{
  clock_t timea;
  struct nkey nctx;
  double seconds = 0;
  int i, j, error = ak_error_ok;

 /* исходный ключ, 32 октета */
  ak_uint8 source_key[32] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf };

 /* номер серии ключа */
  ak_uint8 key_document_number[8] = { 0x00, 0x00, 0xaa, 0xaa, 0xbb, 0xbb, 0x19, 0x99 };

 /* указатель на производный ключ */
  ak_uint8 *derived_key = NULL;

 /* 1. Создаем контекст выработки производных ключей */
  if(( error = ak_nkey_context_create(
                   &nctx,                  /* контекст выработки производных ключей */
                   source_key,                      /* исходная ключевая информация */
                   sizeof( source_key ),    /*  размер исходной ключевой информации */
                   key_document_number,                        /* номер серии ключа */
                   sizeof( key_document_number ),         /* размер серии в октетах */
     ( ak_uint8 *) "source-PlatformID",                /* идентификатор отправителя */
                   17,
     ( ak_uint8 *) "destination-PlatformID-identifier", /* идентификатор получателя */
                   33 )
    ) != ak_error_ok ) {
    printf("incorrect creation of temporary key buffer\n");
    return EXIT_FAILURE;
  }

 /* 2. Иллюстрируем процедуру выработки производного ключа
       и тестируем скорость работы (с дополнительной нагрузкой) */
  timea = clock();
   for( i = 0; i < total_key_count; i++ ) {
     derived_key = ak_nkey_context_generate_next_key( &nctx );

     printf("key [%3d]: ", ak_nkey_context_get_key_number( &nctx ));
      for( j = 0; j < 32; j++ ) printf("%02x ", derived_key[j] );
     printf("\n");
   }
  timea = clock() - timea;
  seconds = (double) timea / (double) CLOCKS_PER_SEC;
  printf("generation of %u keys: %fs (one key per %f sec)\n",
                                total_key_count, seconds, total_key_count/seconds );
 return EXIT_SUCCESS;
}
