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
#include <base/string/String.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  HTTPS request.
*/

class _COM_AZURE_DEV__BASE__API HTTPSRequest : public Object {
public:

  /**
    Initializes the request.
  */
  HTTPSRequest();
  
  // set header
  
  bool open(const String& url);
  
  void send(const String& body);
  
  void close();
  
  // get response
  // responseText
  
  ~HTTPSRequest();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
