/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILE_DESCRIPTOR_H
#define _DK_SDU_MIP__BASE_IO__FILE_DESCRIPTOR_H

#include <base/io/Stream.h>
#include <base/string/FormatOutputStream.h>
#include <base/mem/ReferenceCountedObjectPointer.h>
#include <base/OperatingSystem.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  The FileDescriptor class serves as a general handle to a source or sink of bytes within the operatingsystem (e.g. file, socket and pipe). This class is normally not used directly by the application.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class FileDescriptor : public virtual Object, public virtual Stream {
public:

  static FileDescriptor getStandardInput() throw();
  static FileDescriptor getStandardOutput() throw();
  static FileDescriptor getStandardError() throw();
protected:

  /**
    Reference counted handle to file descriptor.

    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class Descriptor : public ReferenceCountedObject {
  private:

    /** Handle to file descriptor. */
    OperatingSystem::Handle handle;
  public:

    /**
      Initializes invalid descriptor.
    */
    inline Descriptor() throw() : handle(-1) {}

    /**
      Initializes descriptor.

      @param handle The handle.
    */
    inline explicit Descriptor(OperatingSystem::Handle h) throw() : handle(h) {}

    /**
      Initializes descriptor from other descriptor.
    */
    inline Descriptor(const Descriptor& copy) throw() : handle(copy.handle) {}

    /**
      Returns the flags of the descriptor.
    */
    int getFlags() const throw(IOException);

    /**
      Sets the flags of the descriptor.
    */
    void setFlags(int flags) throw(IOException);

    /**
      Returns the handle.
    */
    inline OperatingSystem::Handle getHandle() const throw() {return handle;}

    /**
      Sets the non blocking flags.

      @param value The desired state.
    */
    void setNonBlocking(bool value) throw(IOException);

    /**
      Destroys the descriptor.
    */
    ~Descriptor() throw(IOException);
  };

  /** Reference counted handle to file descriptor. */
  ReferenceCountedObjectPointer<Descriptor> fd;
public:

  /**
    Initializes the file descriptor as invalid.
  */
  FileDescriptor() throw();

  /**
    Initializes the file descriptor.

    @param handle Handle to file descriptor.
  */
  FileDescriptor(OperatingSystem::Handle handle) throw();

  /**
    Copy constructor.
  */
  FileDescriptor(const FileDescriptor& copy) throw();

  /**
    Assignment operator.
  */
  FileDescriptor& operator=(const FileDescriptor& eq) throw();

  /**
    Closes the file descriptor.
  */
  void close() throw(IOException);

  /**
    Gets the flags of the file descriptor.
  */
  int getFlags() const throw(IOException);

  /**
    Sets the flags of the file descriptor.
  */
  void setFlags(int flags) throw(IOException);

  /**
    Gets the handle of the file descriptor.
  */
  OperatingSystem::Handle getHandle() const throw();

  /**
    Returns true if the handle is valid.
  */
  bool isValid() const throw();

  /**
    Sets the handle of the file descriptor.
  */
  void setHandle(OperatingSystem::Handle handle) throw();

  /**
    Sets the non-blocking flag of the file descriptor.
  */
  void setNonBlocking(bool value) throw(IOException);

  /**
    Destroy file descriptor.
  */
  ~FileDescriptor() throw(IOException);

  /**
    Writes a string representation of a FileDescriptor object to a format stream.
  */
  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const FileDescriptor& value);
};

/**
  Writes a string representation of a FileDescriptor object to a format stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const FileDescriptor& value);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
