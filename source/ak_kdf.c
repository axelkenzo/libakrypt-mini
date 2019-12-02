/* ----------------------------------------------------------------------------------------------- */
/*  Copyright (c) 2014 - 2019 by Axel Kenzo, axelkenzo@mail.ru                                     */
/*                                                                                                 */
/*  Файл ak_kdf.с                                                                                  */
/*  - содержит реализацию функций выработки производных ключей.                                    */
/* ----------------------------------------------------------------------------------------------- */
 #include <libakrypt-mini.h>

/* ----------------------------------------------------------------------------------------------- */
/*! @param nctx Контекст структуры генерации производных ключей.
    @param source Исходная ключевая информация, представляющая собой последовательность октетов
    произвольной длины.
    @param source_size Длина исходной ключевой информации. В текущей реализации допускается
    единственное значение 32 октета (256 бит).
    @param salt Произвольная последовательность октетов. Используется для генерации
    промежуточного ключа. В качестве данной последовательности моuen выступать, например,
    номер ключевой серии, дата выработки производных ключей и т.п.
    @param salt_size Длина последовательности октетов salt. В текущей реализации не должна
    превышать 64-х октетов.
    @param src Идентификатор отправителя информации. Используется при выработки производных
    ключей для определения направления передачи зашифрованной информации.
    @param src_size Размер идентификатора отправителя информации (в октетах).
    @param dst Идентификатор получателя информации. Используется при выработки производных
    ключей для определения направления передачи зашифрованной информации.
    @param dst_size Размер идентификатора получателя информации (в октетах).
    @return В случае успешного завершения функций возвращает \ref ak_error_ok. В случае
    возникновения ошибки возвращеется ее код.                                                      */
/* ----------------------------------------------------------------------------------------------- */
 int ak_nkey_context_create( ak_nkey nctx,
             ak_uint8 *source, const size_t source_size, ak_uint8 *salt, const size_t salt_size,
                                    ak_uint8 *src, size_t src_size, ak_uint8 *dst, size_t dst_size )
{
  int error = ak_error_ok;

  if(( nctx == NULL ) || ( source == NULL ) || ( salt == NULL )) return ak_error_null_pointer;
  if( salt_size == 0 ) return ak_error_zero_length;
  if(( salt_size > 64 ) || ( source_size != 32 )) return ak_error_wrong_length;

 /* формируем контекст и вырабатываем инициализируем ключ */
  if(( error = ak_hmac_context_create_streebog512( &nctx->hctx )) != ak_error_ok ) return error;
  if(( error = ak_hmac_context_set_key( &nctx->hctx, salt, salt_size )) != ak_error_ok ) return error;

 /* вычисляем значение промежуточного ключа */
  ak_memset( nctx->buffer, 0, sizeof( nctx->buffer ));
  if(( error = ak_hmac_context_ptr( &nctx->hctx, source, source_size,
                                                        nctx->buffer )) != ak_error_ok ) return error;

 /* инициализируем контекст вычисленным значением */
  if(( error = ak_hmac_context_create_streebog256( &nctx->hctx )) != ak_error_ok ) return error;
  if(( error = ak_hmac_context_set_key( &nctx->hctx, nctx->buffer, 32 )) != ak_error_ok ) return error;

 /* формируем изменяемый вектор */
  nctx->key_number = -1;
  nctx->length = 36;
  ak_memset( nctx->buffer, 0, sizeof( nctx->buffer ));
  ak_memcpy( nctx->buffer + nctx->length, src, src_size ); nctx->length += src_size;
  ak_memcpy( nctx->buffer + nctx->length, dst, dst_size ); nctx->length += dst_size;
  nctx->buffer[nctx->length++] = 0x80;
  nctx->buffer[nctx->length++] = 0;
  nctx->buffer[nctx->length++] = 0;
  nctx->buffer[nctx->length++] = 0;
  nctx->buffer[nctx->length++] = 0;

 return ak_error_ok;
}

/* ----------------------------------------------------------------------------------------------- */
 ak_uint8 *ak_nkey_context_generate_next_key( ak_nkey nctx )
{
  int error = ak_error_ok;
  if( nctx == NULL ) return NULL;
  if((ak_uint32) ++nctx->key_number >= 2147483648 ) return NULL;

  /* уточняем последнее */
   nctx->buffer[35] =  nctx->key_number&0xFF;
   nctx->buffer[34] = (nctx->key_number >> 8 )&0xFF;
   nctx->buffer[33] = (nctx->key_number >> 16 )&0xFF;
   nctx->buffer[32] = (nctx->key_number >> 24 )&0xFF;

  /* вырабатываем новое значение */
   if(( error = ak_hmac_context_ptr( &nctx->hctx,
                       nctx->buffer, nctx->length, nctx->buffer )) != ak_error_ok ) return NULL;
 return nctx->buffer;
}

/* ----------------------------------------------------------------------------------------------- */
 ak_int32 ak_nkey_context_get_key_number( ak_nkey nctx )
{
  if( nctx == NULL ) return -1;
   else return nctx->key_number;
}

/* ----------------------------------------------------------------------------------------------- */
/*                                                                                       ak_kdf.c  */
/* ----------------------------------------------------------------------------------------------- */
