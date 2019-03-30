 #include <stdio.h>
 #include <stdlib.h>
 #include <libakrypt.h>

 int main( void )
{
  int error = ak_error_ok;
  struct verifykey vkey; /* открытый ключ подписи */

 /* константное значение открытого ключа */
  ak_uint8 openkey[64] = {
   0xE4, 0x72, 0xDF, 0xD0, 0x09, 0x5B, 0x29, 0x32, 0xB2, 0x14, 0xF8, 0xDF, 0x8B, 0xF4, 0xFF, 0x64,
   0xEE, 0x0B, 0x04, 0xE9, 0x18, 0xD2, 0xF3, 0x54, 0xC1, 0x84, 0xDC, 0xB0, 0x1A, 0xC2, 0x21, 0xFD,
   0x49, 0x50, 0xE4, 0x58, 0x14, 0x1D, 0x04, 0x86, 0xDF, 0x62, 0x58, 0xA9, 0x82, 0x9A, 0x23, 0x59,
   0x06, 0x7E, 0x30, 0xA2, 0xD7, 0x01, 0x2A, 0x1A, 0x06, 0x07, 0x4F, 0xAC, 0xC9, 0xDE, 0x26, 0x50 };

 /* константное значение электронной подписи */
  ak_uint8 sign[64] = {
   0x5C, 0x91, 0x3F, 0xE8, 0x6E, 0xB6, 0xE9, 0x73, 0x63, 0xB6, 0x82, 0x2C, 0x0D, 0xB4, 0xF7, 0xC4,
   0x48, 0xE8, 0x78, 0x35, 0x06, 0x34, 0x97, 0x75, 0x60, 0xE6, 0xC8, 0x17, 0x7B, 0xA0, 0xF5, 0x30,
   0x5E, 0x8F, 0xB1, 0xCD, 0xA1, 0x09, 0xC5, 0xC8, 0x78, 0x37, 0x6C, 0x43, 0x93, 0x7F, 0x7D, 0xF3,
   0x61, 0x56, 0x86, 0xCC, 0xE9, 0xC9, 0xAE, 0x3E, 0xA1, 0x9D, 0xD0, 0x85, 0x2B, 0x73, 0x6E, 0x3F };

 /* встроенный тест */
  if(( error = ak_wcurve_test( )) == ak_error_ok ) printf("elliptic curves testing Ok\n");
    else printf("elliptic curves testing wrong\n");

 /* устанавливаем открытый ключ */
  if(( error = ak_verifykey_context_create_from_ptr( &vkey, openkey, sizeof( openkey ),
           (const ak_wcurve) &id_rfc4357_gost_3410_2001_paramSetA )) != ak_error_ok ) {
    printf("public key creation error\n");
    return EXIT_FAILURE;
  } else printf("creation of public key Ok\n");

 /* теперь проверка выработанной подписи */
  if( ak_verifykey_context_verify_ptr( &vkey, "1234567890", 10, sign )) {
    printf("sign Ok\n");
    return EXIT_SUCCESS;
  }

 return EXIT_FAILURE;
}
