 #include <stdio.h>
 #include <stdlib.h>
 #include <libakrypt.h>

 int main( void )
{
 int error = ak_error_ok;

 /* встроенный тест */
 if(( error = ak_wcurve_test( )) == ak_error_ok ) printf("elliptic curves testing Ok\n");
   else printf("elliptic curves testing wrong\n");

 return EXIT_SUCCESS;
}
