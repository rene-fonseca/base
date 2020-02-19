/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/OwnedNativeString.h>
#include <base/mem/ReferenceCountedAllocator.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

OwnedNativeString::OwnedNativeString() noexcept
{
  static char EMPTY[1] = {'\0'};
  text = EMPTY;
}

OwnedNativeString::OwnedNativeString(const char* text)
{
  if (text) {
    MemorySize length = getNullTerminatedLength(text);
    ReferenceCountedAllocator<char>* buffer = new ReferenceCountedAllocator<char>(length + 1);
    ReferenceCountedObject::ReferenceImpl(*buffer).addReference();
    this->text = buffer->getElements();
    copy(this->text, text, length + 1);
    this->buffer = buffer;
  } else {
    static char EMPTY[1] = {'\0'};
    this->text = EMPTY;
  }
}

OwnedNativeString::OwnedNativeString(const OwnedNativeString& copy) noexcept
{
  text = copy.text;
  buffer = copy.buffer;
  if (buffer) {
    ReferenceCountedObject::ReferenceImpl(*buffer).addReference();
  }
}

OwnedNativeString::~OwnedNativeString()
{
  if (buffer) {
    if (ReferenceCountedObject::ReferenceImpl(*buffer).removeReference()) {
      delete buffer;
    }
  }
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
