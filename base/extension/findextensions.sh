#!/bin/sh

LANGUAGE_FILTER=C++
FLAVOR_FILTER=win32

echo Importing extensions

echo "Generated at `date --iso-8601=seconds`" > extension.log
echo >> extension.log
echo "Looking for language: $LANGUAGE_FILTER" >> extension.log
echo "Looking for flavor: $FLAVOR_FILTER" >> extension.log
echo >> extension.log

echo "/* This is an automatically generated file. Do NOT edit. */" > extension.h
echo >> extension.h

echo "# This is an automatically generated file. Do NOT edit." > extension.am
echo >> extension.am

# TAG: $1 could contain spaces
SEARCHDIR=$1
if test x"$SEARCHDIR" = x; then
  SEARCHDIR=.
fi

for extension in `(cd $SEARCHDIR; find . -name "*.ext")`; do
  EXTFILE=`echo $extension | sed 's/^\.\///'`
  echo "Checking extension description: $EXTFILE"
  
  DIR=`echo $EXTFILE | sed 's/^\(.*\)\/.*\.ext$/\1/'`
  
  EXTPATH="$SEARCHDIR/$EXTFILE"
  NAME=`cat $EXTPATH | sed -n 's/^NAME[ ]*:[ ]*\([A-Za-z0-9_]\+\)$/\1/p'`
  AUTHOR=`cat $EXTPATH | sed -n 's/^AUTHOR[ ]*:[ ]*\(.*\)$/\1/p'`
  DOMAIN=`cat $EXTPATH | sed -n 's/^DOMAIN[ ]*:[ ]*\([A-Za-z0-9.]\+\)$/\1/p'`
  COMPONENT=`cat $EXTPATH | sed -n 's/^COMPONENT[ ]*:[ ]*\(.*\)$/\1/p'`
  DESCRIPTION=`cat $EXTPATH | sed -n 's/^DESCRIPTION[ ]*:[ ]*\(.*\)$/\1/p'`
  LANGUAGE=`cat $EXTPATH | sed -n 's/^LANGUAGE[ ]*:[ ]*\(.*\)$/\1/p'`
  FLAVOR=`cat $EXTPATH | sed -n 's/^FLAVOR[ ]*:[ ]*\([a-z0-9]*\)$/\1/p'`
  ATTENTION=`cat $EXTPATH | sed -n 's/^ATTENTION[ ]*:[ ]*\(_[A-Z_]\+\)$/\1/p'`
  
  if test "x$NAME" = x; then
    echo Invalid NAME field
    exit 1
  fi
  
  if test "x$DOMAIN" = x; then
    echo Invalid DOMAIN field
    exit 1
  fi
  
  if test "x$LANGUAGE" = x; then
    echo Invalid LANGUAGE field
    exit 1
  fi
  
  if test "x$FLAVOR" = x; then
    echo Invalid FLAVOR field
    exit 1
  fi
  
  if test "x$ATTENTION" = x; then
    echo Invalid ATTENTION field
    exit 1
  fi  
  
  echo "Found extension $NAME in $DIR" >> extension.log
  echo "  Author: $AUTHOR" >> extension.log
  echo "  Domain: $DOMAIN" >> extension.log
  echo "  Component: $COMPONENT" >> extension.log
  echo "  Description: $DESCRIPTION" >> extension.log
  echo "  Language: $LANGUAGE" >> extension.log
  echo "  Flavor = $FLAVOR" >> extension.log
  echo "  Attention = $ATTENTION" >> extension.log
  
  if test "x$FLAVOR" = "x$FLAVOR_FILTER"; then
    if test "x$LANGUAGE" = "x$LANGUAGE_FILTER"; then
      echo "  Install directory: base/extension/$DIR" >> extension.log
      echo "  Source: $DIR/$NAME.cpp" >> extension.log
      echo "  Header: $DIR/$NAME.h" >> extension.log
      echo "  Use extension by include <base/extension/$DIR/$NAME.h>" >> extension.log
      
      echo "#define $ATTENTION" >> extension.h

      echo "libextension_la_SOURCES += $DIR/$NAME.cpp" >> extension.am
      echo "nobase_section_HEADERS += $DIR/$NAME.h" >> extension.am
    fi
  else
    echo "  Install directory: base/extension/$DIR" >> extension.log
  fi
  
  echo >> extension.log
done
