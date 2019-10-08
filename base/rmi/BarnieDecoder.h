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

#include <base/rmi/OrbDecoder.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class _DK_SDU_MIP__BASE__API BarnieDecoder : public OrbDecoder {
public:
  
  /**
    Decodes the elements of an array of unsigned int.
  */
  void read(unsigned int* buffer, unsigned int size) throw(IOException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
