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
    copy(buffer->getElements(), text, length + 1);
    this->text = buffer->getElements();
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

OwnedNativeString::OwnedNativeString(OwnedNativeString&& move) noexcept
{
  text = move.text;
  buffer = move.buffer;
  move.buffer = nullptr;
}

OwnedNativeString& OwnedNativeString::operator=(const char* text) noexcept
{
  this->text = nullptr;
  if (buffer) {
    if (ReferenceCountedObject::ReferenceImpl(*buffer).removeReference()) {
      delete buffer;
      buffer = nullptr;
    }
  }

  if (text) {
    MemorySize length = getNullTerminatedLength(text);
    ReferenceCountedAllocator<char>* buffer = new ReferenceCountedAllocator<char>(length + 1);
    ReferenceCountedObject::ReferenceImpl(*buffer).addReference();
    copy(buffer->getElements(), text, length + 1);
    this->text = buffer->getElements();
    this->buffer = buffer;
  } else {
    static char EMPTY[1] = {'\0'};
    this->text = EMPTY;
  }
  return *this;
}

OwnedNativeString& OwnedNativeString::operator=(const OwnedNativeString& copy) noexcept
{
  if (&copy != this) {
    text = copy.text;
    buffer = copy.buffer;
    if (buffer) {
      ReferenceCountedObject::ReferenceImpl(*buffer).addReference();
    }
  }
  return *this;
}

OwnedNativeString& OwnedNativeString::operator=(OwnedNativeString&& move) noexcept
{
  if (&move != this) {
    text = move.text;
    buffer = move.buffer;
    move.text = nullptr;
    move.buffer = nullptr;
  }
  return *this;
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
