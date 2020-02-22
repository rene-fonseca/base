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
#include <base/string/String.h>
#include <base/filesystem/FileSystemException.h>
#include <base/NotSupported.h>
#include <base/io/File.h>
#include <base/security/Trustee.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  @defgroup filesystem File system
*/

/**
  File system.
  
  @short File system
  @ingroup filesystem
  @version 1.1
*/

class _COM_AZURE_DEV__BASE__API FileSystem : public Object {
public:

  /** Returns true if char is a separator. For Windows both \ and / are separator. */
  static inline bool isSeparator(ucs4 ch) noexcept
  {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    return (ch == '/') || (ch == '\\');
#else
    return (ch == '/');
#endif
  }

  /**
    Returns the index of the first separator. For Windows both \ and / are separator.

    Returns -1 is not found.
  */
  static MemoryDiff findSeparator(const String& path, MemorySize start);
  
  /**
    Returns the index of the last separator. For Windows both \ and / are separator.
    
    Returns -1 is not found.
  */
  static MemoryDiff findLastSeparator(const String& path);

  /** Specifies the maximum length of a path. */
  static const unsigned int MAXIMUM_PATH_LENGTH;

  /** File system entry type. */
  enum {
    BLOCK = 1, /**< Block. */
    CHARACTER = 2, /**< Character. */
    DEVICE = 1024, /**< Device. */
    FIFO = 4, /**< Fifo. */
    FOLDER = 8, /**< Folder. */
    LINK = 16, /**< Link. */
    MESSAGE_QUEUE = 32, /**< Message queue. */
    REGULAR = 64, /**< Regular. */
    SOCKET = 128, /**< Socket. */
    SEMPAHORE = 256, /**< Semaphore. */
    SHARED_MEMORY = 512 /**< Shared memory. */
  };

  /** The temporary folder. */
  enum TemporaryFolder {
    /**
      Uses the temporary folder specified by the 'TMP' environment variable. If
      the variable doesn't exist the TEMP variable is used. If neither are
      defined this falls back on MACHINE_NONPERSISTENT.
    */
    USER_SPECIFIED,
    /** Specifies the temporary folder intended for non-persistent temporary files. */
    MACHINE_NONPERSISTENT,
    /** Specifies the temporary folder intended for persistent temporary files. */
    MACHINE_PERSISTENT
  };
  
  /** The fields of a temporary file. */
  enum TemporaryFileNameField {
    /** Add the prefix '~'. */
    PREFIX = 1,
    /** Add the user name. */
    USER = 2,
    /** Add the formal name of application. */
    APPLICATION = 4,
    /** Add the process id. */
    PROCESS = 8,
    /** Add the host name. */
    HOST = 16,
    /** Add internal counter (and increment the counter). */
    COUNTER = 32,
    /** Add the current time. */
    TIME = 64,
    /** Add a random number. */
    RANDOM = 128,
    /** Add the suffix '.tmp'. */
    SUFFIX = 256
  };

  /** Temporary file options. */
  enum TemporaryFileOption {
    /** Sets the access control of the file such that only the owner of the process has access to it. */
    STRICT_ACCESS = 1,
    /** The content of the file is overwritten when the file is closed (this implies REMOVE_ON_CLOSE). */
    OVERWRITE_ON_CLOSE = 2,
    /** The file is removed when closed. */
    REMOVE_ON_CLOSE = 4,
    /** The file is opened in exclusive mode. */
    EXCLUSIVE = 8,
    /** The file is opened in secure mode. */
    SECURE = STRICT_ACCESS | OVERWRITE_ON_CLOSE | REMOVE_ON_CLOSE
  };
  
  /**
    Returns the combined path of the specified base and relative path.

    @param base The base path.
    @param relative The relative path.

    @return the relative path if base is blank.
  */
  static String getPath(const String& base, const String& relative) noexcept;

  /**
    Returns the combined path of the specified base and relative path.

    @param base The base path.
    @param relative The relative path.

    @return Returns the relative path if base is blank.
  */
  static inline String join(const String& base, const String& relative) noexcept
  {
    return getPath(base, relative);
  }

  /**
    Returns the combined path of the specified base and relative path.

    @param paths The paths to join.
  */
  static String join(const std::initializer_list<String>& paths) noexcept;

  enum Component {
    FOLDER_PATH, /**< The folder with an ending separator (i.e. '/' or '\'). */
    FILENAME, /**< The name and extension. */
    NAME, /**< The name of the entry excluding the extension and dot. */
    DOTEXTENSION, /**< The extension (including the dot). */
    EXTENSION /**< The extension (excluding the dot). */
  };
  
  /* Quota. */
  class _COM_AZURE_DEV__BASE__API Quota {
  public:

    /** Hard limit. */
    uint64 softLimit = 0;
    /** Soft limit. */
    uint64 hardLimit = 0;
    /** Current usage. */
    uint64 currentUsage = 0;
  };
  
  /**
    Returns the specified component of the path.

    @param path The path.
    @param component The desired component (FOLDER_PATH, NAME, EXTENSION, ...).
  */
  static String getComponent(const String& path, Component component);

  /**
    Returns true if the specifies path is an absolute path.
  */
  static bool isAbsolutePath(const String& path) noexcept;

  /**
    Returns true if the specified path is a explicit folder path (i.e. ends with
    the separator).
  */
  static inline bool isFolderPath(const String& path) noexcept;
  
  /**
    Converts the path to an absolute path.

    @param base The base path.
    @param path The path to be converted.
  */
  static String toAbsolutePath(const String& base, const String& path);

  /**
    Returns the absolute path of the first file found.
    
    @param searchPaths The search paths.
    @param relative The relative path (SHOULD not be absolute).
    @param index Specifies the first index in the searchPaths. The default is 0.
    
    @return Empty string if not found. The relative path if it is really an absolute path.
  */
  static String findFile(const Array<String>& searchPaths, const String& relative, unsigned int index = 0);

  /**
    Returns a URL from the specified path. The URL has the following format:
    "file:///C:/WINNT" or "file:///usr/local".
    
    @param path The path to convert to a URL (MUST be absolute).
  */
  static String toUrl(const String& path);
  
  /**
    Returns the path of the current folder.
  */
  static String getCurrentFolder();
  
  /**
    Sets the current folder.
  */
  static void setCurrentFolder(const String& path);

  /**
    Returns the file system flags describing the specified entry.
  */
  static unsigned int getType(const String& path);

  /**
    Returns the size of the specified file.
  */
  static uint64 getSize(const String& path);
  
  /**
    Returns true if the entry exists.
  */
  static bool entryExists(const String& path);
  
  /**
    Returns true if the file exists.
  */
  static bool fileExists(const String& path);

  /**
    Returns true if the folder exists.
  */
  static bool folderExists(const String& path);

  /**
    Removes the specified file.
  */
  static void removeFile(const String& path);

  /**
    Removes the specified folder. The folder must be empty.
  */
  static void removeFolder(const String& path);

  /**
    Makes a folder.
  */
  static void makeFolder(const String& path);

  /**
    Makes a folder.
  */
  static void makeFolderRecursive(const String& path);

  /**
    Returns true if symbolic links are supported. This method always returns
    true for Unices.
  */
  static bool doesSupportLinks() noexcept;
  
  /**
    Creates a hard link.
    
    @param target The target of the link.
    @param destination The path of the destination file/folder.
  */
  static void makeHardLink(const String& target, const String& destination);
  
  /**
    Returns true if the file object specified by the path is a symbolic link.

    @return False if the path doesn't point to an object.
  */
  static bool isLink(const String& path);
  
  /**
    Creates a symbolic link.
    
    @param target The target of the link.
    @param destination The path of the destination file/folder.
  */
  static void makeLink(const String& target, const String& destination);
  
  /**
    Returns the target of the symbolic link.

    @param path The path of the symbolic link.
  */
  static String getLinkTarget(const String& path);
  
  /**
    Returns the path to the folder intended for temporary files.

    @param folder Specifies the folder to be used. The default is USER_SPECIFIED.
  */
  static String getTempFolder(TemporaryFolder folder = USER_SPECIFIED) noexcept;
  
  /**
    Generates a temporary file name. The returned name should be used in
    conjuction with a folder path returned by getTempFolder or other folder
    path. Every field is separated by a '-' except for the PREFIX and SUFFIX.
    The order of the fields are fixed and given by the order of the fields
    within the enumeration TemporaryFileNameField.
    
    @param fields Specifies the fields to include in the name. The default is
    PREFIX|APPLICATION|USER|SUFFIX.
  */
  static String getTempFileName(unsigned int fields = PREFIX|APPLICATION|USER|SUFFIX) noexcept;
  
  /**
    Returns a temporary file.
    
    @param options The file options. The default is SECURE.
  */
  static File getTempFile(unsigned int options = SECURE);

  /** File system variables. */
  enum Variable {
    MIN_FILE_SIZE_BITS, /**< Specifies the minimum number of bits required to represent the maximum supported file size. */
    MAX_NUM_OF_LINKS, /**< Specifies the maximum number of links to a file. */
    MAX_LEN_OF_NAME, /**< Specifies the maximum length of a file name. */
    MAX_LEN_OF_PATH, /**< Specifies the maximum length of a path. */
    MAX_SIZE_OF_PIPE_BUFFER, /**< Specifies the maximum number bytes that is guaranteed to be atomic when writing to a pipe. */
    MAX_SIZE_OF_SYMLINK /**< Specifies the maximum number of bytes in a symbolic link. */
  };
  
  /**
    Returns the value of the specified file system variable.
  */
  static unsigned long getVariable(const String& path, Variable variable);

  /** System folders. */
  enum Folder {
    /**
      The root folder.
    */
    ROOT,
    /**
      The folder intended for devices.
    */
    DEVICES,
    /**
      Folder intended for temporary files of applications. The files are not
      guaranteed to be preserved between application invocations.
    */
    TEMP   
  };

  /**
    Returns the path to the specified folder.
  */
  static String getFolder(Folder folder) noexcept;

  /**
    Returns the relative path to the given folder.

    getRelativePath("a/b/c/d/e/f", "a/b/c/g/h/i") returns "../../../g/h/i".
  */
  static String getRelativePath(const String& folder, const String& path);

  /**
    Returns the parent folder.
  */
  static String getParent(const String& path);

  /**
    Returns the path split into sections.
    
    Separators are not included in result except the first item it is is to indicate root.
  */
  static Array<String> split(const String& path);

  /**
    Joins the paths.
  */
  static String join(const Array<String>& paths);

  /**
    Returns the normalized path.
  */
  static String normalize(const String& path);
  
  /**
    Returns the quota.

    @param path The path to the device.
    @param trustee The truestee.
  */
  static Quota getQuota(const String& path, Trustee trustee);
};

/** File system path. Used for concatenation. */
class _COM_AZURE_DEV__BASE__API Path : public Object {
private:

  String path;
public:

  /** Initializes path. */
  inline Path() noexcept
  {
  }

  /** Initializes path. */
  inline Path(const String& _path) noexcept
    : path(_path)
  {
  }

  /** Initializes path. */
  inline Path(String&& _path) noexcept
    : path(moveObject(_path))
  {
  }

  /** Initializes path. */
  inline Path(const std::initializer_list<String>& paths)
  {
    path = FileSystem::join(paths);
  }
  
  /** Returns the path. */
  inline operator const String&() const noexcept
  {
    return path;
  }
};

/** Concats paths. */
inline Path operator/(const String& a, const Path& b)
{
  return FileSystem::join(a, b);
}

/** Concats paths. */
inline Path operator/(const Path& a, const String& b)
{
  return FileSystem::join(a, b);
}

/** Concats paths. */
inline Path operator/(const Path& a, const Path& b)
{
  return FileSystem::join(a, b);
}

/** Concats paths. Use / operator. */
Path operator+(const Path& a, const String& b) = delete;

/**
  Writes a string representation of the InetAddress object to a format stream.
*/
inline FormatOutputStream& operator<<(FormatOutputStream& stream, const Path& value)
{
  return stream << static_cast<const String&>(value);
}

_COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE(Path)

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
