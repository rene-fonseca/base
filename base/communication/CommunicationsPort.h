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

#include <base/Object.h>
#include <base/communication/CommunicationsException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  @defgroup communications Communications
*/

/**
  This is the base class of all communication ports.
  
  @short A communications port.
  @see Communications port.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API CommunicationsPort : public Object {
public:
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
