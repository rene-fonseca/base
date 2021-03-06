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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  The FileDescriptor class serves as a general handle to a source or sink of
  bytes within the operatingsystem (e.g. file, socket and pipe). This class is
  normally not used directly by the application.

  @short File descriptor.
  @ingroup io
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API FileDescriptor : public virtual Object, public virtual Stream {
public:

  /** Returns the standard input file descriptor. */
  static FileDescriptor getStandardInput() noexcept;
  /** Returns the standard output file descriptor. */
  static FileDescriptor getStandardOutput() noexcept;
  /** Returns the standard error file descriptor. */
  static FileDescriptor getStandardError() noexcept;

  /**
    Reference counted handle to file descriptor.

    @short File descriptor
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API Descriptor : public Handle {
    friend class Initialization;
    friend class FileDescriptor;
  private:

    /** Invalid handle. */
    static Handle* invalid;

    /** Initializes descriptor. */
    inline explicit Descriptor(OperatingSystem::Handle handle) noexcept
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
  FileDescriptor() noexcept;

  /**
    Initializes the file descriptor.

    @param handle Handle to file descriptor.
  */
  FileDescriptor(OperatingSystem::Handle handle) noexcept;

  /**
    Copy constructor.
  */
  FileDescriptor(const FileDescriptor& copy) noexcept;

  /**
    Assignment operator.
  */
  FileDescriptor& operator=(const FileDescriptor& assign) noexcept;

  /**
    Closes the file descriptor.
  */
  void close();

  /** Returns true if the handle is an ANSI terminal. */
  bool isANSITerminal() const noexcept;

  /** Returns true if the handle is a terminal. */
  bool isTerminal() const noexcept;

  /** Returns true if the file descriptor is a pipe. */
  bool isPipe() const noexcept;

  /**
    Gets the flags of the file descriptor.
  */
  int getFlags() const;

  /**
    Sets the flags of the file descriptor.
  */
  void setFlags(int flags);

  /**
    Gets the handle of the file descriptor.
  */
  OperatingSystem::Handle getHandle() const noexcept;

  /**
    Returns true if the handle is valid.
  */
  bool isValid() const noexcept;

  /**
    Sets the handle of the file descriptor.
  */
  void setHandle(OperatingSystem::Handle handle) noexcept;

  /**
    Sets the non-blocking flag of the file descriptor.
  */
  void setNonBlocking(bool value);

  /**
    Destroys the file descriptor.
  */
  ~FileDescriptor();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
