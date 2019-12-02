/* ----------------------------------------------------------------------------------------------- */
/*  Copyright (c) 2014 - 2019 by Axel Kenzo, axelkenzo@mail.ru                                     */
/*                                                                                                 */
/*  Файл ak_sign.h                                                                                 */
/*  - содержит реализацию функций для работы с электронной подписью.                               */
/* ----------------------------------------------------------------------------------------------- */
 #include <libakrypt-mini.h>

/* ----------------------------------------------------------------------------------------------- */
/*! Функция инициализирует контекст открытого ключа и присваивает ему значение,
    переданное в качестве аргументов функции. Формат передаваемых данных
    соответсвует тому, что формируется функцией ak_verify_context_export_ptr().

    @param pctx Контекст открытого ключа электронной подписи
    @param key Вектор (последовательность байт), образующая открытый ключ.
    @param size Длина открытого ключа в байтах.
    @param wc Указатель на параметры эллиптической кривой.

    @return В случае успеха возвращается \ref ak_error_ok. В противном случае
    возвращается код ошибки.                                                                       */
/* ----------------------------------------------------------------------------------------------- */
 int ak_verifykey_context_create_from_ptr( ak_verifykey pctx,
                                                  ak_pointer key, size_t size, const ak_wcurve wc )
{
#ifndef LIBAKRYPT_LITTLE_ENDIAN
  int i = 0;
#endif
  size_t blen = 0;
  int error = ak_error_ok;

  if( pctx == NULL ) return ak_error_null_pointer;
  if( key == NULL ) return ak_error_null_pointer;
  if(( size != 64 ) && ( size != 128 )) return ak_error_wrong_key_length;
  if(( wc->size != ak_mpzn256_size ) && ( wc->size != ak_mpzn512_size )) return ak_error_wrong_length;

 /* устанавливаем OID алгоритма */
  if( wc->size == ak_mpzn256_size ) {
    if(( error = ak_hash_context_create_streebog256( &pctx->ctx )) != ak_error_ok ) return error;
  }

  if( wc->size == ak_mpzn512_size ) {
    if(( error = ak_hash_context_create_streebog512( &pctx->ctx )) != ak_error_ok ) return error;
  }

 /* устанавливаем эллиптическую кривую */
  pctx->wc = ( ak_wcurve ) wc;

 /* копируем данные */
  blen = wc->size*sizeof( ak_uint64 );
  ak_memset( pctx->qpoint.x, 0, 2*blen );
  ak_memcpy( pctx->qpoint.x, key, blen );

  ak_memset( pctx->qpoint.y, 0, 2*blen );
  ak_memcpy( pctx->qpoint.y, ((ak_uint64 *)key)+wc->size, blen );

  ak_memset( pctx->qpoint.z, 0, sizeof( pctx->qpoint.z ));
  pctx->qpoint.z[0] = 1;

#ifndef LIBAKRYPT_LITTLE_ENDIAN
  for( i = 0; i < pctx->wc->size; i++ ) {
    pctx->qpoint.x[i] = bswap_64( pctx->qpoint.x[i] );
    pctx->qpoint.y[i] = bswap_64( pctx->qpoint.y[i] );
  }
#endif
 return ak_error_ok;
}

/* ----------------------------------------------------------------------------------------------- */
/*! @param pctx контекст открытого ключа алгоритма электронной подписи.
    @return В случае успеха возвращается ноль (\ref ak_error_ok). В противном случае возвращается
    код ошибки.                                                                                    */
/* ----------------------------------------------------------------------------------------------- */
 int ak_verifykey_context_destroy( ak_verifykey pctx )
{
  if( pctx == NULL ) return ak_error_null_pointer;
 return ak_hash_context_destroy( &pctx->ctx );
}

/* ----------------------------------------------------------------------------------------------- */
/*! @param pctx контекст открытого ключа.
    @param hash хеш-код сообщения (последовательность байт), для которого проверяется электронная подпись.
    @param hsize размер хеш-кода, в байтах.
    @param sign электронная подпись, для которой выполняется проверка.
    @return Функция возыращает истину, если подпись верна. Если функция не верна или если
    возникла ошибка, то возвращается ложь. Код шибки может получен с помощью
    вызова функции ak_error_get_value().                                                           */
/* ----------------------------------------------------------------------------------------------- */
 ak_bool ak_verifykey_context_verify_hash( ak_verifykey pctx,
                                        const ak_pointer hash, const size_t hsize, ak_pointer sign )
{
#ifndef LIBAKRYPT_LITTLE_ENDIAN
  int i = 0;
#endif
  ak_mpzn512 v, z1, z2, u, r, s, h;
  struct wpoint cpoint, tpoint;

  if( pctx == NULL ) return ak_false;
  if( hash == NULL ) return ak_false;
  if( hsize != sizeof( ak_uint64 )*(pctx->wc->size )) return ak_false;
  if( sign == NULL ) return ak_false;

  ak_memcpy( r, ( ak_uint64* )sign, sizeof( ak_uint64 )*pctx->wc->size );
  ak_memcpy( s, ( ak_uint64 *)sign + pctx->wc->size, sizeof( ak_uint64 )*pctx->wc->size );
  ak_memcpy( h, hash, sizeof( ak_uint64 )*pctx->wc->size );

#ifndef LIBAKRYPT_LITTLE_ENDIAN
  for( i = 0; i < pctx->wc->size; i++ ) {
     r[i] = bswap_64( r[i] );
     s[i] = bswap_64( s[i] );
     h[i] = bswap_64( h[i] );
  }
#endif

  ak_mpzn_set( v, h, pctx->wc->size );
  ak_mpzn_rem( v, v, pctx->wc->q, pctx->wc->size );
  if( ak_mpzn_cmp_ui( v, pctx->wc->size, 0 )) ak_mpzn_set_ui( v, pctx->wc->size, 1 );
  ak_mpzn_mul_montgomery( v, v, pctx->wc->r2q, pctx->wc->q, pctx->wc->nq, pctx->wc->size );

  /* вычисляем v (в представлении Монтгомери) */
  ak_mpzn_set_ui( u, pctx->wc->size, 2 );
  ak_mpzn_sub( u, pctx->wc->q, u, pctx->wc->size );
  ak_mpzn_modpow_montgomery( v, v, u, pctx->wc->q, pctx->wc->nq, pctx->wc->size ); // v <- v^{q-2} (mod q)

  /* вычисляем z1 */
  ak_mpzn_mul_montgomery( z1, s, pctx->wc->r2q, pctx->wc->q, pctx->wc->nq, pctx->wc->size );
  ak_mpzn_mul_montgomery( z1, z1, v, pctx->wc->q, pctx->wc->nq, pctx->wc->size );
  ak_mpzn_mul_montgomery( z1, z1, pctx->wc->point.z, pctx->wc->q, pctx->wc->nq, pctx->wc->size );

  /* вычисляем z2 */
  ak_mpzn_mul_montgomery( z2, r, pctx->wc->r2q, pctx->wc->q, pctx->wc->nq, pctx->wc->size );
  ak_mpzn_sub( z2, pctx->wc->q, z2, pctx->wc->size );
  ak_mpzn_mul_montgomery( z2, z2, v, pctx->wc->q, pctx->wc->nq, pctx->wc->size );
  ak_mpzn_mul_montgomery( z2, z2, pctx->wc->point.z, pctx->wc->q, pctx->wc->nq, pctx->wc->size );

 /* сложение точек и проверка */
  ak_wpoint_pow( &cpoint, &pctx->wc->point, z1, pctx->wc->size, pctx->wc );
  ak_wpoint_pow( &tpoint, &pctx->qpoint, z2, pctx->wc->size, pctx->wc );
  ak_wpoint_add( &cpoint, &tpoint, pctx->wc );
  ak_wpoint_reduce( &cpoint, pctx->wc );
  ak_mpzn_rem( cpoint.x, cpoint.x, pctx->wc->q, pctx->wc->size );

  if( ak_mpzn_cmp( cpoint.x, r, pctx->wc->size )) return ak_false;
 return ak_true;
}

/* ----------------------------------------------------------------------------------------------- */
/*! @param pctx контекст открытого ключа.
    @param in область памяти для которой проверяется электронная подпись.
    @param size размер области памяти в байтах.
    @param sign электронная подпись, для которой выполняется проверка.
    @return Функция возыращает истину, если подпись верна. Если функция не верна или если
    возникла ошибка, то возвращается ложь. Код шибки может получен с помощью
    вызова функции ak_error_get_value().                                                           */
/* ----------------------------------------------------------------------------------------------- */
 ak_bool ak_verifykey_context_verify_ptr( ak_verifykey pctx, const ak_pointer in,
                                                               const size_t size, ak_pointer sign )
{
  ak_uint8 hash[64];
  int error = ak_error_ok;

 /* необходимые проверки */
  if( pctx == NULL ) return ak_false;
  if( in == NULL ) return ak_false;
  if( pctx->ctx.hsize > 64 ) return ak_false;

 /* вычисляем значение хеш-кода, а после подписываем его */
  ak_memset( hash, 0, 64 );
  if(( error = ak_hash_context_ptr( &pctx->ctx, in, size, hash )) != ak_error_ok ) return ak_false;

 return ak_verifykey_context_verify_hash( pctx, hash, pctx->ctx.hsize, sign );
}

/* ----------------------------------------------------------------------------------------------- */
/*                                                                                      ak_sign.c  */
/* ----------------------------------------------------------------------------------------------- */
