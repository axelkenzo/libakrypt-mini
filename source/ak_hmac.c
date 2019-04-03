/* ----------------------------------------------------------------------------------------------- */
/*  Copyright (c) 2014 - 2019 by Axel Kenzo, axelkenzo@mail.ru                                     */
/*                                                                                                 */
/*  Файл ak_hmac.с                                                                                 */
/*  - содержит реализацию семейства ключевых алгоритмов хеширования HMAC.                          */
/* ----------------------------------------------------------------------------------------------- */
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <libakrypt.h>

/* ----------------------------------------------------------------------------------------------- */
/*! @param hctx Контекст алгоритма HMAC выработки имитовставки.
    @return В случае успешного завершения функций возвращает \ref ak_error_ok. В случае
    возникновения ошибки возвращеется ее код.                                                      */
/* ----------------------------------------------------------------------------------------------- */
 int ak_hmac_context_create_streebog512( ak_hmac hctx )
{
  if( hctx == NULL ) return ak_error_null_pointer;
  memset( hctx->key, 0, sizeof( hctx->key ));

 return ak_hash_context_create_streebog512( &hctx->ctx );
}

/* ----------------------------------------------------------------------------------------------- */
/*! @param hctx Контекст алгоритма HMAC выработки имитовставки.
    @return В случае успешного завершения функций возвращает \ref ak_error_ok. В случае
    возникновения ошибки возвращеется ее код.                                                      */
/* ----------------------------------------------------------------------------------------------- */
 int ak_hmac_context_destroy( ak_hmac hctx )
{
  if( hctx == NULL ) return ak_error_null_pointer;
  memset( hctx->key, 0, sizeof( hctx->key ));

 return ak_hash_context_destroy( &hctx->ctx );
}

/* ----------------------------------------------------------------------------------------------- */
/*! @param hctx Контекст алгоритма HMAC выработки имитовставки.
    К моменту вызова функции контекст должен быть инициализирован.
    @param ptr Указатель на данные, которые будут интерпретироваться в качестве значения ключа.
    @param size Размер данных, на которые указывает `ptr` (размер в байтах).
    Если величина `size` меньше, чем размер выделенной памяти под секретный ключ, то копируется
    только `size` байт (остальные заполняются нулями). Если `size` больше, чем количество выделенной памяти
    под ключ, то данные предварительно хешируются.

    @return В случае успеха возвращается значение \ref ak_error_ok. В противном случае
    возвращается код ошибки.                                                                       */
/* ----------------------------------------------------------------------------------------------- */
 int ak_hmac_context_set_key( ak_hmac hctx, const ak_pointer ptr, const size_t size )
{
  if( hctx == NULL ) return ak_error_null_pointer;
 /* вспоминаем, что если ключ длиннее, чем длина входного блока хэш-функции, то в качестве
                                                                      ключа используется его хэш */
  if( size > hctx->ctx.bsize ) {

    if( hctx->ctx.hsize > 64 ) return ak_error_wrong_length;
    return ak_hash_context_ptr( &hctx->ctx, ptr, size, hctx->key );

  }
  memset( hctx->key, 0, sizeof( hctx->key ));  memcpy( hctx->key, ptr, size );
 return ak_error_ok;
}

/* ----------------------------------------------------------------------------------------------- */
/*! @param ctx Контекст алгоритма HMAC выработки имитовставки.
    @return В случае успеха функция возвращает \ref ak_error_ok. В противном случае
    возвращается код ошибки.                                                                       */
/* ----------------------------------------------------------------------------------------------- */
 static int ak_hmac_context_clean( ak_hmac hctx )
{
  int error = ak_error_ok;
  size_t idx = 0, len = 0;
  ak_uint8 buffer[64]; /* буффер для хранения промежуточных значений */

  if( hctx == NULL ) return ak_error_null_pointer;

 /* проверяем наличие ключа и его ресурс */
  if( hctx->ctx.bsize > sizeof( buffer )) return ak_error_wrong_length;

 /* фомируем маскированное значение ключа */
  len = ak_min( hctx->ctx.bsize, 64 );
  for( idx = 0; idx < len; idx++ ) buffer[idx] = hctx->key[idx] ^ 0x36;
  for( ; idx < hctx->ctx.bsize; idx++ ) buffer[idx] = 0x36;

 /* инициализируем начальное состояние контекста хеширования */
  if(( error = hctx->ctx.clean( &hctx->ctx )) != ak_error_ok ) return error;

 /* обновляем состояние контекста хеширования */
  error = hctx->ctx.update( &hctx->ctx, buffer, hctx->ctx.bsize );

 /* очищаем буффер */
  memset( buffer, 0, sizeof( buffer ));

 return error;
}

/* ----------------------------------------------------------------------------------------------- */
/*! @param ctx Контекст алгоритма HMAC выработки имитовставки.
    @param data Указатель на обрабатываемые данные.
    @param size Длина обрабатываемых данных (в байтах); длина должна быть кратна длине блока
    обрабатываемых данных используемой функции хеширования
    @return В случае успеха функция возвращает \ref ak_error_ok. В противном случае
    возвращается код ошибки.                                                                       */
/* ----------------------------------------------------------------------------------------------- */
 int ak_hmac_context_update( ak_hmac hctx, const ak_pointer data, const size_t size )
{
  if( hctx == NULL ) return ak_error_null_pointer;
  if( !size ) return ak_error_zero_length;
  if( size%hctx->ctx.bsize ) return ak_error_wrong_length;

  return hctx->ctx.update( &hctx->ctx, data, size );
}

/* ----------------------------------------------------------------------------------------------- */
/*! @param ctx Контекст алгоритма HMAC выработки имитовставки.
    @param data блок входных данных; длина блока должна быть менее, чем блина блока
           обрабатываемых данных для используемой функции хеширования
    @param size длина блока обрабатываемых данных
    @param out указатель на область памяти, куда будет помещен результат; если out равен NULL,
           то создается новый буффер, в который помещается результат.
    @return если out равен NULL, то возвращается указатель на созданный буффер. В противном случае
           возвращается NULL. В случае возникновения ошибки возывращается NULL. Код ошибки может
           быть получен c помощью вызова функции ak_error_get_value().                             */
/* ----------------------------------------------------------------------------------------------- */
 int ak_hmac_context_finalize( ak_hmac hctx, const ak_pointer data,
                                                               const size_t size, ak_pointer out )
{
  int error = ak_error_ok;
  size_t idx = 0, len = 0;
  ak_uint8 temporary[128]; /* первый буффер для хранения промежуточных значений */
  ak_uint8 keybuffer[128]; /* второй буффер для хранения промежуточных значений */

 /* выполняем проверки */
  if( hctx == NULL ) return ak_error_null_pointer;

 /* ограничение в связи с константным размером временного буффера */
  if( hctx->ctx.hsize > sizeof( temporary )) return ak_error_wrong_length;
  if( size >= hctx->ctx.bsize ) return ak_error_zero_length;

 /* обрабатываем хвост предыдущих данных */
  memset( temporary, 0, sizeof( temporary ));
  error = ak_error_ok;
  if(( error =  hctx->ctx.finalize( &hctx->ctx, data, size, temporary )) != ak_error_ok ) return error;

 /* фомируем маскированное значение ключа */
  len = ak_min( hctx->ctx.bsize, 64 );
  for( idx = 0; idx < len; idx++ ) keybuffer[idx] = hctx->key[idx] ^ 0x5C;
  for( ; idx < hctx->ctx.bsize; idx++ ) keybuffer[idx] = 0x5C;

 /* возвращаем контекст хеширования в начальное состояние */
  if(( error = hctx->ctx.clean( &hctx->ctx )) != ak_error_ok ) return error;

 /* обновляем состояние контекста хеширования */
  if(( error = hctx->ctx.update( &hctx->ctx, keybuffer, hctx->ctx.bsize )) != ak_error_ok )
    return error;

 /* очищаем буффер */
  memset( keybuffer, 0, sizeof( keybuffer ));

 /* последний update/finalize и возврат результата */
  if( hctx->ctx.bsize == hctx->ctx.hsize ) {
    hctx->ctx.update( &hctx->ctx, temporary, hctx->ctx.hsize );
    error = hctx->ctx.finalize( &hctx->ctx, NULL, 0, out );
  } else error = hctx->ctx.finalize( &hctx->ctx, temporary, hctx->ctx.hsize, out );

 /* очищаем контекст функции хеширования, ключ не трогаем */
  hctx->ctx.clean( &hctx->ctx );

 return error;
}

/* ----------------------------------------------------------------------------------------------- */
/*! Функция вычисляет имитовставку от заданной области памяти на которую указывает in. Размер памяти
    задается в байтах в переменной size. Результат вычислений помещается в область памяти,
    на которую указывает out. Если out равен NULL, то функция создает новый буффер
    (структуру struct buffer), помещает в нее вычисленное значение и возвращает на указатель на
    буффер. Буффер должен позднее быть удален с помощью вызова ak_buffer_delete().

    \b Внимание. После завершения вычислений контекст функции хеширования инициализируется
    в начальное состояние.

    @param hctx Контекст алгоритма HMAC выработки имитовставки.
    @param in Указатель на входные данные для которых вычисляется хеш-код.
    @param size Размер входных данных в байтах.
    @param out Область памяти, куда будет помещен результат. Память должна быть заранее выделена.
    Размер выделяемой памяти может быть определен с помощью вызова ak_hash_get_code_size().
    Указатель out может принимать значение NULL.

    @return Функция возвращает NULL, если указатель out не есть NULL, в противном случае
    возвращается указатель на буффер, содержащий результат вычислений. В случае возникновения
    ошибки возвращается NULL, при этом код ошибки может быть получен с помощью вызова функции
    ak_error_get_value().                                                                          */
/* ----------------------------------------------------------------------------------------------- */
 int ak_hmac_context_ptr( ak_hmac hctx, const ak_pointer in, const size_t size, ak_pointer out )
{
  int error = ak_error_ok;
  size_t quot = 0, offset = 0;

  if( hctx == NULL ) return ak_error_null_pointer;
  if( in == NULL ) return ak_error_null_pointer;

  /* вычищаем результаты предыдущих вычислений */
  if(( error = ak_hmac_context_clean( hctx )) != ak_error_ok ) return error;
  quot = size/hctx->ctx.bsize;
  offset = quot*hctx->ctx.bsize;

  /* вызываем, если длина сообщения не менее одного полного блока */
  if( quot > 0 )
    if(( error = ak_hmac_context_update( hctx, in, offset )) != ak_error_ok ) return error;

  /* обрабатываем хвост и возвращаем результат */
 return ak_hmac_context_finalize( hctx, (ak_uint8 *)in + offset, size - offset, out );
}

/* ----------------------------------------------------------------------------------------------- */
/*! Функция вырабатывает ключевой вектор из заданного пользователем пароля и инициализационного
    вектора в соответствии с алгоритмом, описанным в отечественных рекомендациях Р 50.1.111-2016.
    При выработке используется алгоритм hmac-streebog512.

    Пароль должен представлять собой ненулевую строку символов в utf8
    кодировке. Размер вырабатываемого ключевого вектора может колебаться от 32-х до 64-х байт.

    @param pass Пароль, строка символов в utf8 кодировке.
    @param pass_size Размер пароля в байтах, должен быть отличен от нуля.
    @param salt Строка с инициализационным вектором (произвольная область памяти). Данное значение
    не является секретным и может храниться или передаваться в открытом виде.
    @param salt_size Размер инициализионного вектора в байтах. Не должен превышать 60 байт.
    @param cnt Параметр, определяющий количество однотипных итераций для выработки ключа; данный
    параметр определяет время работы алгоритма; параметр не является секретным и может храниться или
    передаваться в открытом виде.
    @param dklen Длина вырабатываемого ключевого вектора в байтах, величина должна принимать
    значение от 32-х до 64-х.
    @param out Указатель на массив, куда будет помещен результат; под данный массив должна быть
    заранее выделена память не менее, чем dklen байт.

    @return В случае успеха функция возвращает \ref ak_error_ok. В противном случае
    возвращается код ошибки.                                                                       */
/* ----------------------------------------------------------------------------------------------- */
 int ak_hmac_context_pbkdf2_streebog512( const ak_pointer pass,
         const size_t pass_size, const ak_pointer salt, const size_t salt_size, const size_t cnt,
                                                               const size_t dklen, ak_pointer out )
{
  struct hmac hctx;
  ak_uint8 result[64], buffer[64];
  int error = ak_error_ok;
  size_t idx = 0, jdx = 0;

 /* в начале, многочисленные проверки входных параметров */
  if( pass == NULL ) return ak_error_null_pointer;
  if( !pass_size ) return ak_error_wrong_length;
  if( salt == NULL ) return ak_error_null_pointer;
  if(( dklen < 32 ) || ( dklen > 64 )) return ak_error_wrong_length;
  if( out == NULL ) return ak_error_null_pointer;

 /* создаем контекст алгоритма hmac и определяем его ключ */
  if(( error = ak_hmac_context_create_streebog512( &hctx )) != ak_error_ok ) return error;
  if(( error = ak_hmac_context_set_key( &hctx, pass, pass_size )) != ak_error_ok ) return error;

 /* начальная инициализация промежуточного вектора */
  memset( result, 0, 64 );
  result[3] = 1;

 /* вычисляем значение первой строки U1  */
  if( salt_size > 60 ) return ak_error_wrong_length;
  memset( buffer, 0, sizeof( buffer ));
  memcpy( buffer, salt, salt_size );
  memcpy( buffer+salt_size, result, 4 );
  if(( error = ak_hmac_context_ptr( &hctx, buffer, salt_size + 4, result )) != ak_error_ok ) return error;
  memcpy( out, result+64-dklen, dklen );

 /* теперь основной цикл по значению аргумента c */
  for( idx = 1; idx < cnt; idx++ ) {
     ak_hmac_context_ptr( &hctx, result, 64, result );
     for( jdx = 0; jdx < dklen; jdx++ ) ((ak_uint8 *)out)[jdx] ^= result[64-dklen+jdx];
  }
  memset( result, 0, 64 );
  ak_hmac_context_destroy( &hctx );

 return error;
}


/* ----------------------------------------------------------------------------------------------- */
/*                            функции для тестирования алгоритма hmac                              */
/* ----------------------------------------------------------------------------------------------- */
 int ak_hmac_test_pbkdf2( void )
{
  ak_uint8 key[32] = {
   0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
   0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
  };

  ak_uint8 data[16] = {
   0x01, 0x26, 0xbd, 0xb8, 0x78, 0x00, 0xaf, 0x21, 0x43, 0x41, 0x45, 0x65, 0x63, 0x78, 0x01, 0x00
  };

  ak_uint8 R512[64] = {
   0xa5, 0x9b, 0xab, 0x22, 0xec, 0xae, 0x19, 0xc6, 0x5f, 0xbd, 0xe6, 0xe5, 0xf4, 0xe9, 0xf5, 0xd8,
   0x54, 0x9d, 0x31, 0xf0, 0x37, 0xf9, 0xdf, 0x9b, 0x90, 0x55, 0x00, 0xe1, 0x71, 0x92, 0x3a, 0x77,
   0x3d, 0x5f, 0x15, 0x30, 0xf2, 0xed, 0x7e, 0x96, 0x4c, 0xb2, 0xee, 0xdc, 0x29, 0xe9, 0xad, 0x2f,
   0x3a, 0xfe, 0x93, 0xb2, 0x81, 0x4f, 0x79, 0xf5, 0x00, 0x0f, 0xfc, 0x03, 0x66, 0xc2, 0x51, 0xe6
  };

  ak_uint8 R1[64] = {
   0x64, 0x77, 0x0a, 0xf7, 0xf7, 0x48, 0xc3, 0xb1, 0xc9, 0xac, 0x83, 0x1d, 0xbc, 0xfd, 0x85, 0xc2,
   0x61, 0x11, 0xb3, 0x0a, 0x8a, 0x65, 0x7d, 0xdc, 0x30, 0x56, 0xb8, 0x0c, 0xa7, 0x3e, 0x04, 0x0d,
   0x28, 0x54, 0xfd, 0x36, 0x81, 0x1f, 0x6d, 0x82, 0x5c, 0xc4, 0xab, 0x66, 0xec, 0x0a, 0x68, 0xa4,
   0x90, 0xa9, 0xe5, 0xcf, 0x51, 0x56, 0xb3, 0xa2, 0xb7, 0xee, 0xcd, 0xdb, 0xf9, 0xa1, 0x6b, 0x47
  };

  ak_uint8 R2[64] = {
   0x5a, 0x58, 0x5b, 0xaf, 0xdf, 0xbb, 0x6e, 0x88, 0x30, 0xd6, 0xd6, 0x8a, 0xa3, 0xb4, 0x3a, 0xc0,
   0x0d, 0x2e, 0x4a, 0xeb, 0xce, 0x01, 0xc9, 0xb3, 0x1c, 0x2c, 0xae, 0xd5, 0x6f, 0x02, 0x36, 0xd4,
   0xd3, 0x4b, 0x2b, 0x8f, 0xbd, 0x2c, 0x4e, 0x89, 0xd5, 0x4d, 0x46, 0xf5, 0x0e, 0x47, 0xd4, 0x5b,
   0xba, 0xc3, 0x01, 0x57, 0x17, 0x43, 0x11, 0x9e, 0x8d, 0x3c, 0x42, 0xba, 0x66, 0xd3, 0x48, 0xde
  };

  ak_uint8 R3[64] = {
   0xe5, 0x2d, 0xeb, 0x9a, 0x2d, 0x2a, 0xaf, 0xf4, 0xe2, 0xac, 0x9d, 0x47, 0xa4, 0x1f, 0x34, 0xc2,
   0x03, 0x76, 0x59, 0x1c, 0x67, 0x80, 0x7f, 0x04, 0x77, 0xe3, 0x25, 0x49, 0xdc, 0x34, 0x1b, 0xc7,
   0x86, 0x7c, 0x09, 0x84, 0x1b, 0x6d, 0x58, 0xe2, 0x9d, 0x03, 0x47, 0xc9, 0x96, 0x30, 0x1d, 0x55,
   0xdf, 0x0d, 0x34, 0xe4, 0x7c, 0xf6, 0x8f, 0x4e, 0x3c, 0x2c, 0xda, 0xf1, 0xd9, 0xab, 0x86, 0xc3
  };

  ak_uint8 R4[64] = {
   0x50, 0xdf, 0x06, 0x28, 0x85, 0xb6, 0x98, 0x01, 0xa3, 0xc1, 0x02, 0x48, 0xeb, 0x0a, 0x27, 0xab,
   0x6e, 0x52, 0x2f, 0xfe, 0xb2, 0x0c, 0x99, 0x1c, 0x66, 0x0f, 0x00, 0x14, 0x75, 0xd7, 0x3a, 0x4e,
   0x16, 0x7f, 0x78, 0x2c, 0x18, 0xe9, 0x7e, 0x92, 0x97, 0x6d, 0x9c, 0x1d, 0x97, 0x08, 0x31, 0xea,
   0x78, 0xcc, 0xb8, 0x79, 0xf6, 0x70, 0x68, 0xcd, 0xac, 0x19, 0x10, 0x74, 0x08, 0x44, 0xe8, 0x30
  };

  struct hmac hkey;
  ak_uint8 out[64];
  int i, error = ak_error_ok;
  ak_uint8 password_one[8] = "password",
           password_two[9] = { 'p', 'a', 's', 's', 0, 'w', 'o', 'r', 'd' },
           salt_one[4]     = "salt",
           salt_two[5]     = { 's', 'a', 0, 'l', 't' };

 /* 1. тестируем HMAC на основе Стрибог 512 */
  if(( error = ak_hmac_context_create_streebog512( &hkey )) != ak_error_ok ) return error;
  if(( error = ak_hmac_context_set_key( &hkey, key, 32 )) != ak_error_ok ) return error;

  memset( out, 0, 64 );
  if(( error = ak_hmac_context_ptr( &hkey, data, 16, out )) != ak_error_ok ) return error;

  for( i = 0; i < 64; i++ )
     if( R512[i] != out[i] ) error = ak_error_not_equal_data;
  if( error != ak_error_ok ) return error;

  ak_hmac_context_destroy( &hkey );

 /* 2. тестируем собственно pbkdf2 */
 /* первый тест из Р 50.1.111-2016 */

  if(( error = ak_hmac_context_pbkdf2_streebog512( password_one, 8,
                                          salt_one, 4, 1, 64, out )) != ak_error_ok ) return error;
  for( i = 0; i < 64; i++ )
     if( R1[i] != out[i] ) error = ak_error_not_equal_data;
  if( error != ak_error_ok ) return error;

 /* второй тест из Р 50.1.111-2016 */
  if(( error = ak_hmac_context_pbkdf2_streebog512( password_one, 8,
                                           salt_one, 4, 2, 64, out )) != ak_error_ok ) return error;
  for( i = 0; i < 64; i++ )
     if( R2[i] != out[i] ) error = ak_error_not_equal_data;
  if( error != ak_error_ok ) return error;

 /* третий тест из Р 50.1.111-2016 */
  if(( error = ak_hmac_context_pbkdf2_streebog512( password_one, 8,
                                        salt_one, 4, 4096, 64, out )) != ak_error_ok ) return error;

  for( i = 0; i < 64; i++ )
     if( R3[i] != out[i] ) error = ak_error_not_equal_data;
  if( error != ak_error_ok ) return error;

 /* четвертый тест из Р 50.1.111-2016 */
  if(( error = ak_hmac_context_pbkdf2_streebog512( password_two, 9,
                                       salt_two, 5, 4096, 64, out )) != ak_error_ok ) return error;
  for( i = 0; i < 64; i++ )
     if( R4[i] != out[i] ) error = ak_error_not_equal_data;

 return error;
}

/* ----------------------------------------------------------------------------------------------- */
/*                                                                                       ak_hmac.с */
/* ----------------------------------------------------------------------------------------------- */
