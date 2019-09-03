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
  int i, j, k, error = ak_error_ok;

 /* тестовый ключ из ГОСТ Р 34.12-2015 */
  ak_uint8 testkey[32] = {
    0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01, 0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe,
    0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00, 0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88 };

 /* номер серии ключа */
  ak_uint8 key_document_number[8] = { 0x00, 0x00, 0xaa, 0xaa, 0xbb, 0xbb, 0x19, 0x99 };

 /* указатель на производный ключ */
  ak_uint8 *derived_key = NULL;

 /* контекст блочного шифра Кузнечик */
  struct bckey bctx;

 /* 1. Создаем контекст выработки производных ключей */
  if(( error = ak_nkey_context_create(
                   &nctx,                  /* контекст выработки производных ключей */
                   testkey,                         /* исходная ключевая информация */
                   sizeof( testkey ),       /*  размер исходной ключевой информации */
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

     /* получаем следующий производный ключ */
      derived_key = ak_nkey_context_generate_next_key( &nctx );

     /* выводим информацию  */
      printf("key [%3d]: ", ak_nkey_context_get_key_number( &nctx ));
        for( j = 0; j < 32; j++ ) printf("%02x ", derived_key[j] );
      printf("\n");

     /* 3. Разворачиваем ключ дальше и вычисляем раундовые ключи,
           которые будут передаваться дальше */
      ak_bckey_context_create( &bctx, derived_key );

     /* выводим информацию (в обратном порядке байт, как в ГОСТе) */
        printf("  round keys:\n");
        for( j = 0; j < 10; j++ ) {
          printf("  K[%2d]: [%016llx:%016llx] -> ", j+1, bctx.direct[2*j+1], bctx.direct[2*j]);
          for( k = 15; k >= 0; k-- ) printf("%02x ", ((ak_uint8 *)bctx.direct)[16*j + k] );
          printf("\n");
        }
        printf("\n");
   }
  timea = clock() - timea;
  seconds = (double) timea / (double) CLOCKS_PER_SEC;
  printf("generation of %u keys: %fs (%f keys per second)\n",
                                total_key_count, seconds, total_key_count/seconds );
 return EXIT_SUCCESS;
}
