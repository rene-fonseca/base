/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/extension/dk.sdu.mip/win32/Registry.h>
#include <base/ByteOrder.h>
#include <base/NotImplemented.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#  include <aclapi.h>
#  undef VOID
#else // unix
#  error Extension not supported for this flavor
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace extension {

  namespace win32 {

    ReferenceCountedObjectPointer<RegistryKey::Handle> RegistryKey::invalid;
    
    RegistryKey::Handle::~Handle() throw(RegistryException) {
      if (handle) {
        assert(
          ::RegCloseKey((HKEY)handle) == ERROR_SUCCESS,
          RegistryException(this)
        );
        handle = 0;
      }
    }
    
    
    
    RegistryKey RegistryKey::getMachine() throw(RegistryException) {
      return RegistryKey::RegistryKey(RegistryKey::MACHINE, MESSAGE(""), RegistryKey::READWRITE);
    }

    RegistryKey RegistryKey::getUser() throw(RegistryException) {
      return RegistryKey::RegistryKey(RegistryKey::USER, MESSAGE(""), RegistryKey::READWRITE);
    }

    RegistryKey::RegistryKey() throw() {
      if (!invalid.isValid()) {
        invalid = new RegistryKey::Handle(OperatingSystem::INVALID_HANDLE);
      }
      key = invalid;
    }
    
    RegistryKey::RegistryKey(const String& machine, Root root, const String& path, Access access) throw(RegistryException) {
      HKEY rootKey;
      
      HKEY regRoot;
      switch (root) {
      case RegistryKey::MACHINE:
        regRoot = HKEY_LOCAL_MACHINE;
        break;
      case RegistryKey::USERS:
        regRoot = HKEY_USERS;
        break;
      default:
        throw RegistryException("Invalid root", this);
      }
      
      assert(
        ::RegConnectRegistry(machine.getElements(), regRoot, &rootKey) == ERROR_SUCCESS,
        RegistryException("Unable to connect to Registry", this)
      );
      
      REGSAM regAccess = 0;
      switch (access) {
      case RegistryKey::READ:
        regAccess |= KEY_READ;
        break;
      case RegistryKey::WRITE:
        regAccess |= KEY_WRITE;
        break;
      case RegistryKey::READWRITE:
        regAccess |= KEY_READ|KEY_WRITE;
        break;
      }

      HKEY regKey;
      if (path.isProper()) {
        LONG error = ::RegOpenKeyEx(rootKey, // handle to open key
                                    path.getElements(), // subkey name
                                    0, // reserved
                                    access, // security access mask
                                    &regKey // handle to new key
        );
        if (error != ERROR_SUCCESS) {
          ::RegCloseKey(rootKey);
          throw RegistryException("Unable to open key", this);
        }
      } else {
        regKey = rootKey;
      }
      
      this->key = new RegistryKey::Handle((OperatingSystem::Handle)regKey);
    }
    
    RegistryKey::RegistryKey(Root root, const String& path, Access access) throw(RegistryException) {
      REGSAM regAccess = 0;
      switch (access) {
      case RegistryKey::READ:
        regAccess |= KEY_READ;
        break;
      case RegistryKey::WRITE:
        regAccess |= KEY_WRITE;
        break;
      case RegistryKey::READWRITE:
        regAccess |= KEY_READ|KEY_WRITE;
        break;
      }
      
      HKEY regKey;
      switch (root) {
      case RegistryKey::CLASSES:
        regKey = HKEY_CLASSES_ROOT;
        break;
      case RegistryKey::MACHINE:
        regKey = HKEY_LOCAL_MACHINE;
        break;
      case RegistryKey::USER:
        regKey = HKEY_CURRENT_USER;
        break;
      case RegistryKey::USERS:
        regKey = HKEY_USERS;
        break;
      }
      assert(::RegOpenKeyEx((HKEY)key->getHandle(), // handle to open key
                            path.getElements(), // subkey name
                            0,   // reserved
                            access, // security access mask
                            &regKey // handle to new key
             ) == ERROR_SUCCESS, RegistryException("Unable to open key", this));
      
      key = new RegistryKey::Handle((OperatingSystem::Handle)regKey);
    }
    
    RegistryKey RegistryKey::open(const String& name, Access access) const throw(RegistryException) {
      REGSAM regAccess = 0;
      switch (access) {
      case RegistryKey::READ:
        regAccess |= KEY_READ;
        break;
      case RegistryKey::WRITE:
        regAccess |= KEY_WRITE;
        break;
      case RegistryKey::READWRITE:
        regAccess |= KEY_READ|KEY_WRITE;
        break;
      }
      
      HKEY regKey;
      assert(::RegOpenKeyEx((HKEY)key->getHandle(), // handle to open key
                            name.getElements(), // subkey name
                            0,   // reserved
                            regAccess, // security access mask
                            &regKey // handle to new key
             ) == ERROR_SUCCESS, RegistryException("Unable to open key", this));

      RegistryKey result;
      result.key = new RegistryKey::Handle((OperatingSystem::Handle)regKey);
      return result;
    }

    RegistryKey RegistryKey::addSubkey(const String& name, Access access) throw(RegistryException) {
      HKEY regKey;
      DWORD disposition;
      assert(::RegCreateKeyEx((HKEY)key->getHandle(), // handle to open key
                              name.getElements(), // subkey name
                              0, // reserved
                              0, // class string
                              0, // special options
                              KEY_ALL_ACCESS, // desired security access
                              0, // inheritance
                              &regKey, // key handle
                              &disposition // disposition value buffer
      ) == ERROR_SUCCESS, RegistryException("Unable to open key", this));

      RegistryKey result;
      result.key = new RegistryKey::Handle((OperatingSystem::Handle)regKey);
      return result;
    }
 
    void RegistryKey::close() throw(RegistryException) {
      key = invalid; // always initialized at this point
    }

    bool RegistryKey::isValid() const throw() {
      return key.getValue() != invalid.getValue(); // TAG: fixme key != invalid
    }
    
    Array<String> RegistryKey::getKeys() throw(RegistryException) {
      DWORD maxLength; // excluding terminator
      DWORD numberOfSubkeys;
      assert(::RegQueryInfoKey((HKEY)key->getHandle(),
                               0, // class buffer
                               0, // size of the class buffer
                               0, // reserved
                               &numberOfSubkeys, // number of subkeys
                               &maxLength, // longest subkey name
                               0, // longest class string
                               0, // number of value entries
                               0, // longest value name
                               0, // longest value data
                               0, // descriptor length
                               0 // last write time
             ) == ERROR_SUCCESS,
             RegistryException(this)
      );
      
      Array<String> result(numberOfSubkeys, String());
      DWORD index = 0;
      char name[maxLength + 1];

      Array<String>::Iterator i = result.getBeginIterator();
      
      while (true) {
        DWORD nameLength = sizeof(name); // input: includes terminator, output: excluding terminator
        assert(::RegEnumKeyEx((HKEY)key->getHandle(), // handle to key to query
                              index++, // index of subkey to query
                              name, // subkey buffer
                              &nameLength, // size of subkey buffer
                              0, // reserved
                              0, // class string buffer
                              0, // size of class string buffer
                              0 // last write time
               ) == ERROR_SUCCESS, RegistryException(this));
        String temp(nameLength);
        copy(temp.getElements(), name, nameLength);
        *i++ = String(name);
      }
      return result;
    }
    
    Array<String> RegistryKey::getValues() const throw(RegistryException) {
      DWORD maxLength; // excluding terminator
      DWORD numberOfValues;
      assert(::RegQueryInfoKey((HKEY)key->getHandle(),
                               0, // class buffer
                               0, // size of the class buffer
                               0, // reserved
                               0, // number of subkeys
                               0, // longest subkey name
                               0, // longest class string
                               &numberOfValues, // number of value entries
                               &maxLength, // longest value name
                               0, // longest value data
                               0, // descriptor length
                               0 // last write time
             ) == ERROR_SUCCESS,
             RegistryException(this)
      );

      Array<String> result(numberOfValues, String());
      DWORD index = 0;
      char name[maxLength + 1];

      Array<String>::Iterator i = result.getBeginIterator();
      
      while (true) {
        DWORD nameLength = sizeof(name); // input: includes terminator, output: excluding terminator
        assert(::RegEnumValue((HKEY)key->getHandle(), // handle to key to query
                              index++, // index of value to query
                              name, // value buffer
                              &nameLength, // size of value buffer
                              0, // reserved
                              0, // type buffer
                              0, // data buffer
                              0 // size of data buffer
               ) == ERROR_SUCCESS, RegistryException(this));
        String temp(nameLength);
        copy(temp.getElements(), name, nameLength);
        *i++ = String(name);
      }
      return result;
    }

    bool RegistryKey::isEmpty() const throw(RegistryException) {
      DWORD numberOfSubkeys;
      DWORD numberOfValues;
      assert(::RegQueryInfoKey((HKEY)key->getHandle(),
                               0, // class buffer
                               0, // size of the class buffer
                               0, // reserved
                               &numberOfSubkeys, // number of subkeys
                               0, // longest subkey name
                               0, // longest class string
                               &numberOfValues, // number of value entries
                               0, // longest value name
                               0, // longest value data
                               0, // descriptor length
                               0 // last write time
             ) == ERROR_SUCCESS,
             RegistryException(this)
      );
      return (numberOfSubkeys == 0) && (numberOfValues == 0);
    }

    bool RegistryKey::isKey(const String& name) const throw(RegistryException) {
      HKEY temp;
      LONG error = ::RegOpenKeyEx((HKEY)key->getHandle(), // handle to open key
                                  name.getElements(), // subkey name
                                  0, // reserved
                                  0, // security access mask
                                  &temp // handle to open key
      );
      if (error == ERROR_SUCCESS) {
        ::RegCloseKey(temp);
      }
      return error == ERROR_SUCCESS;
    }
    
    bool RegistryKey::isValue(const String& name) const throw(RegistryException) {
      return ::RegQueryValueEx((HKEY)key->getHandle(),
                               name.getElements(), // value name
                               0, // reserved
                               0, // type buffer
                               0, // data buffer
                               0 // size of data buffer
             ) == ERROR_SUCCESS; // TAG: does this work
    }

    Date RegistryKey::getLastModification() const throw(RegistryException) {
      FILETIME modificationTime;
      assert(::RegQueryInfoKey((HKEY)key->getHandle(),
                               0, // class buffer
                               0, // size of the class buffer
                               0, // reserved
                               0, // number of subkeys
                               0, // longest subkey name
                               0, // longest class string
                               0, // number of value entries
                               0, // longest value name
                               0, // longest value data
                               0, // descriptor length
                               &modificationTime // last write time
             ) == ERROR_SUCCESS,
             RegistryException(this)
      );
      return Date((Cast::impersonate<int64>(modificationTime) - 116444736000000000LL)/10000000);
    }
    
    Trustee RegistryKey::getOwner() const throw(RegistryException) {
      SECURITY_DESCRIPTOR* securityDescriptor;
      PSID ownerSID;
      assert(::GetSecurityInfo((HKEY)key->getHandle(), SE_REGISTRY_KEY,
                               OWNER_SECURITY_INFORMATION, 0, &ownerSID, 0, 0,
                               &securityDescriptor) == ERROR_SUCCESS,
             RegistryException(this)
      );
      Trustee owner(Trustee::UNSPECIFIED, (const void*)ownerSID);
      ::LocalFree(securityDescriptor);
      return owner;
    }

    Trustee RegistryKey::getGroup() const throw(RegistryException) {
      SECURITY_DESCRIPTOR* securityDescriptor;
      PSID groupSID;
      assert(::GetSecurityInfo((HKEY)key->getHandle(), SE_REGISTRY_KEY,
                               GROUP_SECURITY_INFORMATION, 0, &groupSID, 0, 0,
                               &securityDescriptor) == ERROR_SUCCESS,
             RegistryException(this)
      );
      Trustee group(Trustee::UNSPECIFIED, (const void*)groupSID);
      ::LocalFree(securityDescriptor);
      return group;
    }
    
    AccessControlList RegistryKey::getACL() const throw(RegistryException) {
      // see File::getACL()
    }
    
    void RegistryKey::setACL(const AccessControlList& acl) throw(RegistryException) {
    }

    AnyValue RegistryKey::getValue(const String& name) const throw(RegistryException) {
      DWORD size;
      DWORD type;
      assert(::RegQueryValueEx((HKEY)key->getHandle(),
                               name.getElements(), // value name
                               0, // reserved
                               &type, // type buffer
                               0, // data buffer
                               &size // size of data buffer
             ) == ERROR_SUCCESS, RegistryException(this));
      
      switch (type) {
      case REG_BINARY:
        return RegistryKey::BINARY_VALUE;
      case REG_DWORD:
        return RegistryKey::INTEGER_VALUE;
      case REG_DWORD_BIG_ENDIAN:
        return RegistryKey::INTEGER_BE_VALUE;
      case REG_EXPAND_SZ:
        return RegistryKey::ENVIRONMENT_STRING_VALUE;
      case REG_LINK:
        return RegistryKey::LINK_VALUE;
      case REG_MULTI_SZ:
        return RegistryKey::STRING_SEQUENCE_VALUE;
      case REG_NONE:
        return RegistryKey::UNSPECIFIED_VALUE;
      case REG_QWORD:
        return RegistryKey::INTEGER_64_VALUE;
      case REG_RESOURCE_LIST:
        return RegistryKey::RESOURCE_LIST_VALUE;
      case REG_SZ:
        return RegistryKey::STRING_VALUE;
      default:
        throw RegistryException("Unsupported value", this);
      }

      DWORD secondType;

      switch (type) {
      case REG_DWORD:
      case REG_DWORD_BIG_ENDIAN:
      case REG_QWORD:
        {
          union {
            uint32 integer32;
            uint64 integer64;
          } buffer;
          
          assert(
            (::RegQueryValueEx((HKEY)key->getHandle(),
                               name.getElements(), // value name
                               0, // reserved
                               &secondType, // type buffer
                               (BYTE*)&buffer, // data buffer
                               &size // size of data buffer
            ) == ERROR_SUCCESS) && (type == secondType),
            RegistryException("Unable to get value", this)
          );
          switch (type) {
          case REG_DWORD:
            return AnyValue(buffer.integer32); // TAG: fix byte order
          case REG_DWORD_BIG_ENDIAN:
            return AnyValue(buffer.integer32); // TAG: fix byte order
          case REG_QWORD:
            return AnyValue(buffer.integer64); // TAG: fix byte order
          }
        }
        break;
      case REG_EXPAND_SZ:
      case REG_SZ:
      case REG_MULTI_SZ:
        --size; // remove last terminator
      case REG_BINARY:
      case REG_LINK:
      case REG_NONE:
      case REG_RESOURCE_LIST:
      default:
        String buffer;
        buffer.forceToLength(size);
        assert(
          (::RegQueryValueEx((HKEY)key->getHandle(),
                             name.getElements(), // value name
                             0, // reserved
                             &secondType, // type buffer
                             (BYTE*)&buffer, // data buffer
                             &size // size of data buffer
          ) == ERROR_SUCCESS) && (type == secondType),
          RegistryException("Unable to get value", this)
        );
        return buffer;
      }
    }

    void RegistryKey::setValue(const String& name, const AnyValue& value) const throw(RegistryException) {
      DWORD type;
      String valueAsString;
      DWORD valueAsDWORD;
      
      switch (value.getRepresentation()) {
      case AnyValue::VOID: // store empty string
        valueAsString = value.getString();
        type = REG_SZ;
        break;
      case AnyValue::CHARACTER: // string
        valueAsString = value.getString();
        type = REG_SZ;
        break;
      case AnyValue::WIDE_CHARACTER: // multibyte string (later ucs)
        valueAsString = value.getString();
        type = REG_SZ;
        break;
      case AnyValue::BOOLEAN: // integer
        valueAsDWORD = value.getUnsignedInteger();
        type = REG_DWORD;
        break;
      case AnyValue::SHORT_INTEGER: // integer
        valueAsDWORD = value.getUnsignedInteger();
        type = REG_DWORD;
        break;
      case AnyValue::UNSIGNED_SHORT_INTEGER: // integer
        valueAsDWORD = value.getUnsignedInteger();
        type = REG_DWORD;
        break;
      case AnyValue::INTEGER: // integer
        valueAsDWORD = value.getUnsignedInteger();
        type = REG_DWORD;
        break;
      case AnyValue::UNSIGNED_INTEGER: // integer
        valueAsDWORD = value.getUnsignedInteger();
        type = REG_DWORD;
        break;
      case AnyValue::LONG_INTEGER: // integer
        valueAsDWORD = value.getUnsignedInteger(); // use 32 or 64 bit
        type = REG_DWORD;
        break;
      case AnyValue::UNSIGNED_LONG_INTEGER: // integer
        valueAsDWORD = value.getUnsignedInteger(); // use 32 or 64 bit
        type = REG_DWORD;
        break;
      case AnyValue::LONG_LONG_INTEGER: // integer
        valueAsDWORD = value.getUnsignedInteger(); // use 64 bit
        type = REG_DWORD;
        break;
      case AnyValue::UNSIGNED_LONG_LONG_INTEGER: // integer
        valueAsDWORD = value.getUnsignedInteger(); // use 64 bit
        type = REG_DWORD;
        break;
      case AnyValue::STRING: // string
        valueAsString = value.getString();
        type = REG_SZ;
        break;
      case AnyValue::WIDE_STRING: // multibyte string (later ucs)
        valueAsString = value.getString();
        type = REG_SZ;
        break;
      }
      
      BYTE* buffer;
      DWORD size;
      
      switch (type) {
      case REG_DWORD:
        buffer = (BYTE*)&valueAsDWORD;
        break;
      case REG_SZ:
        buffer = (BYTE*)valueAsString.getElements();
        break;
      }
      
      assert(::RegSetValueEx((HKEY)key->getHandle(), // handle to key
                             name.getElements(), // value name
                             0, // reserved
                             REG_DWORD, // value type
                             buffer, // value data
                             size // size of value data
      ) == ERROR_SUCCESS, RegistryException("Unable to set value", this));
    }

    uint32 RegistryKey::getInteger(const String& name) const throw(RegistryException) {
      uint32 buffer;
      DWORD size = sizeof(buffer);
      DWORD type;
      assert(::RegQueryValueEx((HKEY)key->getHandle(),
                               name.getElements(), // value name
                               0, // reserved
                               &type, // type buffer
                               (BYTE*)&buffer, // data buffer
                               &size // size of data buffer
             ) == ERROR_SUCCESS, RegistryException(this));
      switch (type) {
      case REG_DWORD:
        return buffer; // TAG: fixme
      case REG_DWORD_BIG_ENDIAN:
        return ByteOrder::fromBigEndian<uint32>(buffer); // TAG: fixme
      default:
        throw RegistryException("Invalid value", this);
      }
    }
    
    void RegistryKey::setInteger(const String& name, uint32 value) throw(RegistryException) {
      LittleEndian<uint32> buffer = value;
      assert(::RegSetValueEx((HKEY)key->getHandle(), // handle to key
                             name.getElements(), // value name
                             0, // reserved
                             REG_DWORD, // value type
                             (BYTE*)&buffer, // value data
                             sizeof(buffer) // size of value data
      ) == ERROR_SUCCESS, RegistryException("Unable to set value", this));
    }

    String RegistryKey::getBinary(const String& name) const throw(RegistryException) {
      DWORD size;
      DWORD type;
      assert((::RegQueryValueEx((HKEY)key->getHandle(),
                                name.getElements(), // value name
                                0, // reserved
                                &type, // type buffer
                                0, // data buffer
                                &size // size of data buffer
              ) == ERROR_SUCCESS) && ((type == REG_BINARY) || (type == REG_NONE)),
             RegistryException(this)
      );

      String result; // TAG: use other type than String?
      result.forceToLength(size);
      DWORD secondType;
      assert((::RegQueryValueEx((HKEY)key->getHandle(),
                                name.getElements(), // value name
                                0, // reserved
                                &secondType, // type buffer
                                (BYTE*)result.getElements(), // data buffer
                                &size // size of data buffer
              ) == ERROR_SUCCESS) && (secondType == type),
             RegistryException(this)
      );
      return result;
    }
    
    void RegistryKey::setBinary(const String& name, const char* buffer, unsigned int size) throw(RegistryException) {
      assert(::RegSetValueEx((HKEY)key->getHandle(), // handle to key
                             name.getElements(), // value name
                             0, // reserved
                             REG_BINARY, // value type
                             (BYTE*)buffer, // value data
                             size // size of value data
      ) == ERROR_SUCCESS, RegistryException("Unable to set value", this));
    }

    String RegistryKey::getString(const String& name) const throw(RegistryException) {
      DWORD size;
      DWORD type;
      assert((::RegQueryValueEx((HKEY)key->getHandle(),
                               name.getElements(), // value name
                               0, // reserved
                               &type, // type buffer
                               0, // data buffer
                               &size // size of data buffer
              ) == ERROR_SUCCESS) && ((type == REG_SZ) || (type == REG_EXPAND_SZ)),
             RegistryException(this)
      );
      ASSERT(size >= 1); // size includes terminator
      String result;
      result.forceToLength(size - 1);
      DWORD secondType;
      assert((::RegQueryValueEx((HKEY)key->getHandle(),
                                name.getElements(), // value name
                                0, // reserved
                                &secondType, // type buffer
                                (BYTE*)result.getElements(), // data buffer
                                &size // size of data buffer
              ) == ERROR_SUCCESS) && (secondType == type),
             RegistryException(this)
      );
      return result;
    }

    void RegistryKey::setValue(const String& name, const String& value) throw(RegistryException) {
      assert(::RegSetValueEx((HKEY)key->getHandle(), // handle to key
                             name.getElements(), // value name
                             0, // reserved
                             REG_SZ, // value type
                             (BYTE*)value.getElements(), // value data
                             value.getLength() + 1 // size of value data
      ) == ERROR_SUCCESS, RegistryException("Unable to set value", this));
    }

    Array<String> RegistryKey::getStringSequence(const String& name) const throw(MemoryException, RegistryException) {
      DWORD size;
      DWORD type;
      assert((::RegQueryValueEx((HKEY)key->getHandle(),
                               name.getElements(), // value name
                               0, // reserved
                               &type, // type buffer
                               0, // data buffer
                               &size // size of data buffer
              ) == ERROR_SUCCESS) && (type == REG_MULTI_SZ), RegistryException(this));

      Allocator<char> buffer(size);
      assert((::RegQueryValueEx((HKEY)key->getHandle(),
                                name.getElements(), // value name
                                0, // reserved
                                &type, // type buffer
                                (BYTE*)buffer.getElements(), // data buffer
                                &size // size of data buffer
              ) == ERROR_SUCCESS) && (type == REG_MULTI_SZ),
             RegistryException(this));
      
      ASSERT((buffer[size - 2] == 0) && (buffer[size - 1] == 0));
      
      Array<String> result;
      const char* src = buffer.getElements();
      while (src[0] && src[1]) {
        const char* end = find<char>(src, size, 0);
        ASSERT(end);
        unsigned int length = end - src;
        String temp(end - src);
        copy<char>(temp.getElements(), src, length);
        result.append(temp);
        
        src += length + 1;
        size -= length + 1;
      }
      return result;
    }

    void RegistryKey::setValue(const String& name, const Array<String>& value) throw(RegistryException) {
      unsigned int size = 0;

      Array<String>::ReadIterator i = value.getBeginIterator();
      const Array<String>::ReadIterator end = value.getEndIterator();
      while (i < end) {
        size += i->getLength() + 1;
        ++i; // next string
      }
      size += 2; // double terminator
      
      Allocator<char> buffer(size);
      char* dest = buffer.getElements();
      i = value.getBeginIterator();
      while (i < end) {
        copy<char>(dest, i->getElements(), i->getLength() + 1); // include terminator
        dest += i->getLength() + 1;
        size += i->getLength() + 1;
        ++i; // next string
      }
      *dest++ = 0; // first terminator
      *dest++ = 0; // second terminator
      
      assert(::RegSetValueEx((HKEY)key->getHandle(), // handle to key
                             name.getElements(), // value name
                             0, // reserved
                             REG_MULTI_SZ, // value type
                             (BYTE*)buffer.getElements(), // value data
                             size // size of value data
             ) == ERROR_SUCCESS, RegistryException("Unable to set value", this));
    }
    
    void RegistryKey::flush() throw(RegistryException) {
      assert(
        ::RegFlushKey((HKEY)key->getHandle()) == ERROR_SUCCESS,
        RegistryException("Unable to remove key", this)
      );
    }

    bool RegistryKey::removeKeyRecursively(OperatingSystem::Handle key, const char* name) throw() {
      HKEY subkey;
      LONG error = ::RegOpenKeyEx((HKEY)key, // handle to open key
                                  name, // subkey name
                                  0, // reserved
                                  KEY_ALL_ACCESS, // security access mask
                                  &subkey // handle to open key
      );
      if (error != ERROR_SUCCESS) {
        return false;
      }
        
      DWORD numberOfSubkeys;
      DWORD maxLength; // excludes terminator
      error = ::RegQueryInfoKey((HKEY)subkey, // handle to key
                                0, // class buffer
                                0, // size of class buffer
                                0, // reserved
                                &numberOfSubkeys, // number of subkeys
                                &maxLength, // longest subkey name
                                0, // longest class string
                                0, // number of value entries
                                0, // longest value name
                                0, // longest value data
                                0, // descriptor length
                                0 // last write time
      );
      if (error != ERROR_SUCCESS) {
        return false;
      }
      
      char subkeyName[maxLength + 1];
      DWORD size = sizeof(subkeyName);
      
      while (numberOfSubkeys--) {
        error = ::RegEnumKeyEx((HKEY)subkey, // handle to key to enumerate
                               0, // subkey index
                               subkeyName, // subkey name
                               &size, // size of subkey buffer
                               0, // reserved
                               0, // class string buffer
                               0, // size of class string buffer
                               0 // last write time
        );
        if (error != ERROR_SUCCESS) {
          return false;
        }
        if (!removeKeyRecursively(subkey, subkeyName)) {
          return false;
        }
      }
      error = ::RegDeleteKey((HKEY)key, name);
      if (error != ERROR_SUCCESS) {
        return false;
      }
    }
    
    bool RegistryKey::removeKey(const String& name, bool force) throw(RegistryException) {
      if (force) {
        if (!removeKeyRecursively(key->getHandle(), name.getElements())) {
          return false;
        }
      }
      if (::RegDeleteKey((HKEY)key->getHandle(), name.getElements()) != ERROR_SUCCESS) {
        return false;
      }
      return true;
    }

    bool RegistryKey::removeValue(const String& name) throw(RegistryException) {
      assert(
        ::RegDeleteValue((HKEY)key->getHandle(), name.getElements()) == ERROR_SUCCESS,
        RegistryException("Unable to remove value", this)
      );
    }
    
    RegistryKey::ValueType RegistryKey::getTypeOfValue(const String& name) const throw(RegistryException) {
      DWORD type;
      assert(
        ::RegQueryValueEx((HKEY)key->getHandle(),
                          name.getElements(), // value name
                          0, // reserved
                          &type, // type buffer
                          0, // data buffer
                          0 // size of data buffer
        ) == ERROR_SUCCESS,
        RegistryException("Unable to query value", this)
      );
      switch (type) {
      case REG_BINARY:
        return RegistryKey::BINARY_VALUE;
      case REG_DWORD:
        return RegistryKey::INTEGER_VALUE;
      case REG_DWORD_BIG_ENDIAN:
        return RegistryKey::INTEGER_BE_VALUE;
      case REG_EXPAND_SZ:
        return RegistryKey::ENVIRONMENT_STRING_VALUE;
      case REG_LINK:
        return RegistryKey::LINK_VALUE;
      case REG_MULTI_SZ:
        return RegistryKey::STRING_SEQUENCE_VALUE;
      case REG_NONE:
        return RegistryKey::UNSPECIFIED_VALUE;
      case REG_QWORD:
        return RegistryKey::INTEGER_64_VALUE;
      case REG_RESOURCE_LIST:
        return RegistryKey::RESOURCE_LIST_VALUE;
      case REG_SZ:
        return RegistryKey::STRING_VALUE;
      default:
        return RegistryKey::UNSUPPORTED_VALUE;
      }
    }
    
    unsigned int RegistryKey::getSize(const String& name) const throw(RegistryException) {
      DWORD size;
      assert(::RegQueryValueEx((HKEY)key->getHandle(),
                               name.getElements(), // value name
                               0, // reserved
                               0, // type buffer
                               0, // data buffer
                               &size // size of data buffer
      ) == ERROR_SUCCESS, RegistryException("Unable to query value", this));
      return size;
    }

  }; // end of namespace win32
  
}; // end of namespace extension

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
