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

#include <base/Object.h>
#include <base/io/FileRegion.h>
#include <base/mem/Reference.h>
#include <base/string/String.h>
#include <base/Date.h>
#include <base/Primitives.h>
#include <base/Resource.h>
#include <base/io/async/AsynchronousIOStream.h>
#include <base/security/AccessControlList.h>
#include <base/ValidifiedResult.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  A file.
  
  @short File.
  @ingroup io
  @version 1.1
*/

class _COM_AZURE_DEV__BASE__API File : public Resource, public AsynchronousIOStream {
public:

  /** File access. */
  enum Access {
    READ, /**< Request read access. */
    WRITE, /**< Request write access. */
    READWRITE /**< Request read and write access. */
  };
  
  /** Type used to specify the relative offset. */
  enum Whence {
    BEGIN, /**< Position is relative to begining of file. */
    CURRENT, /**< Position is relative to current position of file. */
    END /**< Position is relative to end of file. */
  };
  
  /** File initialization options. */
  enum Options {
    CREATE = 1, /**< Specifies that the file should be created if it doesn't exist. */
    TRUNCATE = 2, /**< Specifies that the file should be truncated if it already exists. */
    EXCLUSIVE = 4, /**< Specifies that the file should be opened in exclusive mode. */
    ASYNCHRONOUS = 8, /**< Specifies that the file should be opened for asynchronous IO (disables the synchronous IO interface). */
    APPEND = 16 /**< Specifies that data should be appended to the file. */
  };

  /** Sticky bit. */
  enum Sticky {
    SET_UID = 0x4000, /**< Set user id on execution. */
    SET_GID = 0x2000, /**< Set group id on execution. */
    RESTRICT = 0x1000, /**< Restrict flag. */
    STICKY_MASK = SET_UID|SET_GID|RESTRICT /**< All sticky bits. */
  };

  /** Permissions. */
  enum Permissions {
    RUSR = 0x400, /**< Read permission for the owner of the file. */
    WUSR = 0x200, /**< Write permission for the owner of the file. */
    XUSR = 0x100, /**< Execute permission for the owner of the file. */
    RWXU = RUSR|WUSR|XUSR, /**< Read, write, and execute permissions for the owner of the file. */
    RGRP = 0x40, /**< Read permission for the group associated with the file. */
    WGRP = 0x20, /**< Write permission for the group associated with the file. */
    XGRP = 0x10, /**< Execute permission for the group associated with the file. */
    RWXG = RGRP|WGRP|XGRP, /**< Read, write, and execute permissions for the group associated with the file. */
    ROTH = 0x4, /**< Read permission for other users. */
    WOTH = 0x2, /**< Write permission for other users. */
    XOTH = 0x1, /**< Execute permission for other users. */
    RWXO = ROTH|WOTH|XOTH, /**< Read, write, and execute permissions for other users. */
    PERMISSION_MASK = RWXU|RWXG|RWXO /**< Any access. */
  };

  /** File system variables. */
  enum Variable {
    MIN_FILE_SIZE_BITS, /**< Specifies the minimum number of bits required to represent the maximum supported file size. */
    MAX_NUM_OF_LINKS, /**< Specifies the maximum number of links to a file. */
    MAX_LEN_OF_NAME, /**< Specifies the maximum length of a file name. */
    MAX_LEN_OF_PATH, /**< Specifies the maximum length of a path. */
    MAX_SIZE_OF_PIPE_BUFFER, /**< Specifies the maximum number bytes that is guaranteed to be atomic when writing to a pipe. */
    MAX_SIZE_OF_SYMLINK /**< Specifies the maximum number of bytes in a symbolic link. */
  };
public:

  /**
    Sets the owner of the file.

    @param path The path of the file.
    @param owner The new owner of the file.
    @param group The primary group of the file.
    @param followLink Specifies whether or not to follow the link.
  */
  static void changeOwner(const String& path, const Trustee& owner, const Trustee& group, bool followLink = true);
  
  /**
    Initializes an invalid file object (correspons to a closed file).
  */
  File() noexcept;

  /**
    Initializes a file. If no options are specified the initialization will
    fail if the file doesn't exist.

    @param path the path of the file.
    @param access The desired access to the files.
    @param option Additional options (i.e. CREATE, TRUNCATE, EXCLUSIVE).
  */
  File(const String& path, Access access, unsigned int options);

  /**
    Closes the file.
  */
  void close();

  /**
    Returns true if the file object has been closed (or need has been opened).
  */
  bool isClosed() const noexcept;

  /**
    Returns the Access Control List (ACL) of the file.
  */
  AccessControlList getACL() const;

  /**
    Returns the owner of the file.
  */
  Trustee getOwner() const;
  
  /**
    Returns the group of the file.
  */
  Trustee getGroup() const;

  /**
    Returns the permissions of the file.
  */
  unsigned int getMode() const;
  
  /**
    Returns the size of the file in bytes.
  */
  long long getSize() const;

  /**
    Returns the current position.
  */
  long long getPosition() const;

  /**
    Sets the position.

    @param position The relative offset from the specified base offset.
    @param whence Specifies the base offset. The default is the beginning of
    the file.
  */
  void setPosition(long long position, Whence whence = BEGIN);

  /**
    Truncates the file to the specified size. The file must have been opened
    with write access. If the file previously was larger than the specified
    size, the extra data is discarded. If the file was previously shorter, it
    is extended with zeros.
  */
  void truncate(long long size);

  /**
    Flushes any buffered data.
  */
  void flush();



  /**
    Locks the specified region of the file. Blocks until lock has been
    acquired. Exclusive locks require the file to be opened with write access.

    @param region The file region to be locked.
    @param exclusive Specifies that the region should be locked exclusively.
    Default is true.
  */
  void lock(const FileRegion& region, bool exclusive = true);

  /**
    Tries to lock specified region of the file. Exclusive locks require the
    file to be opened with write access.

    @param region The file region to be locked.
    @param exclusive Specifies that the region should be locked exclusively.
    Default is true.

    @return True if the lock was acquired.
  */
  bool tryLock(const FileRegion& region, bool exclusive = true);

  /**
    Unlocks the specified region of the file.

    @param region The file region to be unlocked.
  */
  void unlock(const FileRegion& region);



  /**
    Returns the last modification time of the file.
  */
  Date getLastModification();

  /**
    Returns the last access time of the file.
  */
  Date getLastAccess();

  /**
    Returns the last chnage time of the file.
  */
  Date getLastChange();



  /**
    Returns the value of the specified file system variable.
  */
  unsigned long getVariable(Variable variable);
  


  /**
    If another process has acquired an exclusive lock on part of the file
    region to be read, the read request will block until the lock has been
    released.

    @return 0 if the end of the file has been reached (or if requested to read
    0 bytes).
  */
  unsigned int read(
    uint8* buffer,
    unsigned int size,
    bool nonblocking = false);

#if 0
  MemorySize read(
    uint8* buffer,
    MemorySize size,
    bool nonblocking = false)
  {
    BASSERT(size <= 0xffffffff); // TAG: temp
    return read(buffer, static_cast<unsigned int>(size), nonblocking);
  }
#endif

  /**
    Read the specified type.
  */
  template<class TYPE>
  inline unsigned int read(TYPE& buffer)
  {
    return read(Cast::getAddress(buffer), sizeof(TYPE));
  }

  /**
    Raises EndOfFile if minimum number of bytes cannot be read without
    exceeding the end of the file.
  */
  unsigned int write(
    const uint8* buffer,
    unsigned int size,
    bool nonblocking = false);

  /**
    Returns the OS handle.
  */
  OperatingSystem::Handle getHandle() const noexcept;

  /**
    Aborts any pending asynchronous operations (read as well as write).
  */
  void asyncCancel();
  
  /**
    Requests and asynchronous read operation. Asynchronous IO is only supported
    if the file has been opened in asynchronous mode.

    @param buffer The bytes to be read.
    @param bytesToRead The number of bytes to be read.
    @param offset The offset from the beginning of the file.
    @param listener The listener to be notified on completion.
  */
  AsynchronousReadOperation read(
    uint8* buffer,
    unsigned int bytesToRead,
    unsigned long long offset,
    AsynchronousReadEventListener* listener);

  /**
    Requests and asynchronous write operation. Asynchronous IO is only
    supported if the file has been opened in asynchronous mode.

    @param buffer The bytes to be written.
    @param bytesToWrite The number of bytes to be written.
    @param offset The offset from the beginning of the file.
    @param listener The listener to be notified on completion.
  */
  AsynchronousWriteOperation write(
    const uint8* buffer,
    unsigned int bytesToWrite,
    unsigned long long offset,
    AsynchronousWriteEventListener* listener);
  
  /**
    Creates a file from the given string. Saves the raw bytes.

    @return Returns true on succefss.
  */
  static bool createFile(const String& path, const uint8* buffer, MemorySize size);

  /**
    Creates a file from the given string. Saves the raw bytes.

    @return Returns true on success.
  */
  static bool createFile(const String& path, const String& text);
  
  enum Encoding {
    ENCODING_RAW, //< Any bytes
    ENCODING_ASCII, //< Only ASCII
    ENCODING_UTF8, //< UTF-8 encoding with Unicode codes
    ENCODING_UTF8_ISO //< UTF-8 encoding with ISO codes
  };
  
  /**
    Reads bytes from file to string.
   
    @param path The file path.
    @param encoding The encoding. Defaults to UTF-8 Unicode.
  */
  static Validified<String> readFile(const String& path, Encoding encoding = ENCODING_UTF8);
  
  /**
    Destroys the file.
  */
  ~File();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
