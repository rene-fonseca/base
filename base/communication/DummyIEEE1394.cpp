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

DummyIEEE1394::DummyIEEE1394()
{
}

bool DummyIEEE1394::hasBeenReset() const noexcept
{
  return false;
}

void DummyIEEE1394::acknowledgeReset() noexcept
{
}

void DummyIEEE1394::resetBus()
{
  _throw IEEE1394Exception("Unable to reset bus.", this);
}

Array<EUI64> DummyIEEE1394::getAdapters()
{
  return Array<EUI64>();
}

void DummyIEEE1394::open()
{
  _throw IEEE1394Exception(this);
}

void DummyIEEE1394::open(const EUI64& adapter)
{
  _throw IEEE1394Exception(this);
}

void DummyIEEE1394::close()
{
}

unsigned int DummyIEEE1394::getLocalId() const
{
  _throw IEEE1394Exception(this);
}

unsigned int DummyIEEE1394::getNumberOfNodes() const
{
  _throw IEEE1394Exception(this);
}

unsigned int DummyIEEE1394::getStatus() const
{
  _throw IEEE1394Exception(this);
}

unsigned int DummyIEEE1394::getFIFOSize() const
{
  _throw IEEE1394Exception(this);
}

void DummyIEEE1394::read(
  unsigned short node,
  uint64 address, uint8* buffer,
  unsigned int size)
{
  _throw IEEE1394Exception(this);
}

void DummyIEEE1394::write(
  unsigned short node,
  uint64 address,
  const uint8* buffer,
  unsigned int size)
{
  _throw IEEE1394Exception(this);
}

unsigned int DummyIEEE1394::read(
  unsigned short node,
  uint64 address,
  uint32* buffer,
  unsigned int size,
  uint32 value) {
  _throw IEEE1394Exception(this);
}

IEEE1394Common::IsochronousReadChannel DummyIEEE1394::getReadChannel(
  unsigned int maxPacketsPerRequest,
  uint64 subchannels)
{
  _throw IEEE1394Exception(this);
}

IEEE1394Common::IsochronousWriteChannel DummyIEEE1394::getWriteChannel(
  unsigned int maxPacketsPerRequest,
  uint64 subchannels)
{
  _throw IEEE1394Exception(this);
}

void DummyIEEE1394::readIsochronous(
  unsigned int channel,
  unsigned int maximumPayload,
  IsochronousChannelListener* listener)
{
  _throw IEEE1394Exception(this);
}

bool DummyIEEE1394::wait(
  unsigned int milliseconds)
{
  _throw IEEE1394Exception(this);
}

void DummyIEEE1394::dequeue()
{
  _throw IEEE1394Exception(this);
}
  
void DummyIEEE1394::registerFCPListener(
  FunctionControlProtocolListener* listener)
{
  _throw IEEE1394Exception(this);
}

void DummyIEEE1394::unregisterFCPListener()
{
  _throw IEEE1394Exception(this);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
