#!/bin/sh

if test x$1 = x; then
  echo Platform description missing 1>&2
  exit 1
fi

. $1
  
# architecture (mostly)

case $unsigned_char in yes|no) export base_cv_unsigned_char="$unsigned_char";; esac
echo base_cv_unsigned_char=$base_cv_unsigned_char

case $char_size in 1|2|4|8|16) base_cv_char_size="$char_size";; esac
echo base_cv_char_size=$base_cv_char_size
case $short_size in 1|2|4|8|16) base_cv_short_size="$short_size";; esac
echo base_cv_short_size=$base_cv_short_size
case $int_size in 1|2|4|8|16) base_cv_int_size="$int_size";; esac
echo base_cv_int_size=$base_cv_int_size
case $long_size in 1|2|4|8|16) base_cv_long_size="$long_size";; esac
echo base_cv_long_size=$base_cv_long_size
case $long_long_size in 1|2|4|8|16) base_cv_long_long_size="$long_long_size";; esac
echo base_cv_long_long_size=$base_cv_long_long_size
case $wchar_size in 1|2|4|8|16) base_cv_wchar_size="$wchar_size";; esac
echo base_cv_wchar_size=$base_cv_wchar_size
case $pointer_size in 1|2|4|8|16) base_cv_pointer_size="$pointer_size";; esac
echo base_cv_pointer_size=$base_cv_pointer_size

case $byte_order in LITTLE_ENDIAN|BIG_ENDIAN) base_cv_byte_order="$byte_order";; esac
echo base_cv_byte_order=$base_cv_byte_order
case $word_order in LITTLE_ENDIAN|BIG_ENDIAN) base_cv_word_order="$word_order";; esac
echo base_cv_word_order=$base_cv_word_order

case $float in
IEEE_754_SINGLE_PRECISION| \
IEEE_754_DOUBLE_PRECISION| \
IEEE_EXTENDED_DOUBLE_PRECISION_96| \
IEEE_EXTENDED_DOUBLE_PRECISION_128| \
IEEE_QUADRUPLE_PRECISION)
  base_cv_float=$float
;;
esac
echo base_cv_float=$base_cv_float

case $double in
IEEE_754_SINGLE_PRECISION| \
IEEE_754_DOUBLE_PRECISION| \
IEEE_EXTENDED_DOUBLE_PRECISION_96| \
IEEE_EXTENDED_DOUBLE_PRECISION_128| \
IEEE_QUADRUPLE_PRECISION)
  base_cv_double=$double
;;
esac
echo base_cv_double=$base_cv_double

case $long_double in
IEEE_754_SINGLE_PRECISION| \
IEEE_754_DOUBLE_PRECISION| \
IEEE_EXTENDED_DOUBLE_PRECISION_96| \
IEEE_EXTENDED_DOUBLE_PRECISION_128| \
IEEE_QUADRUPLE_PRECISION)
  export base_cv_long_double=$long_double
;;
esac
echo base_cv_long_double=$base_cv_long_double



# functions
default_available_functions=
default_inavailable_functions="mbrtowc mbsrtowcs memchr memcmp memcpy memmove memset nanosleep pselect pthread_yield readdir_r yield"

for function in $default_available_functions; do
  export base_cv_function_$function=yes
done

for function in $default_inavailable_functions; do
  export base_cv_function_$function=no
done

for function in $functions; do
  case $function in
  [A-Za-z0-9_]*)
    export base_cv_function_$function=yes
  ;;
  *)
    echo Invalid function name 1>&2
    exit 1
  ;;
  esac
done

all_functions="$default_available_functions $default_inavailable_functions"
for function in $all_functions; do
  variable=function_$function
  if test "x${!variable-unset}" != xunset; then
    case ${!variable} in
    yes|no)
      export base_cv_function_$function=${!variable}
    ;;
    *)
      echo Invalid value of $variable x${!variable-unset} 1>&2
      exit 1
    ;;
    esac
  fi
  variable=base_cv_function_$function
  echo base_cv_function_$function=${!variable}
done

# symbols/types: SOCKLEN_T
# TAG: fixme

# sub apis  
default_available_apis=
default_inavailable_apis="aio inet_ipv6 lfs pthread pthread_rwlock pthread_semaphore regexp wide"

for api in $default_available_apis; do
  export base_cv_api_$api=yes
done

for api in $default_inavailable_apis; do
  export base_cv_api_$api=no
done

for api in $apis; do
  case $api in
  [A-Za-z0-9_]*)
    export base_cv_api_$api=yes
  ;;
  *)
    echo Invalid api name 1>&2
    exit 1
  ;;
  esac
done

all_apis="$default_available_apis $default_inavailable_apis"
for api in $all_apis; do
  variable=api_$api
  if test "x${!variable-unset}" != xunset; then
    case ${!variable} in
    yes|no)
      export base_cv_api_$api=${!variable}
    ;;
    *)
      echo Invalid value of $variable x${!variable-unset} 1>&2
      exit 1
    ;;
    esac
  fi
  variable=base_cv_api_$api
  echo base_cv_api_$api=${!variable}
done
