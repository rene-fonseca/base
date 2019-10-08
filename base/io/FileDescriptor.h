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

#include <base/io/Stream.h>
#include <base/string/FormatOutputStream.h>
#include <base/mem/Reference.h>
#include <base/io/Handle.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  The FileDescriptor class serves as a general handle to a source or sink of
  bytes within the operatingsystem (e.g. file, socket and pipe). This class is
  normally not used directly by the application.

  @short File descriptor.
  @ingroup io
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API FileDescriptor : public virtual Object, public virtual Stream {
public:

  /** Returns the standard input file descriptor. */
  static FileDescriptor getStandardInput() throw();
  /** Returns the standard output file descriptor. */
  static FileDescriptor getStandardOutput() throw();
  /** Returns the standard error file descriptor. */
  static FileDescriptor getStandardError() throw();

  /**
    Reference counted handle to file descriptor.

    @short File descriptor
    @version 1.0
  */
  class _DK_SDU_MIP__BASE__API Descriptor : public Handle {
    friend class Initialization;
    friend class FileDescriptor;
  private:

    /** Invalid handle. */
    static Handle* invalid;

    /** Initializes descriptor. */
    inline explicit Descriptor(OperatingSystem::Handle handle) throw()
      : Handle(handle) {
    }
    
    /** Releases the resources taken up by the descriptor. */
    ~Descriptor();
  };
protected:

  /** Reference counted handle to file descriptor. */
  Reference<Handle> fd;
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
    Destroys the file descriptor.
  */
  ~FileDescriptor();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
