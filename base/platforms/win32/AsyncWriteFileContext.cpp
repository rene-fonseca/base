/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/win32/AsyncWriteFileContext.h>
#include <base/Primitives.h>

#if !(_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  error Inclusion of platform specific source file
#endif

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  #include <windows.h>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace win32 {
  
  void CALLBACK asyncWriteFileCallback(DWORD errorCode, DWORD bytesWritten, OVERLAPPED* overlapped) {
    AsyncWriteFileContext* context = Cast::pointer<AsyncWriteFileContext::CallbackInfo*>(overlapped)->context;
    unsigned int flags = context->flags | AsynchronousWriteCompletion::COMPLETED;
    if (errorCode == 0) {
      flags |= AsynchronousWriteCompletion::SUCCESSFUL;
    } else if (errorCode == ERROR_OPERATION_ABORTED) { // FIXME: is this ok to do
      flags |= AsynchronousWriteCompletion::ABORTED;
    }
    context->flags = flags;
    context->bytesWritten = bytesWritten;
    context->notifyAsynchronousCompletionListener();
    context->selfReference = 0; // release destruction lock (do NOT access state hereafter)
  }

  AsyncWriteFileContext::AsyncWriteFileContext(
    OperatingSystem::Handle handle,
    const uint8* _buffer,
    unsigned int _bytesToWrite,
    unsigned long long _offset,
    AsynchronousWriteEventListener* _listener) throw(IOException) 
    : listener(_listener),
      buffer(_buffer),
      bytesToWrite(_bytesToWrite),
      offset(_offset),
      bytesWritten(0),
      flags(0) {
    clear(callbackInfo.overlapped);
    callbackInfo.overlapped.Offset = getLowWordOf64(offset);
    callbackInfo.overlapped.OffsetHigh = getHighWordOf64(offset);
    callbackInfo.context = this;
    
    BOOL result = ::WriteFileEx(
      handle,
      buffer,
      bytesToWrite,
      &callbackInfo.overlapped,
      asyncWriteFileCallback
    );
    if (!result) {
      //DWORD lastError = ::GetLastError();
      selfReference = 0; // release destruction lock (do NOT access state hereafter)
      throw IOException(this);
    }
  }

  AsyncWriteFileContext::~AsyncWriteFileContext() {
    bassert(
      (flags & AsynchronousWriteCompletion::COMPLETED) != 0,
      AsynchronousException(this)
    );
  }
  
}; // win32 namespace

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
