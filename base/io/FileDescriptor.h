/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILE_DESCRIPTOR_H
#define _DK_SDU_MIP__BASE_IO__FILE_DESCRIPTOR_H

#include <base/io/Stream.h>
#include <base/string/FormatOutputStream.h>
#include <base/mem/ReferenceCountedObjectPointer.h>

/**
  The FileDescriptor class serves as a general handle to a source or sink of bytes within the operatingsystem (e.g. file, socket and pipe). This class is normally not used directly by the application.

  @author René Møller Fonseca
  @version 1.0
*/

class FileDescriptor : public virtual Object, public virtual Stream {
protected:

  /** Reference counted handle to file descriptor. */
  class Descriptor : public ReferenceCountedObject {
  private:

    /** Handle to file descriptor. */
    int handle;
  public:
    Descriptor() throw();
    Descriptor(int handle) throw();
    void close() throw(IOException);
    int getFlags() const throw(IOException);
    void setFlags(int flags) throw(IOException);
    inline int getHandle() const throw() {return handle;}
    inline void setHandle(int handle) throw() {this->handle = handle;}
    void setNonBlocking(bool value) throw(IOException);
    ~Descriptor() throw(IOException);
  };

  /** Reference counted handle to file descriptor. */
  ReferenceCountedObjectPointer<Descriptor> fd;
public:

  /**
    Initializes the file descriptor.

    @param handle Handle to file descriptor. Default is the invalid handle '-1'.
  */
  FileDescriptor(int handle = -1) throw();

  /**
    Copy constructor.
  */
  FileDescriptor(const FileDescriptor& copy) throw();

  /**
    Assignment operator.
  */
  FileDescriptor& operator=(FileDescriptor& eq) throw();

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
  int getHandle() const throw();

  /**
    Sets the handle of the file descriptor.
  */
  void setHandle(int handle) throw();

  /**
    Sets the non-blocking flag of the file descriptor.
  */
  void setNonBlocking(bool value) throw(IOException);

  /**
    Writes a string representation of a FileDescriptor object to a format stream.
  */
  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const FileDescriptor& value);
public:

  /** Handle to the standard input stream. */
  static FileDescriptor bin;
  /** Handle to the standard output stream. */
  static FileDescriptor bout;
  /** Handle to the standard error stream. */
  static FileDescriptor berr;
};

/**
  Writes a string representation of a FileDescriptor object to a format stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const FileDescriptor& value);

#endif
