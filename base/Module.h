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

#include <base/collection/Array.h>

#if defined(MODULE_REGISTER)
#  error MODULE_REGISTER already defined
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Module.
*/
class _COM_AZURE_DEV__BASE__API Module : public ReferenceCountedObject {
public:

  enum Impact {
    PRIVACY = 1 << 0,
    SECURITY = 1 << 1,
    CRITICAL = 1 << 2,
    COSMETIC = 1 << 31
  };
private:

  /** Fully qualified id. */
  String id;
  /** Url. */
  String url;
  /** Consumer. */
  String consumer;
  /** License. */
  String license;
  /** Description. */
  String description;
public:

  /** Returns true if the name is valid ID => (PREFIX:NAME:VERSION). */
  static bool isFullyQualifiedId(const String& id) noexcept;

  /** Returns true if the consumer is valid CONSUMER => (PREFIX:NAME). */
  static bool isValidConsumer(const String& consumer) noexcept;

  /** Returns true if the prefix is valid PREFIX => (Guid || ([a-zA-Z0-9_]+(.[a-zA-Z0-9_]+)*)). */
  static bool isValidPrefix(const String& prefix) noexcept;

  /** Returns true if the name is valid NAME => ([a-zA-Z0-9_]+). */
  static bool isValidName(const String& name) noexcept;

  /** Returns true if the version is valid. VERSION => ((0 || [1-9][0-9]*)(.(0 || [1-9][0-9]*))*). */
  static bool isValidVersion(const String& version) noexcept;

  /** Compares the given versions. */
  static int compareVersions(const String& a, const String& b) noexcept;

  /** Initializes the module. */
  Module();

  /** Sets the id of the module. */
  void setId(const String& id);
  
  /** Sets the url for the module. */
  void setUrl(const String& url);
  
  /** Sets the consumer file of the module. */
  void setConsumer(const String& consumer);

  /** Sets the license of the module. */
  void setLicense(const String& license);

  /** Sets the description of the module. */
  void setDescription(const String& description);

  /** Returns the fully qualified id (PREFIX:NAME:VERSION) of the module. */
  inline const String& getId() const noexcept
  {
    return id;
  }

  /** Returns the url of the module. */
  inline const String& getUrl() const noexcept
  {
    return url;
  }

  /** Returns the consumer module of the module. */
  inline const String& getConsumer() const noexcept
  {
    return consumer;
  }

  /** Returns the license of the module. */
  inline const String& getLicense() const noexcept
  {
    return license;
  }

  /** Returns the description of the module. */
  inline const String& getDescription() const noexcept
  {
    return description;
  }
};

/** Module manager. */
class _COM_AZURE_DEV__BASE__API ModuleManager {
  friend class Module;
private:
  
  Array<Reference<Module> > modules;

  static ModuleManager moduleManager;
public:

  ModuleManager();
  
  static ModuleManager& getManager();

  /** Adds the given module. */
  void addModule(Reference<Module> module);

  /** Create module instance and adds it. */
  void registerModule(const std::initializer_list<const char*>& info, const String& consumer = String());

  /** Create module instance and adds it. */
  void registerModule(const String& id,
                      const String& consumer = String(),
                      const String& url = String(),
                      const String& license = String(),
                      const String& description = String());

  /** Loads the registered modules. */
  void loadModules();

  /** Runs the modules matching the given pattern. */
  bool traverseModules(const String& pattern = "*");

  /** Returns all the registered modules. */
  inline const Array<Reference<Module> >& getModules() const
  {
    return modules;
  }

  /** Internal module registration helper. */
  class _COM_AZURE_DEV__BASE__API RegisterEntry {
    friend class ModuleManager;
  public:

    typedef void (*Entry)();

    struct EntryNode {
      Entry entry; // entry function for module registration
      EntryNode* next; // next entry function node
      bool loaded; // indicates that module has been registered
    };
  private:

    EntryNode* node = nullptr;

    /** The last module registered. This is a pointer so it is always ready for use during global initialization. */
    static EntryNode* nodes;
  public:

    /** Records module for delayed registration. */
    RegisterEntry(EntryNode* entry);

    ~RegisterEntry();
  };
};

/** Returns initializer list for use with MODULE_REGISTER. Version is a list of integers separated by a dot of maximum value 999999999. */
#define MODULE_MAKE_INITIALIZER(PREFIX, NAME, VERSION, URL) \
  {"PREFIX=" PREFIX, "NAME=" NAME, "VERSION=" VERSION, "URL=" URL}

/**
  Registers module using initializer list. Takes an initializers of string
  literals with name/value pairs separated by =. Current PREFIX, NAME,
  VERSION, and URL are defined. You can use "_" to ensure name won't conflict
  with future name identifiers used by Base framework.
*/
#define MODULE_REGISTER(THIS_MODULE, MODULE) \
  namespace _COM_AZURE_DEV__BASE__MAKE_IDENTIFIER(_modules_) { \
    void _entry() { base::ModuleManager::getManager().registerModule(MODULE, THIS_MODULE); } \
    base::ModuleManager::RegisterEntry::EntryNode _storage = {_entry, nullptr, false}; \
    base::ModuleManager::RegisterEntry _register(&_storage); \
  }

#define MODULE_REGISTER_EXPLICIT_IMPL(THIS_MODULE, MODULE, URL) \
  namespace _COM_AZURE_DEV__BASE__MAKE_IDENTIFIER(_modules_) { \
    void _entry() { base::ModuleManager::getManager().registerModule(MODULE, THIS_MODULE, URL); } \
    base::ModuleManager::RegisterEntry::EntryNode _storage = {_entry, nullptr, false}; \
    base::ModuleManager::RegisterEntry _register(&_storage); \
  }

/** Fast registration of module for later run. Module will be put in a unique subnamespace. */
#define MODULE_REGISTER_EXPLICIT(THIS_MODULE, PREFIX, MODULE, VERSION, URL) \
  MODULE_REGISTER_EXPLICIT_IMPL(THIS_MODULE, PREFIX ":" MODULE ":" VERSION, URL)

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
