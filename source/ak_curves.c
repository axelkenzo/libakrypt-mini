/* ----------------------------------------------------------------------------------------------- */
/*  Copyright (c) 2014 - 2019 by Axel Kenzo, axelkenzo@mail.ru                                     */
/*                                                                                                 */
/*  Файл ak_curves.с                                                                               */
/*  - содержит реализацию функций для работы с эллиптическими кривыми.                             */
/* ----------------------------------------------------------------------------------------------- */
 #include <string.h>
 #include <libakrypt-mini.h>

/* ----------------------------------------------------------------------------------------------- */
/*                         теперь параметры эллиптических кривых                                   */
/* ----------------------------------------------------------------------------------------------- */
/*! \brief Параметры 256-ти битной эллиптической кривой из тестового примера ГОСТ Р 34.10-2012 (Приложение А.1). */
/*! \code
      a = "7",
      b = "5FBFF498AA938CE739B8E022FBAFEF40563F6E6A3472FC2A514C0CE9DAE23B7E",
      p = "8000000000000000000000000000000000000000000000000000000000000431",
      q = "8000000000000000000000000000000150FE8A1892976154C59CFC193ACCF5B3",
     px = "2",
     py = "8E2A8A0E65147D4BD6316030E16D19C85C97F0A9CA267122B96ABBCEA7E8FC8"
    \endcode                                                                                       */
/* ----------------------------------------------------------------------------------------------- */
 static struct wcurve id_tc26_gost_3410_2012_256_paramSetTest = {
  ak_mpzn256_size,
  1,
  { 0xffffffffffffc983LL, 0xffffffffffffffffLL, 0xffffffffffffffffLL, 0x7fffffffffffffffLL }, /* a (в форме Монтгомери) */
  { 0x807bbfa323a3952aLL, 0x004469b4541a2542LL, 0x20391abe272c66adLL, 0x58df983a171cd5aeLL }, /* b (в форме Монтгомери) */
  { 0x0000000000000431LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x8000000000000000LL }, /* p */
  { 0x0000000000464584LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* r2 */
  { 0xc59cfc193accf5b3LL, 0x50fe8a1892976154LL, 0x0000000000000001LL, 0x8000000000000000LL }, /* q */
  { 0xecaed44677f7f28dLL, 0x4af1f8ac73c6c555LL, 0xc0db8b05c83ad16aLL, 0x6e749e5b503b112aLL }, /* r2q */
  {
    { 0x0000000000000002LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* px */
    { 0x2b96abbcea7e8fc8LL, 0x85c97f0a9ca26712LL, 0xbd6316030e16d19cLL, 0x08e2a8a0e65147d4LL }, /* py */
    { 0x0000000000000001LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }  /* pz */
  },
  0xdbf951d5883b2b2fLL, /* n */
  0x66ff43a234713e85LL, /* nq */
  "8000000000000000000000000000000000000000000000000000000000000431"
 };

/* ----------------------------------------------------------------------------------------------- */
/*! \brief Параметры 256-ти битной эллиптической кривой из рекомендаций Р 50.1.114-2016 (paramSetA). */
/*! \code
      a = "C2173F1513981673AF4892C23035A27CE25E2013BF95AA33B22C656F277E7335",
      b = "295F9BAE7428ED9CCC20E7C359A9D41A22FCCD9108E17BF7BA9337A6F8AE9513",
      p = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD97",
      q = "400000000000000000000000000000000FD8CDDFC87B6635C115AF556C360C67",
     px = "91E38443A5E82C0D880923425712B2BB658B9196932E02C78B2582FE742DAA28",
     py = "32879423AB1A0375895786C4BB46E9565FDE0B5344766740AF268ADB32322E5C",
    \endcode                                                                                       */
/* ----------------------------------------------------------------------------------------------- */
 struct wcurve id_tc26_gost_3410_2012_256_paramSetA = {
  ak_mpzn256_size,
  4, /* cofactor */
  { 0x6d0078e62fc81048LL, 0x94db4f98bfb73698LL, 0x75e9b60631449efdLL, 0xca0709cc398e1cd1LL }, /* a */
  { 0xacd1216d5cc63966LL, 0x534b728e6773c810LL, 0xfb4e95d31a5032feLL, 0xb76e3775f6a4aee7LL }, /* b */
  { 0xfffffffffffffd97LL, 0xffffffffffffffffLL, 0xffffffffffffffffLL, 0xffffffffffffffffLL }, /* p */
  { 0x000000000005cf11LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* r2 */
  { 0xc115af556c360c67LL, 0x0fd8cddfc87b6635LL, 0x0000000000000000LL, 0x4000000000000000LL }, /* q */
  { 0x57cb446240dd1710LL, 0x7556091c4805caa4LL, 0xd0593365f9384bcdLL, 0x0fb1fbc48b0f0eb4LL }, /* r2q */
  {
    { 0x8b2582fe742daa28LL, 0x658b9196932e02c7LL, 0x880923425712b2bbLL, 0x91e38443a5e82c0dLL }, /* px */
    { 0xaf268adb32322e5cLL, 0x5fde0b5344766740LL, 0x895786c4bb46e956LL, 0x32879423ab1a0375LL }, /* py */
    { 0x0000000000000001LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }  /* pz */
  },
  0x46f3234475d5add9LL, /* n */
  0x035bdd1aeafdb0a9LL, /* nq */
  "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD97"
};

/* ----------------------------------------------------------------------------------------------- */
/*! \brief Параметры 256-ти битной эллиптической кривой, определяемые RFC-4357, set A (вариант КриптоПро). */
/*! \code
      a = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD94",
      b = "A6",
      p = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD97",
      q = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6C611070995AD10045841B09B761B893",
     px = "1",
     qx = "8D91E471E0989CDA27DF505A453F2B7635294F2DDF23E3B122ACC99C9E9F1E14"
    \endcode                                                                                       */
/* ----------------------------------------------------------------------------------------------- */
 struct wcurve id_rfc4357_gost_3410_2001_paramSetA = {
  ak_mpzn256_size,
  1,
  { 0xfffffffffffff65cLL, 0xffffffffffffffffLL, 0xffffffffffffffffLL, 0xffffffffffffffffLL }, /* a */
  { 0x0000000000019016LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* b */
  { 0xfffffffffffffd97LL, 0xffffffffffffffffLL, 0xffffffffffffffffLL, 0xffffffffffffffffLL }, /* p */
  { 0x000000000005cf11LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* r2 */
  { 0x45841b09b761b893LL, 0x6c611070995ad100LL, 0xffffffffffffffffLL, 0xffffffffffffffffLL }, /* q */
  { 0x9ac2d7858e79a469LL, 0xfb07f8222e76dd52LL, 0xf74885d08a3714c6LL, 0x551fe9cb451179dbLL }, /* r2q */
  {
    { 0x0000000000000001LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* px */
    { 0x22acc99c9e9f1e14LL, 0x35294f2ddf23e3b1LL, 0x27df505a453f2b76LL, 0x8d91e471e0989cdaLL }, /* py */
    { 0x0000000000000001LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }  /* pz */
  },
  0x46f3234475d5add9LL, /* n */
  0x9ee6ea0b57c7da65LL, /* nq */
  "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD97"
 };

/* ----------------------------------------------------------------------------------------------- */
/*! \brief Параметры 256-ти битной эллиптической кривой, определяемые RFC-4357, set B (вариант КриптоПро). */
/*! \code
      a = "8000000000000000000000000000000000000000000000000000000000000C96",
      b = "3E1AF419A269A5F866A7D3C25C3DF80AE979259373FF2B182F49D4CE7E1BBC8B",
      p = "8000000000000000000000000000000000000000000000000000000000000C99",
      q = "800000000000000000000000000000015F700CFFF1A624E5E497161BCC8A198F",
     px = "1",
     py = "3FA8124359F96680B83D1C3EB2C070E5C545C9858D03ECFB744BF8D717717EFC"
    \endcode                                                                                       */
/* ----------------------------------------------------------------------------------------------- */
 struct wcurve id_rfc4357_gost_3410_2001_paramSetB = {
  ak_mpzn256_size,
  1,
  { 0x0000000000004b96LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* a */
  { 0x8dcc455aa9c5a084LL, 0x91ab42df6cf438a8LL, 0x8f8aa907eeac7d11LL, 0x3ce5d221f6285375LL }, /* b */
  { 0x0000000000000c99LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x8000000000000000LL }, /* p */
  { 0x00000000027acdc4LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* r2 */
  { 0xe497161bcc8a198fLL, 0x5f700cfff1a624e5LL, 0x0000000000000001LL, 0x8000000000000000LL }, /* q */
  { 0x29b721f4e6cd7823LL, 0x2a3104a7ea43e855LL, 0x4a2e7e2f6882cf10LL, 0x09d1d2c4e5082466LL }, /* r2q */
  {
    { 0x0000000000000001LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* px */
    { 0x744bf8d717717efcLL, 0xc545c9858d03ecfbLL, 0xb83d1c3eb2c070e5LL, 0x3fa8124359f96680LL }, /* py */
    { 0x0000000000000001LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }  /* pz */
  },
  0xbd667ab8a3347857LL, /* n */
  0xca89614990611a91LL, /* nq */
  "8000000000000000000000000000000000000000000000000000000000000C99"
 };

/* ----------------------------------------------------------------------------------------------- */
/*! \brief Параметры 256-ти битной эллиптической кривой, определяемые RFC-4357, set C (вариант КриптоПро). */
/*! \code
       a = "9B9F605F5A858107AB1EC85E6B41C8AACF846E86789051D37998F7B9022D7598",
       b = "805A",
       p = "9B9F605F5A858107AB1EC85E6B41C8AACF846E86789051D37998F7B9022D759B",
       q = "9B9F605F5A858107AB1EC85E6B41C8AA582CA3511EDDFB74F02F3A6598980BB9",
      px = "0",
      py = "41ECE55743711A8C3CBF3783CD08C0EE4D4DC440D4641A8F366E550DFDB3BB67"
    \endcode                                                                                       */
/* ----------------------------------------------------------------------------------------------- */
 struct wcurve id_rfc4357_gost_3410_2001_paramSetC = {
  ak_mpzn256_size,
  1,
  { 0x5ffcd69d0ae34c07LL, 0x0d9628a05ad19921LL, 0x5799e9d81848eb56LL, 0x0a1ce1dcc49b8526LL }, /* a */
  { 0x4be8a4e93bda2acfLL, 0x79cc0e3e90d382ddLL, 0x3ba4c8b01d9cc79bLL, 0x5cc73b5a966609e9LL }, /* b */
  { 0x7998f7b9022d759bLL, 0xcf846e86789051d3LL, 0xab1ec85e6b41c8aaLL, 0x9b9f605f5a858107LL }, /* p */
  { 0x409973b4c427fceaLL, 0x1017bb39c2d346c5LL, 0x186304212849c07bLL, 0x807a394ede097652LL }, /* r2 */
  { 0xf02f3a6598980bb9LL, 0x582ca3511eddfb74LL, 0xab1ec85e6b41c8aaLL, 0x9b9f605f5a858107LL }, /* q */
  { 0xe94faab66aba180eLL, 0x04fda8694afda24bLL, 0xc67e5d0ee96e8ed3LL, 0x7aa61b49a49d4759LL }, /* r2q */
  {
    { 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* px */
    { 0x366e550dfdb3bb67LL, 0x4d4dc440d4641a8fLL, 0x3cbf3783cd08c0eeLL, 0x41ece55743711a8cLL }, /* py */
    { 0x0000000000000001LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }  /* pz */
  },
  0xdf6e6c2c727c176dLL, /* n */
  0xa1c6af0a552f7577LL, /* nq */
  "9B9F605F5A858107AB1EC85E6B41C8AACF846E86789051D37998F7B9022D759B"
 };

/* ----------------------------------------------------------------------------------------------- */
/*! \brief Параметры 512-ти битной эллиптической кривой из тестового примера ГОСТ Р 34.10-2012 (Приложение А.2). */
/*! \code
      a = "7",
      b = "1CFF0806A31116DA29D8CFA54E57EB748BC5F377E49400FDD788B649ECA1AC4361834013B2AD7322480A89CA58E0CF74BC9E540C2ADD6897FAD0A3084F302ADC",
      p = "4531ACD1FE0023C7550D267B6B2FEE80922B14B2FFB90F04D4EB7C09B5D2D15DF1D852741AF4704A0458047E80E4546D35B8336FAC224DD81664BBF528BE6373",
      q = "4531ACD1FE0023C7550D267B6B2FEE80922B14B2FFB90F04D4EB7C09B5D2D15DA82F2D7ECB1DBAC719905C5EECC423F1D86E25EDBE23C595D644AAF187E6E6DF",
     px = "24D19CC64572EE30F396BF6EBBFD7A6C5213B3B3D7057CC825F91093A68CD762FD60611262CD838DC6B60AA7EEE804E28BC849977FAC33B4B530F1B120248A9A",
     py = "2BB312A43BD2CE6E0D020613C857ACDDCFBF061E91E5F2C3F32447C259F39B2C83AB156D77F1496BF7EB3351E1EE4E43DC1A18B91B24640B6DBB92CB1ADD371E",
    \endcode                                                                                       */
/* ----------------------------------------------------------------------------------------------- */
 struct wcurve id_tc26_gost_3410_2012_512_paramSetTest = {
  ak_mpzn512_size,
  1,
  { 0xd029a50f056849c5ULL, 0xc102fa1830a665e5ULL, 0x93678fa569b3c155ULL, 0x61dff2a95e2108c5ULL, 0x3500e30d3e698dd3ULL, 0xb9cafa8506ed8887ULL, 0xb1b73df28851b571ULL, 0x3e261f7e31fc8188ULL }, /* a */
  { 0x3d869f8d06cde456ULL, 0x22167b920ce0bfcbULL, 0xf7fdd636df3cc250ULL, 0x45228319a5e6292dULL, 0xfd513828d9ad288dULL, 0xc7d45cb277e670aaULL, 0x04890c718bc5c744ULL, 0x1a693f403fc50f21ULL }, /* b */
  { 0x1664bbf528be6373ULL, 0x35b8336fac224dd8ULL, 0x0458047e80e4546dULL, 0xf1d852741af4704aULL, 0xd4eb7c09b5d2d15dULL, 0x922b14b2ffb90f04ULL, 0x550d267b6b2fee80ULL, 0x4531acd1fe0023c7ULL }, /* p */
  { 0x001c10bc2d005b65ULL, 0x4b907a71e647ee63ULL, 0xe417d58d200c2aa0ULL, 0x0815b9eb1e7dd300ULL, 0xca0bc8af77c8690aULL, 0xfcd983cfb7c663d9ULL, 0x01fde9ca99de0852ULL, 0x1d887dcd9cd19c10ULL }, /* r2 */
  { 0xd644aaf187e6e6dfULL, 0xd86e25edbe23c595ULL, 0x19905c5eecc423f1ULL, 0xa82f2d7ecb1dbac7ULL, 0xd4eb7c09b5d2d15dULL, 0x922b14b2ffb90f04ULL, 0x550d267b6b2fee80ULL, 0x4531acd1fe0023c7ULL }, /* q */
  { 0xb03174e56db6ba90ULL, 0x561500cb39a9b66bULL, 0x929e0924887fab48ULL, 0xe23c04dc39c8c930ULL, 0x0cc44723bcc36979ULL, 0xd70dfcccc3dd062fULL, 0x80bc9d923a08f9a9ULL, 0x3057350e3201bb36ULL }, /* r2q */
  {
    { 0xb530f1b120248a9aULL, 0x8bc849977fac33b4ULL, 0xc6b60aa7eee804e2ULL, 0xfd60611262cd838dULL, 0x25f91093a68cd762ULL, 0x5213b3b3d7057cc8ULL, 0xf396bf6ebbfd7a6cULL, 0x24d19cc64572ee30ULL }, /* px */
    { 0x6dbb92cb1add371eULL, 0xdc1a18b91b24640bULL, 0xf7eb3351e1ee4e43ULL, 0x83ab156d77f1496bULL, 0xf32447c259f39b2cULL, 0xcfbf061e91e5f2c3ULL, 0x0d020613c857acddULL, 0x2bb312a43bd2ce6eULL }, /* py */
    { 0x0000000000000001ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL }  /* pz */
  },
  0xd6412ff7c29b8645LL, /* n */
  0x50bc7d084a21aae1LL, /* nq */
  "4531ACD1FE0023C7550D267B6B2FEE80922B14B2FFB90F04D4EB7C09B5D2D15DF1D852741AF4704A0458047E80E4546D35B8336FAC224DD81664BBF528BE6373"
 };

/* ----------------------------------------------------------------------------------------------- */
/*! \brief Параметры 512-ти битной эллиптической кривой из рекомендаций Р 50.1.114-2016 (paramSetA). */
/*! \code
      a = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDC4",
      b = "E8C2505DEDFC86DDC1BD0B2B6667F1DA34B82574761CB0E879BD081CFD0B6265EE3CB090F30D27614CB4574010DA90DD862EF9D4EBEE4761503190785A71C760",
      p = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDC7",
      q = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF27E69532F48D89116FF22B8D4E0560609B4B38ABFAD2B85DCACDB1411F10B275",
     px = "3",
     py = "7503CFE87A836AE3A61B8816E25450E6CE5E1C93ACF1ABC1778064FDCBEFA921DF1626BE4FD036E93D75E6A50E3A41E98028FE5FC235F5B889A589CB5215F2A4",
    \endcode                                                                                       */
/* ----------------------------------------------------------------------------------------------- */
 struct wcurve id_tc26_gost_3410_2012_512_paramSetA = {
  ak_mpzn512_size,
  1,
  { 0xfffffffffffff71cULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL }, /* a */
  { 0x3e2a1b8106e8a17dULL, 0x3e694a40649ca74bULL, 0x7cd5ed6575cbfc5fULL, 0x84e4722c383c8743ULL, 0x9527086e6e4db48eULL, 0x2d4b3fda85c534b6ULL, 0x9d2dd3769d088dffULL, 0x57e4a0c5f647c2e3ULL }, /* b */
  { 0xfffffffffffffdc7ULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL }, /* p */
  { 0x000000000004f0b1ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL }, /* r2 */
  { 0xcacdb1411f10b275ULL, 0x9b4b38abfad2b85dULL, 0x6ff22b8d4e056060ULL, 0x27e69532f48d8911ULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL }, /* q */
  { 0x546775b92106e979ULL, 0xb55cd33800ab10e6ULL, 0x80b08b27e9cebbc7ULL, 0xa06b76a2bae6fc86ULL, 0xc7433579e382956fULL, 0xbab8be5dd7b1651dULL, 0xee028bf9d8ed3314ULL, 0xb66ae6c00bebd6c3ULL }, /* r2q */
  {
    { 0x0000000000000003ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL }, /* px */
    { 0x89a589cb5215f2a4ULL, 0x8028fe5fc235f5b8ULL, 0x3d75e6a50e3a41e9ULL, 0xdf1626be4fd036e9ULL, 0x778064fdcbefa921ULL, 0xce5e1c93acf1abc1ULL, 0xa61b8816e25450e6ULL, 0x7503cfe87a836ae3ULL }, /* py */
    { 0x0000000000000001ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL }  /* pz */
  },
  0x58a1f7e6ce0f4c09LL, /* n */
  0x02ccc1665d51f223LL, /* nq */
  "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDC7"
 };

/* ----------------------------------------------------------------------------------------------- */
/*! \brief Параметры 512-ти битной эллиптической кривой из рекомендаций Р 50.1.114-2016 (paramSetB). */
/*! \code
      a = "8000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000006C",
      b = "687D1B459DC841457E3E06CF6F5E2517B97C7D614AF138BCBF85DC806C4B289F3E965D2DB1416D217F8B276FAD1AB69C50F78BEE1FA3106EFB8CCBC7C5140116",
      p = "8000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000006F",
      q = "800000000000000000000000000000000000000000000000000000000000000149A1EC142565A545ACFDB77BD9D40CFA8B996712101BEA0EC6346C54374F25BD",
     px = "2",
     py = "1A8F7EDA389B094C2C071E3647A8940F3C123B697578C213BE6DD9E6C8EC7335DCB228FD1EDF4A39152CBCAAF8C0398828041055F94CEEEC7E21340780FE41BD",
    \endcode                                                                                       */
/* ----------------------------------------------------------------------------------------------- */
 struct wcurve id_tc26_gost_3410_2012_512_paramSetB = {
  ak_mpzn512_size,
  1,
  { 0x000000000000029aULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL }, /* a */
  { 0xdbe748c318a75dd6ULL, 0xc954a7809097bfc1ULL, 0x6553cd27e2d5a471ULL, 0xb99b326049435cf3ULL, 0xe9eac8a216d2c5e7ULL, 0x260b45a102d0cc51ULL, 0x8636181d6c5bd56dULL, 0x638259a12c5765bcULL }, /* b */
  { 0x000000000000006fULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x8000000000000000ULL }, /* p */
  { 0x000000000000c084ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL }, /* r2 */
  { 0xc6346c54374f25bdULL, 0x8b996712101bea0eULL, 0xacfdb77bd9d40cfaULL, 0x49a1ec142565a545ULL, 0x0000000000000001ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x8000000000000000ULL }, /* q */
  { 0x3163da9749d3cb8bULL, 0x267d56905313f38bULL, 0xc55538cf997acac4ULL, 0xb1532b08f1e25e5cULL, 0xc385980eb887a3f9ULL, 0x9f96043308eeb401ULL, 0xf96232d7a52b18feULL, 0x21c65cda4cadccc0ULL }, /* r2q */
  {
    { 0x0000000000000002ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL }, /* px */
    { 0x7e21340780fe41bdULL, 0x28041055f94ceeecULL, 0x152cbcaaf8c03988ULL, 0xdcb228fd1edf4a39ULL, 0xbe6dd9e6c8ec7335ULL, 0x3c123b697578c213ULL, 0x2c071e3647a8940fULL, 0x1a8f7eda389b094cULL }, /* py */
    { 0x0000000000000001ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL }  /* pz */
  },
  0x4e6a171024e6a171LL, /* n */
  0xc07d62492cbac26bLL, /* nq */
  "8000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000006F",
 };

/* ----------------------------------------------------------------------------------------------- */
/*! \brief Параметры 512-ти битной эллиптической кривой из рекомендаций Р 50.1.114-2016 (paramSetC). */
/*! \code
      a = "DC9203E514A721875485A529D2C722FB187BC8980EB866644DE41C68E143064546E861C0E2C9EDD92ADE71F46FCF50FF2AD97F951FDA9F2A2EB6546F39689BD3",
      b = "B4C4EE28CEBC6C2C8AC12952CF37F16AC7EFB6A9F69F4B57FFDA2E4F0DE5ADE038CBC2FFF719D2C18DE0284B8BFEF3B52B8CC7A5F5BF0A3C8D2319A5312557E1",
      p = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDC7",
      q = "3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC98CDBA46506AB004C33A9FF5147502CC8EDA9E7A769A12694623CEF47F023ED",
     px = "E2E31EDFC23DE7BDEBE241CE593EF5DE2295B7A9CBAEF021D385F7074CEA043AA27272A7AE602BF2A7B9033DB9ED3610C6FB85487EAE97AAC5BC7928C1950148",
     py = "F5CE40D95B5EB899ABBCCFF5911CB8577939804D6527378B8C108C3D2090FF9BE18E2D33E3021ED2EF32D85822423B6304F726AA854BAE07D0396E9A9ADDC40F",
    \endcode                                                                                       */
/* ----------------------------------------------------------------------------------------------- */
 struct wcurve id_tc26_gost_3410_2012_512_paramSetC = {
  ak_mpzn512_size,
  4,
  { 0xd341ab3699869915ULL, 0x3d6c9273ccebc4c1ULL, 0x486b484c83cb0726ULL, 0x9a8145b812d1a7b0ULL, 0x2003251cadf8effaULL, 0x6b20d9f8b7db94f1ULL, 0xdd0c19f57c9cc019ULL, 0x408aa82ae77985caULL }, /* a */
  { 0xb304002a3c03ce62ULL, 0xcbe7bfdf359dc095ULL, 0x57398fea29abadadULL, 0x3ce46aec38657034ULL, 0xabf0edb5e37f775eULL, 0x63ccffc5280e7697ULL, 0x6754d90e93579656ULL, 0xc9b558b380cc6f00ULL }, /* b */
  { 0xfffffffffffffdc7ULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL }, /* p */
  { 0x000000000004f0b1ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL }, /* r2 */
  { 0x94623cef47f023edULL, 0xc8eda9e7a769a126ULL, 0x4c33a9ff5147502cULL, 0xc98cdba46506ab00ULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0x3fffffffffffffffULL }, /* q */
  { 0xe58fa18ee6ca4eb6ULL, 0xe79280282d956fcaULL, 0xd016086ec2d4f903ULL, 0x542f8f3fa490666aULL, 0x04f77045db49adc9ULL, 0x314e0a57f445b20eULL, 0x8910352f3bea2192ULL, 0x394c72054d8503beULL }, /* r2q */
  {
    { 0xc5bc7928c1950148ULL, 0xc6fb85487eae97aaULL, 0xa7b9033db9ed3610ULL, 0xa27272a7ae602bf2ULL, 0xd385f7074cea043aULL, 0x2295b7a9cbaef021ULL, 0xebe241ce593ef5deULL, 0xe2e31edfc23de7bdULL }, /* px */
    { 0xd0396e9a9addc40fULL, 0x04f726aa854bae07ULL, 0xef32d85822423b63ULL, 0xe18e2d33e3021ed2ULL, 0x8c108c3d2090ff9bULL, 0x7939804d6527378bULL, 0xabbccff5911cb857ULL, 0xf5ce40d95b5eb899ULL }, /* py */
    { 0x0000000000000001ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL }  /* pz */
  },
  0x58a1f7e6ce0f4c09LL, /* n */
  0x0ed9d8e0b6624e1bLL, /* nq */
  "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDC7"
 };

/* ----------------------------------------------------------------------------------------------- */
/*! Функция вычисляерт величину \f$\Delta \equiv -16(4a^3 + 27b^2) \pmod{p} \f$, зависящую
    от параметров эллиптической кривой

    @param d Вычет, в который помещается вычисленное значение.
    @param ec Контекст эллиптической кривой, для которой вычисляется ее дискриминант               */
/* ----------------------------------------------------------------------------------------------- */
 void ak_mpzn_set_wcurve_discriminant( ak_uint64 *d, ak_wcurve ec )
{
  ak_mpznmax s, one = ak_mpznmax_one;

 /* определяем константы 4 и 27 в представлении Монтгомери */
  ak_mpzn_set_ui( d, ec->size, 4 );
  ak_mpzn_set_ui( s, ak_mpznmax_size, 27 );
  ak_mpzn_mul_montgomery( d, d, ec->r2, ec->p, ec->n, ec->size );
  ak_mpzn_mul_montgomery( s, s, ec->r2, ec->p, ec->n, ec->size );

 /* вычисляем 4a^3 (mod p) значение в представлении Монтгомери */
  ak_mpzn_mul_montgomery( d, d, ec->a, ec->p, ec->n, ec->size );
  ak_mpzn_mul_montgomery( d, d, ec->a, ec->p, ec->n, ec->size );
  ak_mpzn_mul_montgomery( d, d, ec->a, ec->p, ec->n, ec->size );

 /* вычисляем значение 4a^3 + 27b^2 (mod p) в представлении Монтгомери */
  ak_mpzn_mul_montgomery( s, s, ec->b, ec->p, ec->n, ec->size );
  ak_mpzn_mul_montgomery( s, s, ec->b, ec->p, ec->n, ec->size );
  ak_mpzn_add_montgomery( d, d, s, ec->p, ec->size );

 /* определяем константу -16 в представлении Монтгомери и вычисляем D = -16(4a^3+27b^2) (mod p) */
  ak_mpzn_set_ui( s, ec->size, 16 );
  ak_mpzn_sub( s, ec->p, s, ec->size );
  ak_mpzn_mul_montgomery( s, s, ec->r2, ec->p, ec->n, ec->size );
  ak_mpzn_mul_montgomery( d, d, s, ec->p, ec->n, ec->size );

 /* возвращаем результат (в обычном представлении) */
  ak_mpzn_mul_montgomery( d, d, one, ec->p, ec->n, ec->size );
}

/* ----------------------------------------------------------------------------------------------- */
/*! @param ec Контекст эллиптической кривой, для которой проверяется, что ее дискриминант
    отличен от нуля.

    @return Функция возвращает \ref ak_error_ok в случае, если дискриминант отличен от нуля.
    В противном случае возвращается код ошибки.                                                    */
/* ----------------------------------------------------------------------------------------------- */
 int ak_wcurve_discriminant_is_ok( ak_wcurve ec )
{
  ak_mpznmax d;
  if( ec == NULL ) return ak_error_null_pointer;

  ak_mpzn_set_wcurve_discriminant( d, ec );
  if( ak_mpzn_cmp_ui( d, ec->size, 0 ) == ak_true ) return ak_error_curve_discriminant;
   else return ak_error_ok;
}


/* ----------------------------------------------------------------------------------------------- */
/*! Для проведения проверки функция вырабатывает случайное число \f$ t \pmod{q} \f$ и проверяет
    выполнимость равенства \f$ t \cdot t^{-1} \equiv 1 \pmod{q}\f$.

    @param ec Контекст эллиптической кривой.

    @return Функция возвращает \ref ak_error_ok в случае, если параметры определены корректно.
    В противном случае возвращается код ошибки.                                                    */
/* ----------------------------------------------------------------------------------------------- */
 int ak_wcurve_check_order_parameters( ak_wcurve ec )
{
  ak_mpzn512 r, s, t;

  memset( t, 0x01, sizeof( ak_uint64 )*ec->size ); // ak_mpzn_set_random( t, ec->size );
  ak_mpzn_rem( t, t, ec->q, ec->size );

  ak_mpzn_set_ui( r, ec->size, 2 );
  ak_mpzn_sub( r, ec->q, r, ec->size );
  ak_mpzn_modpow_montgomery( s, t, r, ec->q, ec->nq, ec->size );
  ak_mpzn_mul_montgomery( t, s, t, ec->q, ec->nq, ec->size );

  ak_mpzn_mul_montgomery( t, t, ec->r2q, ec->q, ec->nq, ec->size );
  ak_mpzn_mul_montgomery( t, t, ec->point.z, ec->q, ec->nq, ec->size );
  ak_mpzn_mul_montgomery( t, t, ec->point.z, ec->q, ec->nq, ec->size );
  if( ak_mpzn_cmp_ui( t, ec->size, 1 )) return ak_error_ok;
   else return ak_error_curve_order_parameters;
}

/* ----------------------------------------------------------------------------------------------- */
/*! Функция принимает на вход контекст эллиптической кривой, заданной в короткой форме Вейерштрасса,
    и выполняет следующие математические проверки

     - проверяется, что модуль кривой (простое число \f$ p \f$) удовлетворяет неравенству
       \f$ 2^{n-32} < p < 2^n \f$, где \f$ n \f$ это либо 256, либо 512 в зависимости от
       параметров кривой,
     - проверяется, что дискриминант кривой отличен от нуля по модулю \f$ p \f$,
     - проверяется, что фиксированная точка кривой, содержащаяся в контексте эллиптической кривой,
       действительно принадлежит эллиптической кривой,
     - проверяется, что порядок этой точки кривой равен простому числу \f$ q \f$,
       содержащемуся в контексте эллиптической кривой.

     @param ec контекст структуры эллиптической кривой, содержащий в себе значения параметров.
     Константные значения структур, которые могут быть использованы библиотекой,
     задаются в файле \ref ak_parameters.h

     @return В случае успеха, функция возвращает \ref ak_error_ok. В противном случае,
     возвращается код ошибки.                                                                      */
/* ----------------------------------------------------------------------------------------------- */
 int ak_wcurve_is_ok( ak_wcurve ec )
{
  struct wpoint wp;
  int error = ak_error_ok;

 /* создали кривую и проверяем веоичину старшего коэффициента ее молуля */
  if( ec->p[ ec->size-1 ] < 0x100000000LL ) return ak_error_curve_prime_modulo;

 /* проверяем, что дискриминант кривой отличен от нуля */
  if(( error = ak_wcurve_discriminant_is_ok( ec )) != ak_error_ok )
    return ak_error_curve_discriminant;

 /* теперь проверяем принадлежность точки кривой */
  if(( error = ak_wpoint_set( &wp, ec )) != ak_error_ok ) return error;
  if( ak_wpoint_is_ok( &wp, ec ) != ak_true ) return ak_error_curve_point;
  if( ak_wpoint_check_order( &wp, ec ) != ak_true ) return ak_error_curve_point_order;

 /* тестируем параметры порядка группы точек, используемые для выработки и проверки электронной подписи */
  if(( error = ak_wcurve_check_order_parameters( ec )) != ak_error_ok ) return error;

 return error;
}

/* ----------------------------------------------------------------------------------------------- */
/*! Проверяются параметры всех эллиптических кривых, доступных через механизм OID.
    Проверка производится путем вызова функции ak_wcurve_is_ok().

    @return Возвращает ak_true в случае успешного тестирования. В случае возникновения
    ошибки функция возвращает ak_false. Код ошибки можеть быть получен с помощью вызова
    ak_error_get_value().                                                                          */
/* ----------------------------------------------------------------------------------------------- */
 int ak_wcurve_test( void )
{
  int reason = ak_error_ok;

 /* организуем цикл по перебору всех известных библиотеке параметров эллиптических кривых */
  if(( reason = ak_wcurve_is_ok( &id_tc26_gost_3410_2012_256_paramSetTest )) != ak_error_ok )
    return reason;
  if(( reason = ak_wcurve_is_ok( &id_tc26_gost_3410_2012_256_paramSetA )) != ak_error_ok )
    return reason;
  if(( reason = ak_wcurve_is_ok( &id_tc26_gost_3410_2012_256_paramSetB )) != ak_error_ok )
    return reason;
  if(( reason = ak_wcurve_is_ok( &id_tc26_gost_3410_2012_256_paramSetC )) != ak_error_ok )
    return reason;
  if(( reason = ak_wcurve_is_ok( &id_tc26_gost_3410_2012_256_paramSetD )) != ak_error_ok )
    return reason;

  if(( reason = ak_wcurve_is_ok( &id_tc26_gost_3410_2012_512_paramSetTest )) != ak_error_ok )
    return reason;
  if(( reason = ak_wcurve_is_ok( &id_tc26_gost_3410_2012_512_paramSetA  )) != ak_error_ok )
    return reason;
  if(( reason = ak_wcurve_is_ok( &id_tc26_gost_3410_2012_512_paramSetB  )) != ak_error_ok )
    return reason;
  if(( reason = ak_wcurve_is_ok( &id_tc26_gost_3410_2012_512_paramSetC  )) != ak_error_ok )
    return reason;

 return reason;
}

/* ----------------------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------------------- */
/*          реализация операций с точками эллиптической кривой в короткой форме Вейерштрасса       */
/* ----------------------------------------------------------------------------------------------- */
/*! @param wp точка \f$ P \f$ эллиптической кривой, которой присваивается значение,
    содержащееся в контексте эллиптической кривой.
    @param wc эллиптическая кривая, которой принадлежит точка.
    @return Функция возвращает \ref ak_error_ok. В случае, когда один  из контекстов
    равен NULL, то возвращается \ref ak_error_null_pointer.                                        */
/* ----------------------------------------------------------------------------------------------- */
 int ak_wpoint_set( ak_wpoint wp, ak_wcurve wc )
{
  if( wp == NULL ) return ak_error_null_pointer;
  if( wc == NULL ) return ak_error_null_pointer;

 /* копируем данные */
  memcpy( wp->x, wc->point.x, wc->size*sizeof( ak_uint64 ));
  memcpy( wp->y, wc->point.y, wc->size*sizeof( ak_uint64 ));
  memcpy( wp->z, wc->point.z, wc->size*sizeof( ak_uint64 ));

 return ak_error_ok;
}

/* ----------------------------------------------------------------------------------------------- */
/*! @param wp точка \f$ P \f$ эллиптической кривой, которой присваивается значение
    бесконечно удаленной точки.
    @param wc эллиптическая кривая, которой принадлежит точка.
    @return Функция возвращает \ref ak_error_ok. В случае, когда один  из контекстов
    равен NULL, то возвращается \ref ak_error_null_pointer.                                        */
/* ----------------------------------------------------------------------------------------------- */
 int ak_wpoint_set_as_unit( ak_wpoint wp, ak_wcurve wc )
{
  if( wp == NULL ) return ak_error_null_pointer;

  ak_mpzn_set_ui( wp->x, wc->size, 0 );
  ak_mpzn_set_ui( wp->y, wc->size, 1 );
  ak_mpzn_set_ui( wp->z, wc->size, 0 );

 return ak_error_ok;
}

/* ----------------------------------------------------------------------------------------------- */
/*! @param wp точка \f$ P \f$ эллиптической кривой, которой присваивается новое значение.
    @param wq точка \f$ Q \f$ эллиптической кривой, значение которой присваивается.
    @param wc эллиптическая кривая, которой принадлежат обе точки.
    @return Функция возвращает \ref ak_error_ok. В случае, когда один  из контекстов
    равен NULL, то возвращается \ref ak_error_null_pointer.                                        */
/* ----------------------------------------------------------------------------------------------- */
 int ak_wpoint_set_wpoint( ak_wpoint wp, ak_wpoint wq, ak_wcurve wc )
{
  if( wp == NULL ) return ak_error_null_pointer;
  if( wq == NULL ) return ak_error_null_pointer;
  if( wc == NULL ) return ak_error_null_pointer;

  memcpy( wp->x, wq->x, wc->size*sizeof( ak_uint64 ));
  memcpy( wp->y, wq->y, wc->size*sizeof( ak_uint64 ));
  memcpy( wp->z, wq->z, wc->size*sizeof( ak_uint64 ));

 return ak_error_ok;
}


/* ----------------------------------------------------------------------------------------------- */
/*! Для заданной точки \f$ P = (x:y:z) \f$ функция проверяет,
    что точка принадлежит эллиптической кривой, то есть что выполнено сравнение
    \f$ yz^2 \equiv x^3 + axz^2 + bz^3 \pmod{p}\f$.

    @param wp точка \f$ P \f$ эллиптической кривой
    @param ec эллиптическая кривая, на принадлежность которой проверяется точка \f$P\f$.

    @return Функция возвращает \ref ak_true если все проверки выполнены. В противном случае
    возвращается \ref ak_false.                                                                    */
/* ----------------------------------------------------------------------------------------------- */
 ak_bool ak_wpoint_is_ok( ak_wpoint wp, ak_wcurve ec )
{
  ak_mpznmax t, s;
  memset( t, 0, sizeof(ak_uint64)*ak_mpznmax_size );
  memset( s, 0, sizeof(ak_uint64)*ak_mpznmax_size );

 /* Проверяем принадлежность точки заданной кривой */
  ak_mpzn_set( t, ec->a, ec->size );
  ak_mpzn_mul_montgomery( t, t, wp->x, ec->p, ec->n, ec->size );
  ak_mpzn_set( s, ec->b, ec->size );
  ak_mpzn_mul_montgomery( s, s, wp->z, ec->p, ec->n, ec->size );
  ak_mpzn_add_montgomery( t, t, s, ec->p, ec->size ); // теперь в t величина (ax+bz)

  ak_mpzn_set( s, wp->z, ec->size );
  ak_mpzn_mul_montgomery( s, s, s, ec->p, ec->n, ec->size );
  ak_mpzn_mul_montgomery( t, t, s, ec->p, ec->n, ec->size ); // теперь в t величина (ax+bz)z^2

  ak_mpzn_set( s, wp->x, ec->size );
  ak_mpzn_mul_montgomery( s, s, s, ec->p, ec->n, ec->size );
  ak_mpzn_mul_montgomery( s, s, wp->x, ec->p, ec->n, ec->size );
  ak_mpzn_add_montgomery( t, t, s, ec->p, ec->size ); // теперь в t величина x^3 + (ax+bz)z^2

  ak_mpzn_set( s, wp->y, ec->size );
  ak_mpzn_mul_montgomery( s, s, s, ec->p, ec->n, ec->size );
  ak_mpzn_mul_montgomery( s, s, wp->z, ec->p, ec->n, ec->size ); // теперь в s величина x^3 + (ax+bz)z^2

  if( ak_mpzn_cmp( t, s, ec->size )) return ak_false;
 return ak_true;
}

/* ----------------------------------------------------------------------------------------------- */
/*! Точка эллиптической кривой \f$ P = (x:y:z) \f$ заменяется значением \f$ 2P  = (x_3:y_3:z_3)\f$,
    то есть складывается сама с собой (удваивается).
    При вычислениях используются соотношения, основанные на результатах работы
    D.Bernstein, T.Lange, <a href="http://eprint.iacr.org/2007/286">Faster addition and doubling
     on elliptic curves</a>, 2007.

    \code
      XX = X^2
      ZZ = Z^2
      w = a*ZZ+3*XX
      s = 2*Y*Z
      ss = s^2
      sss = s*ss
      R = Y*s
      RR = R^2
      B = (X+R)^2-XX-RR
      h = w^2-2*B
      X3 = h*s
      Y3 = w*(B-h)-2*RR
      Z3 = sss
    \endcode

    @param wp удваиваемая точка \f$ P \f$ эллиптической кривой.
    @param ec эллиптическая кривая, которой принадлежит точка \f$P\f$.                             */
/* ----------------------------------------------------------------------------------------------- */
 void ak_wpoint_double( ak_wpoint wp, ak_wcurve ec )
{
 ak_mpznmax u1, u2, u3, u4, u5, u6, u7;

 if( ak_mpzn_cmp_ui( wp->z, ec->size, 0 ) == ak_true ) return;
 if( ak_mpzn_cmp_ui( wp->y, ec->size, 0 ) == ak_true ) {
   ak_wpoint_set_as_unit( wp, ec );
   return;
 }
 // dbl-2007-bl
 ak_mpzn_mul_montgomery( u1, wp->x, wp->x, ec->p, ec->n, ec->size );
 ak_mpzn_mul_montgomery( u2, wp->z, wp->z, ec->p, ec->n, ec->size );
 ak_mpzn_lshift_montgomery( u4, u1, ec->p, ec->size );
 ak_mpzn_add_montgomery( u4, u4, u1, ec->p, ec->size );
 ak_mpzn_mul_montgomery( u3, u2, ec->a, ec->p, ec->n, ec->size );
 ak_mpzn_add_montgomery( u3, u3, u4, ec->p, ec->size );  // u3 = az^2 + 3x^2
 ak_mpzn_mul_montgomery( u4, wp->y, wp->z, ec->p, ec->n, ec->size );
 ak_mpzn_lshift_montgomery( u4, u4, ec->p, ec->size );   // u4 = 2yz
 ak_mpzn_mul_montgomery( u5, wp->y, u4, ec->p, ec->n, ec->size ); // u5 = 2y^2z
 ak_mpzn_lshift_montgomery( u6, u5, ec->p, ec->size ); // u6 = 2u5
 ak_mpzn_mul_montgomery( u7, u6, wp->x, ec->p, ec->n, ec->size ); // u7 = 8xy^2z
 ak_mpzn_lshift_montgomery( u1, u7, ec->p, ec->size );
 ak_mpzn_sub( u1, ec->p, u1, ec->size );
 ak_mpzn_mul_montgomery( u2, u3, u3, ec->p, ec->n, ec->size );
 ak_mpzn_add_montgomery( u2, u2, u1, ec->p, ec->size );
 ak_mpzn_mul_montgomery( wp->x, u2, u4, ec->p, ec->n, ec->size );
 ak_mpzn_mul_montgomery( u6, u6, u5, ec->p, ec->n, ec->size );
 ak_mpzn_sub( u6, ec->p, u6, ec->size );
 ak_mpzn_sub( u2, ec->p, u2, ec->size );
 ak_mpzn_add_montgomery( u2, u2, u7, ec->p, ec->size );
 ak_mpzn_mul_montgomery( wp->y, u2, u3, ec->p, ec->n, ec->size );
 ak_mpzn_add_montgomery( wp->y, wp->y, u6, ec->p, ec->size );
 ak_mpzn_mul_montgomery( wp->z, u4, u4, ec->p, ec->n, ec->size );
 ak_mpzn_mul_montgomery( wp->z, wp->z, u4, ec->p, ec->n, ec->size );
}

/* ----------------------------------------------------------------------------------------------- */
/*! Для двух заданных точек эллиптической кривой \f$ P = (x_1: y_1: z_1) \f$ и
    \f$ Q = (x_2:y_2:z_2)\f$ вычисляется сумма \f$ P+Q = (x_3:y_3:z_3)\f$,
    которая присваивается точке \f$ P\f$.

    Для вычислений используются соотношения,
    приведенные в работе H.Cohen, A.Miyaji and T.Ono
    <a href=http://link.springer.com/chapter/10.1007/3-540-49649-1_6>Efficient elliptic curve
    exponentiation using mixed coordinates</a>, 1998.

    \code
      Y1Z2 = Y1*Z2
      X1Z2 = X1*Z2
      Z1Z2 = Z1*Z2
      u = Y2*Z1-Y1Z2
      uu = u^2
      v = X2*Z1-X1Z2
      vv = v^2
      vvv = v*vv
      R = vv*X1Z2
      A = uu*Z1Z2-vvv-2*R
      X3 = v*A
      Y3 = u*(R-A)-vvv*Y1Z2
      Z3 = vvv*Z1Z2
    \endcode

    Если в качестве точки \f$ Q \f$ передается точка \f$ P \f$,
    то функция ak_wpoint_add() корректно обрабатывает такую ситуацию и вызывает функцию
    удвоения точки ak_wpoint_double().

    @param wp1 Точка \f$ P \f$, в которую помещается результат операции сложения; первое слагаемое
    @param wp2 Точка \f$ Q \f$, второе слагаемое
    @param ec Эллиптическая кривая, которой принадллежат складываемые точки                        */
/* ----------------------------------------------------------------------------------------------- */
 void ak_wpoint_add( ak_wpoint wp1, ak_wpoint wp2, ak_wcurve ec )
{
  ak_mpznmax u1, u2, u3, u4, u5, u6, u7;

  if( ak_mpzn_cmp_ui( wp2->z, ec->size, 0 ) == ak_true ) return;
  if( ak_mpzn_cmp_ui( wp1->z, ec->size, 0 ) == ak_true ) {
    ak_wpoint_set_wpoint( wp1, wp2, ec );
    return;
  }
  // поскольку удвоение точки с помощью формул сложения дает бесконечно удаленную точку,
  // необходимо выполнить проверку
  ak_mpzn_mul_montgomery( u1, wp1->x, wp2->z, ec->p, ec->n, ec->size );
  ak_mpzn_mul_montgomery( u2, wp2->x, wp1->z, ec->p, ec->n, ec->size );
  if( ak_mpzn_cmp( u1, u2, ec->size ) == 0 ) { // случай совпадения х-координат точки
    ak_mpzn_mul_montgomery( u1, wp1->y, wp2->z, ec->p, ec->n, ec->size );
    ak_mpzn_mul_montgomery( u2, wp2->y, wp1->z, ec->p, ec->n, ec->size );
    if( ak_mpzn_cmp( u1, u2, ec->size ) == 0 ) // случай полного совпадения точек
      ak_wpoint_double( wp1, ec );
     else ak_wpoint_set_as_unit( wp1, ec );
    return;
  }

  //add-1998-cmo-2
  ak_mpzn_mul_montgomery( u1, wp1->x, wp2->z, ec->p, ec->n, ec->size );
  ak_mpzn_mul_montgomery( u2, wp1->y, wp2->z, ec->p, ec->n, ec->size );
  ak_mpzn_sub( u2, ec->p, u2, ec->size );
  ak_mpzn_mul_montgomery( u3, wp1->z, wp2->z, ec->p, ec->n, ec->size );
  ak_mpzn_mul_montgomery( u4, wp2->y, wp1->z, ec->p, ec->n, ec->size );
  ak_mpzn_add_montgomery( u4, u4, u2, ec->p, ec->size );
  ak_mpzn_mul_montgomery( u5, u4, u4, ec->p, ec->n, ec->size );
  ak_mpzn_sub( u7, ec->p, u1, ec->size );
  ak_mpzn_mul_montgomery( wp1->x, wp2->x, wp1->z, ec->p, ec->n, ec->size );
  ak_mpzn_add_montgomery( wp1->x, wp1->x, u7, ec->p, ec->size );
  ak_mpzn_mul_montgomery( u7, wp1->x, wp1->x, ec->p, ec->n, ec->size );
  ak_mpzn_mul_montgomery( u6, u7, wp1->x, ec->p, ec->n, ec->size);
  ak_mpzn_mul_montgomery( u1, u7, u1, ec->p, ec->n, ec->size );
  ak_mpzn_lshift_montgomery( u7, u1, ec->p, ec->size );
  ak_mpzn_add_montgomery( u7, u7, u6, ec->p, ec->size );
  ak_mpzn_sub( u7, ec->p, u7, ec->size );
  ak_mpzn_mul_montgomery( u5, u5, u3, ec->p, ec->n, ec->size );
  ak_mpzn_add_montgomery( u5, u5, u7, ec->p, ec->size );
  ak_mpzn_mul_montgomery( wp1->x, wp1->x, u5, ec->p, ec->n, ec->size );
  ak_mpzn_mul_montgomery( u2, u2, u6, ec->p, ec->n, ec->size );
  ak_mpzn_sub( u5, ec->p, u5, ec->size );
  ak_mpzn_add_montgomery( u1, u1, u5, ec->p, ec->size );
  ak_mpzn_mul_montgomery( wp1->y, u4, u1, ec->p, ec->n, ec->size );
  ak_mpzn_add_montgomery( wp1->y, wp1->y, u2, ec->p, ec->size );
  ak_mpzn_mul_montgomery( wp1->z, u6, u3, ec->p, ec->n, ec->size );
}

/* ----------------------------------------------------------------------------------------------- */
/*! Для точки \f$ P = (x:y:z) \f$ функция вычисляет аффинное представление,
    задаваемое следующим вектором \f$ P = \left( \frac{x}{z} \pmod{p}, \frac{y}{z} \pmod{p}, 1\right) \f$,
    где \f$ p \f$ модуль эллиптической кривой.

    @param wp Точка кривой, которая приводится к аффинной форме
    @param ec Эллиптическая кривая, которой принадлежит точка                                      */
/* ----------------------------------------------------------------------------------------------- */
 void ak_wpoint_reduce( ak_wpoint wp, ak_wcurve ec )
{
 ak_mpznmax u, one = ak_mpznmax_one;
 if( ak_mpzn_cmp_ui( wp->z, ec->size, 0 ) == ak_true ) {
   ak_wpoint_set_as_unit( wp, ec );
   return;
 }

 ak_mpzn_set_ui( u, ec->size, 2 );
 ak_mpzn_sub( u, ec->p, u, ec->size );
 ak_mpzn_modpow_montgomery( u, wp->z, u, ec->p, ec->n, ec->size ); // u <- z^{p-2} (mod p)
 ak_mpzn_mul_montgomery( u, u, one, ec->p, ec->n, ec->size );

 ak_mpzn_mul_montgomery( wp->x, wp->x, u, ec->p, ec->n, ec->size );
 ak_mpzn_mul_montgomery( wp->y, wp->y, u, ec->p, ec->n, ec->size );
 ak_mpzn_set_ui( wp->z, ec->size, 1 );
}

/* ----------------------------------------------------------------------------------------------- */
/*! Для заданной точки \f$ P = (x:y:z) \f$ и заданного целого числа (вычета) \f$ k \f$
    функция вычисляет кратную точку \f$ Q \f$, удовлетворяющую
    равенству \f$  Q = [k]P = \underbrace{P+ \cdots + P}_{k}\f$.

    При вычислении используется метод `лесенки Монтгомери`, выравнивающий время работы алгоритма
    вне зависимости от вида числа \f$ k \f$.

    \b Для \b информации:
     \li Функция не приводит результирующую точку \f$ Q \f$ к аффинной форме.
     \li Исходная точка \f$ P \f$ и результирующая точка \f$ Q \f$ могут совпадать.

    @param wq Точка \f$ Q \f$, в которую помещается результат
    @param wp Точка \f$ P \f$
    @param k Степень кратности
    @param size Размер степени \f$ k \f$ в машинных словах - значение, как правило,
    задаваемое константой \ref ak_mpzn256_size или \ref ak_mpzn512_size. В общем случае
    может приниимать любое неотрицательное значение.
    @param ec Эллиптическая кривая, на которой происходят вычисления                               */
/* ----------------------------------------------------------------------------------------------- */
 void ak_wpoint_pow( ak_wpoint wq, ak_wpoint wp, ak_uint64 *k, size_t size, ak_wcurve ec )
{
  ak_uint64 uk = 0;
  long long int i, j;
  struct wpoint Q, R; /* две точки из лесенки Монтгомери */

 /* начальные значения для переменных */
  ak_wpoint_set_as_unit( &Q, ec );
  ak_wpoint_set_wpoint( &R, wp, ec );

 /* полный цикл по всем(!) битам числа k */
  for( i = size-1; i >= 0; i-- ) {
     uk = k[i];
     for( j = 0; j < 64; j++ ) {
       if( uk&0x8000000000000000LL ) { ak_wpoint_add( &Q, &R, ec ); ak_wpoint_double( &R, ec ); }
        else { ak_wpoint_add( &R, &Q, ec ); ak_wpoint_double( &Q, ec ); }
       uk <<= 1;
     }
  }
 /* копируем полученный результат */
  ak_wpoint_set_wpoint( wq, &Q, ec );
}

/* ----------------------------------------------------------------------------------------------- */
/*! Для заданной точки \f$ P = (x:y:z) \f$ функция проверяет
    что порядок точки действительно есть величина \f$ q \f$, заданная в параметрах
    эллиптической кривой, то есть проверяется выполнимость равенства \f$ [q]P = \mathcal O\f$,
    где \f$ \mathcal O \f$ - бесконечно удаленная точка (ноль группы точек эллиптической кривой),
    а \f$ q \f$ порядок подгруппы, в которой реализуются вычисления.

    @param wp точка \f$ P \f$ эллиптической кривой
    @param ec эллиптическая кривая, на принадлежность которой проверяется точка \f$P\f$.

    @return Функция возвращает \ref ak_true если все проверки выполнены. В противном случае
    возвращается \ref ak_false.                                                                    */
/* ----------------------------------------------------------------------------------------------- */
 ak_bool ak_wpoint_check_order( ak_wpoint wp, ak_wcurve ec )
{
  struct wpoint ep;

  ak_wpoint_set_as_unit( &ep, ec );
  ak_wpoint_pow( &ep, wp, ec->q, ec->size, ec );
  return ak_mpzn_cmp_ui( ep.z, ec->size, 0 );
}

/* ----------------------------------------------------------------------------------------------- */
/*                                                                                    ak_curves.c  */
/* ----------------------------------------------------------------------------------------------- */
