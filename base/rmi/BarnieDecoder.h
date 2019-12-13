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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class _COM_AZURE_DEV__BASE__API BarnieDecoder : public OrbDecoder {
public:
  
  /**
    Decodes the elements of an array of unsigned int.
  */
  void read(unsigned int* buffer, unsigned int size);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
