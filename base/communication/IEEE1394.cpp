/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/communication/IEEE1394.h>
#include <base/string/StringOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

IEEE1394::GloballyUniqueIdentifier::GloballyUniqueIdentifier() throw() {
  clear(id);
}

IEEE1394::GloballyUniqueIdentifier::GloballyUniqueIdentifier(const unsigned char value[8]) throw() {
  copy<unsigned char>(id, value, sizeof(id));
}

IEEE1394::GloballyUniqueIdentifier::GloballyUniqueIdentifier(const GloballyUniqueIdentifier& _copy) throw() {
  copy<unsigned char>(id, _copy.id, sizeof(id));
}

IEEE1394::GloballyUniqueIdentifier& IEEE1394::GloballyUniqueIdentifier::operator=(const GloballyUniqueIdentifier& eq) throw() {
  if (&eq != this) {
    copy<unsigned char>(id, eq.id, sizeof(id));
  }
  return *this;
}



void IEEE1394::IsochronousRequestImpl::setOptions(unsigned int options) throw(IEEE1394Exception) {
  assert(status == READY, IEEE1394Exception("Request not ready", this));
  this->options = options;
}

void IEEE1394::IsochronousRequestImpl::reset() throw(IEEE1394Exception) {
  assert(status != PENDING, IEEE1394Exception("Request is pending", this));
  resetContext();
  status = READY;
}

IEEE1394::IsochronousReadRequestImpl::IsochronousReadRequestImpl() throw()
  : subchannel(0),
    buffer(0),
    bufferSize(0),
    receivedPackets(0) {
  // TAG: subchannel must be reserved - at least one reserved channel
}

void IEEE1394::IsochronousReadRequestImpl::setSubchannel(unsigned int subchannel) throw(OutOfDomain, IEEE1394Exception) {
  assert(getStatus() == READY, IEEE1394Exception("Request not ready", this));
  assert(subchannel <= 63, OutOfDomain(this));
  this->subchannel = subchannel;
}

void IEEE1394::IsochronousReadRequestImpl::setBuffer(char* buffer, unsigned int size) throw(IEEE1394Exception) {
  assert(getStatus() == READY, IEEE1394Exception("Request not ready", this));
  this->buffer = buffer;
  this->bufferSize = size;
}

void IEEE1394::IsochronousReadFixedDataRequestImpl::setBuffer(char* buffer, unsigned int size, char* secondaryBuffer) throw(IEEE1394Exception) {
  IsochronousReadRequestImpl::setBuffer(buffer, size);
  this->secondaryBuffer = secondaryBuffer;
}

IEEE1394::IsochronousWriteRequestImpl::IsochronousWriteRequestImpl() throw()
  : buffer(0),
    bufferSize(0),
    numberOfPackets(0),
    speed(100),
    transmittedPackets(0) {
}
 
void IEEE1394::IsochronousWriteRequestImpl::setBuffer(const char* buffer, unsigned int size, unsigned int numberOfPackets) throw(IEEE1394Exception) {
  assert(getStatus() == READY, IEEE1394Exception("Request not ready", this));
  this->buffer = buffer;
  this->bufferSize = size;
  this->numberOfPackets = numberOfPackets;
}

void IEEE1394::IsochronousWriteRequestImpl::setSpeed(unsigned int speed) throw(OutOfDomain, IEEE1394Exception) {
  assert(getStatus() == READY, IEEE1394Exception("Request not ready", this));
  switch (speed) {
  case 100:
  case 200:
  case 400:
  case 800:
  case 1600:
  case 3200:
    this->speed = speed;
    break;
  default:
    throw OutOfDomain(this);
  }
}

void IEEE1394::IsochronousWriteDataRequestImpl::setBuffer(const char* buffer, unsigned int size, unsigned int numberOfPackets, const char* secondaryBuffer) throw(IEEE1394Exception) {
  IsochronousWriteRequestImpl::setBuffer(buffer, size, numberOfPackets);
  this->secondaryBuffer = secondaryBuffer;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const IEEE1394::GloballyUniqueIdentifier& value) throw(IOException) {
  StringOutputStream s;
  const unsigned char* id = value.getBytes();
  s << id[0] << ':' << id[1] << ':' << id[2] << ':' << id[3] << ':'
    << id[4] << ':' << id[5] << ':' << id[6] << ':' << id[7] << FLUSH;
  return stream << s.getString();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
