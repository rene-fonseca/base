/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/Trace.h>
#include <stdio.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void Trace::message(const char* message) {
  printf("TRACE %s\n", message); // printf must be MT-safe
}

void Trace::member(void* ptr, const char* message) {
  printf("TRACE %p >> %s\n", ptr, message); // printf must be MT-safe
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
