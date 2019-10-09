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

#include <base/Exception.h>
#include <base/collection/Array.h>
#include <base/string/String.h>
#include <base/security/Trustee.h>
#include <base/security/AccessControlList.h>
#include <base/OperatingSystem.h>
#include <base/mem/Reference.h>
#include <base/Date.h>
#include <base/AnyValue.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace extension {

  /**
    This exception is raised by the RegistryKey extension class.
    
    @short Registry exception
    @ingroup exceptions extensions
    @version 1.0
  */

  class _DK_SDU_MIP__BASE__API RegistryException : public Exception {
  public:
    
    /**
      Initializes the exception object with no message.
    */
    inline RegistryException() throw() {
    }
    
    /**
      Initializes the exception object.
      
      @param message The message.
    */
    inline RegistryException(const char* message) throw() {
    }
    
    /**
      Initializes the exception object without an associated message.
      
      @param type The identity of the type.
    */
    inline RegistryException(Type type) throw() : Exception(type) {
    }
    
    /**
      Initializes the exception object.
       
      @param message An NULL-terminated string (ASCII).
      @param type The identity of the type.
    */
    inline RegistryException(const char* message, Type type) throw() : Exception(message, type) {
    }
  };
  
  /**
    This class provides access to the Registry on Windows platforms. This class
    is an extension component of the Base Framework and is only available for
    WIN32 platforms.
    
    @short Registry key.
    @ingroup extensions
    @version 1.0
  */
  
  class RegistryKey : public Object {
  private:

    /**
      A handle to a key within the Registry.
      
      @short Handle of Registry key.
      @ingroup extensions
      @version 1.0
    */
    
    class Handle : public ReferenceCountedObject {
    private:

      /** The hande to the Registry key. */
      OperatingSystem::Handle handle;
    public:

      /** Initializes handle. */
      inline Handle(void* _handle) throw() : handle(_handle) {
      }

      /** Returns the handle to the key. */
      inline OperatingSystem::Handle getHandle() const throw() {
        return handle;
      }

      /** Destroys the handle. */
      ~Handle();
    };

    /** Invalid handle. */
    static Reference<Handle> invalid;
    
    /** Handle to Registry key. */
    Reference<Handle> key;

    bool removeKeyRecursively(OperatingSystem::Handle key, const char* name) throw();
  public:

    /** Registry root. */
    enum Root {
      CLASSES, /**< The classes registry. */
      MACHINE, /**< The registry of the local machine. */
      USER, /**< The registry of the current user. */
      USERS, /**< The registry of the users. */
      SYSTEM /**< The default registry for new users. */ // TAG: fixme
    };

    /** The type of a value within the Registry. */
    enum ValueType {
      UNSUPPORTED_VALUE, /**< Value type is not supported. */
      UNSPECIFIED_VALUE, /**< Value type is unspecified. */
      BINARY_VALUE, /**< Unspecified binary data. */
      INTEGER_VALUE, /**< A 32 bit integer in little endian byte order. */
      INTEGER_BE_VALUE, /**< A 32 bit integer in big endian byte order. */
      INTEGER_64_VALUE, /**< A 64 bit integer in little endian byte order. */
      LINK_VALUE, /**< A link. */
      RESOURCE_LIST_VALUE, /**< Device driver resource list. */
      ENVIRONMENT_STRING_VALUE, /**< String possibly containing references to environment variables. */
      STRING_SEQUENCE_VALUE, /**< A sequence of strings. */
      STRING_VALUE /**< String value. */
    };

    enum Access {
      READ, /**< Read access. */
      WRITE, /**< Write access. */
      READWRITE /**< Read and write access. */
    };
    
    /**
      Returns a handle to the Registry of the local machine.
    */
    static RegistryKey getMachine() throw(RegistryException);
    
    /**
      Returns a handle to the personal Registry of the current user.
    */
    static RegistryKey getUser() throw(RegistryException);
    
    /**
      Initializes key as invalid.
    */
    RegistryKey() throw();

    /**
      Initialization of handle from other handle.
    */
    inline RegistryKey(const RegistryKey& copy) throw()
      : key(copy.key) {
    }

    /**
      Assignment of handle by handle.
    */
    inline RegistryKey& operator=(const RegistryKey& eq) throw() {
      key = eq.key;
      return *this;
    }
    
    /**
      Opens the specified Registry key on a remote machine.

      @param machine The machine (e.g. "\\machine").
      @param root The Registry root (e.g. MACHINE).
      @param path The path of the key within the specified root.
      @param access The desired access (i.e. READ, WRITE, READWRITE).
    */
    RegistryKey(const String& machine, Root root, const String& path, Access access) throw(RegistryException);

    /**
      Opens the specified Registry key.

      @param root The Registry root (e.g. MACHINE).
      @param path The path of the key within the specified root.
      @param access The desired access (i.e. READ, WRITE, READWRITE).
    */
    RegistryKey(Root root, const String& path, Access access) throw(RegistryException);
    
    /**
      Returns a handle to the specified subkey of this key.
      
      @param name The name of the subkey.
      @param access The desired access (i.e. READ, WRITE, READWRITE).
    */
    RegistryKey open(const String& name, Access access) const throw(RegistryException);

    /**
      Adds a new subkey.
      
      @param name The name of the subkey.
      @param access The desired access (i.e. READ, WRITE, READWRITE).
    */
    RegistryKey addSubkey(const String& name, Access access) throw(RegistryException);

    /**
      Closes the handle to this key.
    */
    void close() throw(RegistryException);

    /**
      Returns true if the handle is valid.
    */
    bool isValid() const throw();
    
    // TAG: use AnyValue (e.g. Map<String, AnyValue>)?

    /**
      Returns the names of the subkeys within the key.
    */
    Array<String> getKeys() throw(RegistryException);

    /**
      Returns the names of the values within the key.
    */
    Array<String> getValues() const throw(RegistryException);
    
    /**
      Returns true if this key is empty.
    */
    bool isEmpty() const throw(RegistryException);

    /**
      Returns true if the specified entry exists and is a key.
    */
    bool isKey(const String& name) const throw(RegistryException);
    
    /**
      Returns true if the specified entry exists and is a value.
    */
    bool isValue(const String& name) const throw(RegistryException);
    
    /**
      Returns the last modification time of the key.
    */
    Date getLastModification() const throw(RegistryException);

    /**
      Returns the owner of the key.
    */
    Trustee getOwner() const throw(RegistryException);

    /**
      Returns the primary group of the key.
    */
    Trustee getGroup() const throw(RegistryException);

    /**
      Returns the ACL for the key.
    */
    AccessControlList getACL() const throw(RegistryException);
    
    /**
      Sets the ACL of the key.
    */
    void setACL(const AccessControlList& acl) throw(RegistryException);

    /**
      Returns the content of the specified value.

      @param name The name of the value.
    */
    AnyValue getValue(const String& name) const throw(RegistryException);

    /**
      Returns the content of the specified value.

      @param name The name of the value.
      @param value The content of the Registry value.
    */
    void setValue(const String& name, const AnyValue& value) const throw(RegistryException);

    /**
      Returns the value of a 32 bit integer value (either INTEGER_VALUE or INTEGER_BE_VALUE)
    */
    uint32 getInteger(const String& name) const throw(RegistryException);

    /**
      Sets the value as a 32 bit integer (INTEGER_VALUE).
      
      @param name The name of the value.
      @param content The content of the value.
    */
    void setInteger(const String& name, uint32 value) throw(RegistryException);
    
    /**
      Returns the value of a binary value (i.e. BINARY_VALUE and UNSPECIFIED_VALUE).
      
      @param name The name of the value.
    */
    String getBinary(const String& name) const throw(RegistryException);
    
    /**
      Sets the value as a binary value (BINARY_VALUE).
      
      @param name The name of the value.
      @param buffer The source of the value content.
      @param size The number of bytes in the buffer.
    */
    void setBinary(const String& name, const char* buffer, unsigned int size) throw(RegistryException);
    
    /**
      Returns the value of a string value (i.e. STRING_VALUE or ENVIRONMENT_STRING_VALUE).
    */
    String getString(const String& name) const throw(RegistryException);
    
    /**
      Sets the value of a string value (STRING_VALUE).
      
      @param name The name of the Registry value.
      @param value The desired content of the value.
    */
    void setValue(const String& name, const String& value) throw(RegistryException);
    
    /**
      Returns the strings of a multi string value.
      
      @param name The name of the Registry value.
    */
    Array<String> getStringSequence(const String& name) const throw(MemoryException, RegistryException);

    /**
      Sets the content of a multi string value.
      
      @param name The name of the Registry value.
      @param value The desired mutil string content of the value.
    */
    void setValue(const String& name, const Array<String>& value) throw(RegistryException);
    
    /**
      Flushes any cached data to the Registry.
    */
    void flush() throw(RegistryException);
    
    /**
      Removes the specified subkey.
      
      @param name The name of the key to remove.
      @param force Specifies if true to remove the even if not empty. The default is false.
      
      @return True if key was removed.
    */
    bool removeKey(const String& name, bool force = false) throw(RegistryException);
    
    /**
      Removes the specified value from this key.
      
      @param name The name of the value.
      
      @return True if value was removed.
    */
    bool removeValue(const String& name) throw(RegistryException);
    
    /**
      Returns the type of the specified key or value.
      
      @param name The name of the value.
    */
    ValueType getTypeOfValue(const String& name) const throw(RegistryException);

    /**
      Returns the size of the specified value.
      
      @param name The name of the value.
    */
    unsigned int getSize(const String& name) const throw(RegistryException);
  };
  
}; // end of namespace extension

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
