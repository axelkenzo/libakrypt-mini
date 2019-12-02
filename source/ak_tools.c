/* ----------------------------------------------------------------------------------------------- */
/*  Copyright (c) 2014 - 2019 by Axel Kenzo, axelkenzo@mail.ru                                     */
/*                                                                                                 */
/*  Файл ak_tools.с                                                                                */
/*  - содержит реализацию элементарных функций из libc                                             */
/* ----------------------------------------------------------------------------------------------- */
 #include <libakrypt-mini.h>

/* ----------------------------------------------------------------------------------------------- */
 void *ak_memcpy( void *dest, const void *src, size_t n )
{
  size_t i;
  if( dest != (void *)0 )
    for( i = 0; i < n; i++ ) ((signed char *)dest)[i] = ((const signed char *)src)[i];

 return dest;
}

/* ----------------------------------------------------------------------------------------------- */
 void *ak_memset( void *s, int c, size_t n )
{
  size_t i;
  if( s != (void *)0 )
    for( i = 0; i < n; i++ ) ((signed char *)s)[i] = (signed char)c;

 return s;
}

/* ----------------------------------------------------------------------------------------------- */
 ak_uint64 ak_bswap_64( ak_uint64 n )
{
#if defined(_AMD64_) || defined(__x86_64__)
  unsigned long long retval;
  __asm__ __volatile__ ("bswapq %[retval]" : [retval] "=rm" (retval) : "[retval]" (n));
  return retval;
#elif defined(_X86_) || defined(__i386__)
  union {
    long long int64part;
    struct {
      unsigned long lowpart;
      unsigned long hipart;
    } parts;
  } retval;
  retval.int64part = n;
  __asm__ __volatile__ ("bswapl %[lowpart]\n"
    "bswapl %[hipart]\n"
    : [lowpart] "=rm" (retval.parts.hipart), [hipart] "=rm" (retval.parts.lowpart)  : "[lowpart]" (retval.parts.lowpart), "[hipart]" (retval.parts.hipart));
  return retval.int64part;
#else
  unsigned char *b = (unsigned char *)&n;
  unsigned char tmp;
  tmp = b[0];
  b[0] = b[7];
  b[7] = tmp;
  tmp = b[1];
  b[1] = b[6];
  b[6] = tmp;
  tmp = b[2];
  b[2] = b[5];
  b[5] = tmp;
  tmp = b[3];
  b[3] = b[4];
  b[4] = tmp;
  return n;
#endif
}

/* ----------------------------------------------------------------------------------------------- */
