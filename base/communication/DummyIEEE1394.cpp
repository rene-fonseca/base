/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/communication/DummyIEEE1394.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

DummyIEEE1394::DummyIEEE1394() throw(IEEE1394Exception) {
}

bool DummyIEEE1394::hasBeenReset() const throw() {
  return false;
}

void DummyIEEE1394::acknowledgeReset() throw() {
}

Array<EUI64> DummyIEEE1394::getAdapters() throw(IEEE1394Exception) {
  return Array<EUI64>();
}

void DummyIEEE1394::open() throw(IEEE1394Exception) {
  throw IEEE1394Exception(this);
}

void DummyIEEE1394::open(const EUI64& adapter) throw(IEEE1394Exception) {
  throw IEEE1394Exception(this);
}

void DummyIEEE1394::close() throw(IEEE1394Exception) {
}

IEEE1394Common::Standard DummyIEEE1394::getCompliance() const throw(IEEE1394Exception) {
  throw IEEE1394Exception(this);
}

unsigned int DummyIEEE1394::getStatus() const throw(IEEE1394Exception) {
  return 0;
}

unsigned int DummyIEEE1394::getFIFOSize() const throw(IEEE1394Exception) {
  return 1024;
}

uint64 DummyIEEE1394::getPresentNodes() const throw(IEEE1394Exception) {
  return 0;
}
  
uint64 DummyIEEE1394::getLinkOnNodes() const throw(IEEE1394Exception) {
  return 0;
}

uint64 DummyIEEE1394::getContenders() const throw(IEEE1394Exception) {
  return 0;
}
  
unsigned int DummyIEEE1394::getSpeedOfNode(unsigned int node) const throw(OutOfDomain, IEEE1394Exception) {
  return 100;
}
  
unsigned int DummyIEEE1394::getMaximumSpeedToNode(unsigned int node) const throw(OutOfDomain, IEEE1394Exception) {
  return 100;
}
  
unsigned int DummyIEEE1394::getBroadcastSpeed() const throw(IEEE1394Exception) {
  return 100;
}

unsigned int DummyIEEE1394::read(unsigned short node, uint64 address, char* buffer, unsigned int size) const throw(OutOfDomain, IOException) {
  throw IOException(this);
}

unsigned int DummyIEEE1394::write(unsigned short node, uint64 address, const char* buffer, unsigned int size) throw(OutOfDomain, IOException) {
  throw IOException(this);
}

unsigned int DummyIEEE1394::read(unsigned short node, uint64 address, uint32* buffer, unsigned int size, uint32 value) const throw(OutOfDomain, IEEE1394Exception) {
  throw IEEE1394Exception(this);
}

IEEE1394Common::IsochronousReadChannel DummyIEEE1394::getReadChannel(unsigned int maxPacketsPerRequest, uint64 subchannels) throw(IEEE1394Exception) {
  throw IEEE1394Exception(this);
}

IEEE1394Common::IsochronousWriteChannel DummyIEEE1394::getWriteChannel(unsigned int maxPacketsPerRequest, uint64 subchannels) throw(IEEE1394Exception) {
  throw IEEE1394Exception(this);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
