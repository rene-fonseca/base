/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__FEATURES_H
#define _DK_SDU_MIP__BASE__FEATURES_H

// Temporary fix. config.h should also be installed
#include "../../config.h"

#if defined(_DK_SDU_MIP__BASE__NAMESPACE)
  #define _DK_SDU_MIP__BASE__ENTER_NAMESPACE namespace base {
  #define _DK_SDU_MIP__BASE__LEAVE_NAMESPACE }
#else
  #define _DK_SDU_MIP__BASE__ENTER_NAMESPACE
  #define _DK_SDU_MIP__BASE__LEAVE_NAMESPACE
#endif

#if defined(_DK_SDU_MIP__BASE__TRACE)
  #define TRACE(text) {Trace::message(text);}
  #define TRACE_MEMBER() {Trace::member(this, __PRETTY_FUNCTION__);}
#else
  #define TRACE(text)
  #define TRACE_MEMBER()
#endif

#endif
