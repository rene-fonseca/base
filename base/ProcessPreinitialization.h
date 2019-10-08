/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace internal {
  class ProcessInitialization;
};

/**
  This class is responsible for the most basic initialization of the
  process. This class is used internally by the framework.

  @short Basic process initialization.
  @see ProcessInitialization
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API ProcessPreinitialization {
  friend class internal::ProcessInitialization;
private:

  /**
    Initializes the process. The process will be terminated immediately if
    initialization fails.
  */
  ProcessPreinitialization() throw();

  /**
    Releases the fundamental process resources.
  */
  ~ProcessPreinitialization() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
