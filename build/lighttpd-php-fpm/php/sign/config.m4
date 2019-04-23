PHP_ARG_ENABLE(sign, whether to enable sign support,
[  --enable-sign      Enable sign support])

if test "$PHP_SIGN" != "no"; then
  PHP_NEW_EXTENSION(sign, sign.c, $ext_shared)
fi
