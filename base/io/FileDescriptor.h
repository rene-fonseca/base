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

  /**
    Reference counted handle to file descriptor.

    @author René Møller Fonseca
    @version 1.0
  */
  class Descriptor : public ReferenceCountedObject {
  private:

    /** Handle to file descriptor. */
    int handle;
  public:

    /**
      Initializes invalid descriptor.
    */
    inline Descriptor() throw() : handle(-1) {}

    /**
      Initializes descriptor.

      @param handle The handle.
    */
    inline explicit Descriptor(int h) throw() : handle(h) {}

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
    inline int getHandle() const throw() {return handle;}

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
  FileDescriptor(int handle) throw();

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
};

/**
  Writes a string representation of a FileDescriptor object to a format stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const FileDescriptor& value);

#endif
