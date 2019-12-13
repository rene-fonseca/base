/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/DateSkeleton.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// on connection between stub and skeleton:
//   stub: get ids from the skeleton

#if 0

DateSkeleton::DateSkeleton() {
  OrbInterface interface(MESSAGE("DateSkeleton/1.0"), 1, 0);
  OrbMethod method(MESSAGE("getDate"), OrbPrimitive(OrbPrimitive::LONG_LONG_INTEGER));
  method.addArgument();
  method.addRaises();
  interface.addMethod(method);
}

// TAG: need support for runtime checking for availability of method/interface/any type really
// Stub::hasMethod(const String& name));
// Stub::importMethod(const String& name));

void DateSkeleton::dispatch(OrbChannel* channel, unsigned int method) {
  // TAG: the component invoking dispatch could handle OrbException
  switch (method) {
  case DateSkeleton::METHOD_GET_DATE:
    {
      // read arguments
      long long result = 0;
      try {
        result = getDate();
        // catch any exceptions specified in the idl document for the method
      } catch (OrbException& e) {
        // send response: server side exception was raised
        OrbExceptionResponse response(e);
      }
      OrbResponse response(OrbPrimitive::LONG_LONG_INTEGER);
      channel->writeResponse(response);
    }
    break;
  default:
    OrbExceptionResponse response(OrbException::INVALID_METHOD, "Invocation of invalid method");
    channel->writeResponse(response);
  }
}

virtual long long DateSkeleton::getDate() const = 0;

DateSkeleton::~DateSkeleton() noexcept {
}

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
