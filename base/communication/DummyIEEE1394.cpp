/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/communication/DummyIEEE1394.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

DummyIEEE1394::DummyIEEE1394() throw(IEEE1394Exception)
{
}

bool DummyIEEE1394::hasBeenReset() const noexcept
{
  return false;
}

void DummyIEEE1394::acknowledgeReset() noexcept
{
}

void DummyIEEE1394::resetBus() throw(IEEE1394Exception)
{
  throw IEEE1394Exception("Unable to reset bus.", this);
}

Array<EUI64> DummyIEEE1394::getAdapters() throw(IEEE1394Exception)
{
  return Array<EUI64>();
}

void DummyIEEE1394::open() throw(IEEE1394Exception)
{
  throw IEEE1394Exception(this);
}

void DummyIEEE1394::open(const EUI64& adapter) throw(IEEE1394Exception)
{
  throw IEEE1394Exception(this);
}

void DummyIEEE1394::close() throw(IEEE1394Exception)
{
}

unsigned int DummyIEEE1394::getLocalId() const noexcept
{
  throw IEEE1394Exception(this);
}

unsigned int DummyIEEE1394::getNumberOfNodes() const noexcept
{
  throw IEEE1394Exception(this);
}

unsigned int DummyIEEE1394::getStatus() const throw(IEEE1394Exception)
{
  throw IEEE1394Exception(this);
}

unsigned int DummyIEEE1394::getFIFOSize() const throw(IEEE1394Exception)
{
  throw IEEE1394Exception(this);
}

void DummyIEEE1394::read(
  unsigned short node,
  uint64 address, uint8* buffer,
  unsigned int size) throw(IEEE1394Exception)
{
  throw IEEE1394Exception(this);
}

void DummyIEEE1394::write(
  unsigned short node,
  uint64 address,
  const uint8* buffer,
  unsigned int size) throw(IEEE1394Exception)
{
  throw IEEE1394Exception(this);
}

unsigned int DummyIEEE1394::read(
  unsigned short node,
  uint64 address,
  uint32* buffer,
  unsigned int size,
  uint32 value) throw(IEEE1394Exception) {
  throw IEEE1394Exception(this);
}

IEEE1394Common::IsochronousReadChannel DummyIEEE1394::getReadChannel(
  unsigned int maxPacketsPerRequest,
  uint64 subchannels) throw(IEEE1394Exception)
{
  throw IEEE1394Exception(this);
}

IEEE1394Common::IsochronousWriteChannel DummyIEEE1394::getWriteChannel(
  unsigned int maxPacketsPerRequest,
  uint64 subchannels) throw(IEEE1394Exception)
{
  throw IEEE1394Exception(this);
}

void DummyIEEE1394::readIsochronous(
  unsigned int channel,
  unsigned int maximumPayload,
  IsochronousChannelListener* listener) throw(OutOfDomain, IEEE1394Exception)
{
  throw IEEE1394Exception(this);
}

bool DummyIEEE1394::wait(
  unsigned int milliseconds) throw(OutOfDomain, IEEE1394Exception)
{
  throw IEEE1394Exception(this);
}

void DummyIEEE1394::dequeue() throw(IEEE1394Exception)
{
  throw IEEE1394Exception(this);
}
  
void DummyIEEE1394::registerFCPListener(
  FunctionControlProtocolListener* listener) throw(IEEE1394Exception)
{
  throw IEEE1394Exception(this);
}

void DummyIEEE1394::unregisterFCPListener() throw(IEEE1394Exception)
{
  throw IEEE1394Exception(this);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
