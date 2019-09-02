/* Пример, иллюстрирующий скорость хеширования памяти.

   test-hspeed.c
*/

 #include <time.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <libakrypt-mini.h>

#ifndef _WIN32
 #include <unistd.h>
#endif
#ifdef __linux__
 #include <sys/resource.h>
#endif

 int main( void )
{
  clock_t timea;
  int i, error;
  struct hash ctx;
  ak_uint8 *data, out[64];
  size_t size = 0;
  double iter = 0, avg = 0;
#ifdef __linux__
  ak_int64 msec;
  struct rusage rg1, rg2;
#endif

 /* встроенный тест */
  if( ak_hash_test_streebog256( )) printf("Testing streebog256 Ok\n");
    else printf("Testing streebog256 wrong\n");
  if( ak_hash_test_streebog512( )) printf("Testing streebog512 Ok\n\n");
    else printf("Testing streebog512 wrong\n\n");

 /* статический объект существует, но он требует инициализации */
  if(( error = ak_hash_context_create_streebog512( &ctx )) != ak_error_ok ) {
    printf("incorrect initialization of hash context" );
    return EXIT_FAILURE;
  }

  for( i = 16; i < 129; i += 16 ) {
    data = malloc( size = ( size_t ) i*1024*1024 );
    memset( data, 0x13, size );

   /* теперь собственно хеширование памяти */
    timea = clock();
#ifdef __linux__
    getrusage( RUSAGE_SELF, &rg1 );
#endif
    ak_hash_context_ptr( &ctx, data, size, out );
#ifdef __linux__
    getrusage( RUSAGE_SELF, &rg2 );
#endif
    timea = clock() - timea;
    printf(" %3uMB: hash time: %fs, per 1MB = %fs, speed = %f MBs (clock)\n", (unsigned int)i,
               (double) timea / (double) CLOCKS_PER_SEC,
               (double) timea / ( (double) CLOCKS_PER_SEC*i ),
               (double) CLOCKS_PER_SEC*i / (double) timea );
   /* снимаем погрешность с первого запуска */
    if( i > 16 ) {
      iter += 1;
      avg += (double) CLOCKS_PER_SEC*i / (double) timea;
    }
#ifdef __linux__
    msec = rg2.ru_utime.tv_sec - rg1.ru_utime.tv_sec;
    if( rg2.ru_utime.tv_usec > rg1.ru_utime.tv_usec ) {
      msec *= 1000000;
      msec += (rg2.ru_utime.tv_usec - rg1.ru_utime.tv_usec);
    } else {
          msec *= 1000000;
          msec -= (rg1.ru_utime.tv_usec - rg2.ru_utime.tv_usec);
    }
    printf(" %3uMB: hash time: %fs, per 1MB = %fs, speed = %f MBs (getrusage)\n\n", (unsigned int)i,
               msec / (double) 1000000,
               (double) timea / ( (double) 1000000*i ),
               (double) 1000000*i / (double) timea );
#endif
    free( data );
  }

  printf("average memory hashing speed: %fs\n", avg/iter );
  ak_hash_context_destroy( &ctx );
 return EXIT_SUCCESS;
}
