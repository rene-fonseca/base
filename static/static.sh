#!/bin/sh

debug=yes

if test -z "$1"; then
  echo Platform description missing 1>&2
  exit 1
fi

. $1

flavor=${flavor-unix}
case $flavor in unix) base_cv_flavor=UNIX;; win32) base_cv_flavor=WIN32;; *) exit 1;; esac
echo base_cv_flavor=$base_cv_flavor

# architecture
case $arch in [A-Z0-9_]*) base_cv_arch=$arch;; *) exit 1;; esac
echo base_cv_arch=$base_cv_arch
case $arch_minor in [A-Z0-9_]*) base_cv_arch_minor=$arch_minor;; *) exit 1;; esac
echo base_cv_arch_minor=$base_cv_arch_minor

case $unsigned_char in yes|no) base_cv_unsigned_char=$unsigned_char;; *) exit 1;; esac
echo base_cv_unsigned_char=$base_cv_unsigned_char

case $char_size in 1|2|4|8|16) base_cv_char_size=$char_size;; *) exit 1;; esac
echo base_cv_char_size=$base_cv_char_size
case $short_size in 1|2|4|8|16) base_cv_short_size=$short_size;; *) exit 1;; esac
echo base_cv_short_size=$base_cv_short_size
case $int_size in 1|2|4|8|16) base_cv_int_size=$int_size;; *) exit 1;; esac
echo base_cv_int_size=$base_cv_int_size
case $long_size in 1|2|4|8|16) base_cv_long_size=$long_size;; *) exit 1;; esac
echo base_cv_long_size=$base_cv_long_size
case $long_long_size in 1|2|4|8|16) base_cv_long_long_size=$long_long_size;; *) exit 1;; esac
echo base_cv_long_long_size=$base_cv_long_long_size
case $wchar_size in 1|2|4|8|16) base_cv_wchar_size=$wchar_size;; *) exit 1;; esac
echo base_cv_wchar_size=$base_cv_wchar_size
case $pointer_size in 1|2|4|8|16) base_cv_pointer_size=$pointer_size;; *) exit 1;; esac
echo base_cv_pointer_size=$base_cv_pointer_size

case $byte_order in LITTLE_ENDIAN|BIG_ENDIAN) base_cv_byte_order=$byte_order;; *) exit 1;; esac
echo base_cv_byte_order=$base_cv_byte_order
case $word_order in LITTLE_ENDIAN|BIG_ENDIAN) base_cv_word_order=$word_order;; *) exit 1;; esac
echo base_cv_word_order=$base_cv_word_order

case $float in
IEEE_754_SINGLE_PRECISION| \
IEEE_754_DOUBLE_PRECISION| \
IEEE_EXTENDED_DOUBLE_PRECISION_96| \
IEEE_EXTENDED_DOUBLE_PRECISION_128| \
IEEE_QUADRUPLE_PRECISION)
  base_cv_float=$float
;;
*)
  exit 1
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
*)
  exit 1
;;
esac
echo base_cv_double=$base_cv_double

case $long_double in
IEEE_754_SINGLE_PRECISION| \
IEEE_754_DOUBLE_PRECISION| \
IEEE_EXTENDED_DOUBLE_PRECISION_96| \
IEEE_EXTENDED_DOUBLE_PRECISION_128| \
IEEE_QUADRUPLE_PRECISION)
  base_cv_long_double=$long_double
;;
*)
  exit 1
;;
esac
echo base_cv_long_double=$base_cv_long_double



# os
os=${os-UNSPECIFIED}
case $os in [A-Z]*) base_cv_os=$os;; *) exit 1;; esac
echo base_cv_os=$base_cv_os

os_minor=${os_minor-0}
case $os_minor in [0-9]*) base_cv_os_minor=$os_minor;; *) exit 1;; esac
echo base_cv_os_minor=$base_cv_os_minor

# TAG: need to check if valid syntax
echo base_cv_features=\""$features"\"



# functions
base_cv_function_mbrtowc=no
base_cv_function_mbsrtowcs=no
base_cv_function_memchr=no
base_cv_function_memcmp=no
base_cv_function_memcpy=no
base_cv_function_memmove=no
base_cv_function_memset=no
base_cv_function_nanosleep=no
base_cv_function_pselect=no
base_cv_function_pthread_yield=no
base_cv_function_readdir_r=no
base_cv_function_yield=no

case ${function_mbrtowc+S}${function_mbrtowc-U} in
Syes|Sno) base_cv_function_mbrtowc=$function_mbrtowc;; U);; *) exit 1;; esac
case ${function_mbsrtowcs+S}${function_mbsrtowcs-U} in
Syes|Sno) base_cv_function_mbsrtowcs=$function_mbsrtowcs;; U);; *) exit 1;; esac
case ${function_memchr+S}${function_memchr-U} in
Syes|Sno) base_cv_function_memchr=$function_memchr;; U);; *) exit 1;; esac
case ${function_memcmp+S}${function_memcmp-U} in
Syes|Sno) base_cv_function_memcmp=$function_memcmp;; U);; *) exit 1;; esac
case ${function_memcpy+S}${function_memcpy-U} in
Syes|Sno) base_cv_function_memcpy=$function_memcpy;; U);; *) exit 1;; esac
case ${function_memmove+S}${function_memmove-U} in
Syes|Sno) base_cv_function_memmove=$function_memmove;; U);; *) exit 1;; esac
case ${function_memset+S}${function_memset-U} in
Syes|Sno) base_cv_function_memset=$function_memset;; U);; *) exit 1;; esac
case ${function_nanosleep+S}${function_nanosleep-U} in
Syes|Sno) base_cv_function_nanosleep=$function_nanosleep;; U);; *) exit 1;; esac
case ${function_pselect+S}${function_pselect-U} in
Syes|Sno) base_cv_function_pselect=$function_pselect;; U);; *) exit 1;; esac
case ${function_pthread_yield+S}${function_pthread_yield-U} in
Syes|Sno) base_cv_function_pthread_yield=$function_pthread_yield;; U);; *) exit 1;; esac
case ${function_readdir_r+S}${function_readdir_r-U} in
Syes|Sno) base_cv_function_readdir_r=$function_readdir_r;; U);; *) exit 1;; esac
case ${function_yield+S}${function_yield-U} in
Syes|Sno) base_cv_function_yield=$function_yield;; U);; *) exit 1;; esac

for function in ${functions-""}; do
  case $function in
  mbrtowc) base_cv_function_mbrtowc=yes;;
  mbsrtowcs) base_cv_function_mbsrtowcs=yes;;
  memchr) base_cv_function_memchr=yes;;
  memcmp) base_cv_function_memcmp=yes;;
  memcpy) base_cv_function_memcpy=yes;;
  memmove) base_cv_function_memmove=yes;;
  memset) base_cv_function_memset=yes;;
  nanosleep) base_cv_function_nanosleep=yes;;
  pselect) base_cv_function_pselect=yes;;
  pthread_yield) base_cv_function_pthread_yield=yes;;
  readdir_r) base_cv_function_readdir_r=yes;;
  yield) base_cv_function_yield=yes;;
  esac
done

echo base_cv_function_mbrtowc=$base_cv_function_mbrtowc
echo base_cv_function_mbsrtowcs=$base_cv_function_mbsrtowcs
echo base_cv_function_memchr=$base_cv_function_memchr
echo base_cv_function_memcmp=$base_cv_function_memcmp
echo base_cv_function_memcpy=$base_cv_function_memcpy
echo base_cv_function_memmove=$base_cv_function_memmove
echo base_cv_function_memset=$base_cv_function_memset
echo base_cv_function_nanosleep=$base_cv_function_nanosleep
echo base_cv_function_pselect=$base_cv_function_pselect
echo base_cv_function_pthread_yield=$base_cv_function_pthread_yield
echo base_cv_function_readdir_r=$base_cv_function_readdir_r
echo base_cv_function_yield=$base_cv_function_yield



# symbols/types: SOCKLEN_T

# sub apis  
base_cv_api_aio=no
base_cv_api_inet_ipv6=no
base_cv_api_lfs=no
base_cv_api_pthread=no
base_cv_api_pthread_rwlock=no
base_cv_api_pthread_semaphore=no
base_cv_api_regexp=no
base_cv_api_wide=no

case ${api_aio+S}${api_aio-U} in
Syes|Sno) base_cv_api_aio=$api_aio;; U);; *) exit 1;; esac
case ${api_inet_ipv6+S}${api_inet_ipv6-U} in
Syes|Sno) base_cv_api_inet_ipv6=$api_inet_ipv6;; U);; *) exit 1;; esac
case ${api_lfs+S}${api_lfs-U} in
Syes|Sno) base_cv_api_lfs=$api_lfs;; U);; *) exit 1;; esac
case ${api_pthread+S}${api_pthread-U} in
Syes|Sno) base_cv_api_pthread=$api_pthread;; U);; *) exit 1;; esac
case ${api_pthread_rwlock+S}${api_pthread_rwlock-U} in
Syes|Sno) base_cv_api_pthread_rwlock=$api_pthread_rwlock;; U);; *) exit 1;; esac
case ${api_pthread_semaphore+S}${api_pthread_semaphore-U} in
Syes|Sno) base_cv_api_pthread_semaphore=$api_pthread_semaphore;; U);; *) exit 1;; esac
case ${api_regexp+S}${api_regexp-U} in
Syes|Sno) base_cv_api_regexp=$api_regexp;; U);; *) exit 1;; esac
case ${api_wide+S}${api_wide-U} in
Syes|Sno) base_cv_api_wide=$api_wide;; U);; *) exit 1;; esac

for api in ${apis-""}; do
  case $api in
  aio) base_cv_api_aio=yes;;
  inet_ipv6) base_cv_api_inet_ipv6=yes;;
  lfs) base_cv_api_lfs=yes;;
  pthread) base_cv_api_pthread=yes;;
  pthread_rwlock) base_cv_api_pthread_rwlock=yes;;
  pthread_semaphore) base_cv_api_pthread_semaphore=yes;;
  regexp) base_cv_api_regexp=yes;;
  wide) base_cv_api_wide=yes;;
  esac
done

echo base_cv_api_aio=$base_cv_api_aio
echo base_cv_api_inet_ipv6=$base_cv_api_inet_ipv6
echo base_cv_api_lfs=$base_cv_api_lfs
echo base_cv_api_pthread=$base_cv_api_pthread
echo base_cv_api_pthread_rwlock=$base_cv_api_pthread_rwlock
echo base_cv_api_pthread_semaphore=$base_cv_api_pthread_semaphore
echo base_cv_api_regexp=$base_cv_api_regexp
echo base_cv_api_wide=$base_cv_api_wide



# TAG: need to check if valid syntax
echo base_cv_libraries=\""$libraries"\"
