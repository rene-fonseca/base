/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

/*
  This program compiles/links/runs if the regex interface is available.
*/

#include <sys/types.h>
#include <regex.h>

int main() {
  regex_t regularExpression;
  regmatch_t pmatch[8];
  char errorStr[256];

  regcomp(&regularExpression, "[a-z]*", REG_EXTENDED|REG_ICASE|REG_NOSUB|REG_NEWLINE);
  int result = regexec(&regularExpression, "identifier", 8, pmatch, REG_NOTBOL|REG_NOTEOL);
  regerror(result, &regularExpression, errorStr, 256);
  regfree(&regularExpression);

  return 0;
}
