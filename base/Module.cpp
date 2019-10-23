/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Module.h>
#include <base/string/Format.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

ModuleManager::RegisterEntry::EntryNode* ModuleManager::RegisterEntry::nodes = nullptr;

Module::Module()
{
}

void Module::setName(const String& _name)
{
  name = _name;
}

void Module::setUrl(const String& _url)
{
  url = _url;
}

void Module::setConsumer(const String& _consumer)
{
  consumer = _consumer;
}

ModuleManager::ModuleManager()
{
}

ModuleManager& ModuleManager::getManager()
{
  return moduleManager;
}

void ModuleManager::addModule(Reference<Module> _module)
{
  modules.append(_module);
}

void ModuleManager::registerModule(const std::initializer_list<const char*>& info, const String& consumer)
{
  String prefix;
  String name;
  String version;
  String url;

  for (auto e : info) {
    String text(e);
    if (auto index = text.indexOf('=')) {
      const auto _n = text.substring(0, index);
      const auto _v = text.substring(index + 1);
      if (_n == "PREFIX") {
        prefix = _v;
      } if (_n == "NAME") {
        name = _v;
      } if (_n == "VERSION") {
        version = _v;
      } if (_n == "URL") {
        url = _v;
      } else {
        // ignore unknown names
      }
    }
  }

  registerModule(prefix + ":" + name + ":" + version, consumer, url);
}

void ModuleManager::registerModule(const String& name, const String& consumer, const String& url)
{
  ASSERT(!name.isEmpty());
  auto _module = new Module();
  _module->setName(name);
  _module->setUrl(url);
  _module->setConsumer(consumer);
  addModule(_module);
}

void ModuleManager::loadModules()
{
  auto entry = RegisterEntry::nodes;
  while (entry) {
    if (!entry->loaded) {
      entry->loaded = true; // TAG: we can also remove from linked list
      if (/*INLINE_ASSERT*/(entry->entry)) { // it is ok for module to have been unloaded
        try {
          entry->entry();
        } catch (...) {
          ASSERT(!"Failed to register module");
        }
      }
    }
    entry = entry->next;
  }
}

bool ModuleManager::isValidConsumer(const String& consumer) noexcept
{
  return !consumer.isEmpty();
}

bool ModuleManager::isValidPrefix(const String& prefix) noexcept
{
  return !prefix.isEmpty();
}

bool ModuleManager::isValidName(const String& name) noexcept
{
  return !name.isEmpty();
}

bool ModuleManager::isValidVersion(const String& version) noexcept
{
  return !version.isEmpty();
}

namespace {

  // TAG: move to JSON class
  String escape(const String& text)
  {
    String result;
    result.ensureCapacity(text.getLength() + 16 + 2);
    result += '"';
    for (char ch : text) { // TAG: need to read as UTF-8?
      // TAG: read ucs4
      ASSERT(ch <= 0x7f); // TAG: add support
      if (ch < ' ') {
        result += '\\';
        switch (ch) {
        case '\b':
          result += 'b';
          break;
        case '\f':
          result += 'f';
          break;
        case '\n':
          result += 'n';
          break;
        case '\r':
          result += 'r';
          break;
        case '\t':
          result += 't';
          break;
        default:
          result += 'u';
          result += ASCIITraits::valueToDigit(0);
          result += ASCIITraits::valueToDigit(0);
          result += ASCIITraits::valueToDigit((ch >> 4) & 0xf);
          result += ASCIITraits::valueToDigit((ch >> 0) & 0xf);
        }
      } else if (ch == '\\') {
        result += '\\';
        result += '\\';
      } else if (ch == '"') {
        result += '\\';
        result += '"';
      } else {
        result += ch;
      }
    }
    return result += '"';
  }

  String presentString(const String& text)
  {
    bool simple = true;
    for (auto ch : text) {
      if (!ASCIITraits::isAlphaNum(ch) || (ch == '_')) {
        simple = false;
        break;
      }
    }
    if (simple) {
      return text;
    }
    return escape(text);
  }
}

bool ModuleManager::traverseModules(const String& pattern)
{
  loadModules();

  // TAG: look for modules id matching pattern

  ObjectModel o;
  auto report = o.createObject();
  report->setValue(o.createString("documentType"), o.createString("modules"));
  report->setValue(o.createString("version"), o.createString("1.0"));
  auto a = o.createArray();
  report->setValue(o.createString("modules"), a);

  unsigned int count = 0;
  unsigned int recalled = 0;

  for (auto _module : modules) {
    fout << "===============================================================================" << EOL;
    
    // TAG: check version syntax - regex's
    auto subs = _module->getName().split(':');
    auto prefix = (subs.getSize() >= 1) ? subs[0] : String();
    auto name = (subs.getSize() >= 2) ? subs[1] : String();
    auto version = (subs.getSize() >= 3) ? subs[2] : String();
    auto consumer = _module->getConsumer();
    auto url = _module->getUrl();

    fout << "MODULE: " << ENDL;
    fout << "  Prefix: " << presentString(prefix);
    if (!isValidPrefix(prefix)) {
      fout << " <BAD PREFIX>";
    }
    fout << EOL;
    fout << "  Name: " << presentString(name);
    if (!isValidName(name)) {
      fout << " <BAD NAME>";
    }
    fout << EOL;
    fout << "  Version: " << presentString(version);
    if (!isValidVersion(version)) {
      fout << " <BAD VERSION>";
    }
    fout << EOL;
    fout << "  Consumer: " << (!consumer.isEmpty() ? presentString(consumer) : String("<UNKNOWN>"));
    if (!consumer.isEmpty() && !isValidConsumer(consumer)) {
      fout << " <BAD CONSUMER>";
    }
    fout << EOL;
    if (!url.isEmpty()) {
      fout << "  Url: " << url << ENDL;
    }

    String license = "MIT"; // TAG: define
    auto item = o.createObject();
    item->setValue(o.createString("prefix"), o.createString(prefix));
    item->setValue(o.createString("name"), o.createString(name));
    item->setValue(o.createString("version"), o.createString(version));
    item->setValue(o.createString("url"), o.createString(url));
    item->setValue(o.createString("consumer"), o.createString(consumer));
    item->setValue(o.createString("license"), o.createString(license));
    a->append(item);

    // TAG: connect to services to check for recalls - show reason/impact

    ++count;
    ++recalled;
  }

  fout << EOL << "===============================================================================" << EOL;
  fout << Format::subst("TOTAL RECALLED: %1/%2", recalled, count) << ENDL;

  ferr << EOL << "REPORT: " << EOL << report << ENDL;

  return !recalled;
}

ModuleManager::RegisterEntry::RegisterEntry(EntryNode* _node)
  : node(_node)
{
  if (node) {
    static unsigned int numberOfModules = 0;
    auto previous = nodes;
    node->next = previous;
    nodes = node;
    ++numberOfModules;
  }
}

ModuleManager::RegisterEntry::~RegisterEntry()
{
  if (node) {
#if 0 // we could force a load
    if (!node->loaded) {
      node->loaded = true;
      node->entry();
    }
#endif
    node->entry = nullptr; // prevent access of unloaded memory
  }
}

/*
  A module is recommended to provide a header for registration:

  #define PREFIX_MODULE_ID "com.company.module/project"
  #define PREFIX_MODULE_NAME "project"
  #define PREFIX_MODULE_VERSION "1.1.0"
  #define PREFIX_MODULE_URL "https://www.company.com/project"
  #define PREFIX_MODULE {"PREFIX=prefix", "NAME=name", "VERSION=version", "URL=url"}

  MODULE_REGISTER_EXPLICIT(THIS_MODULE, PREFIX_MODULE_ID, PREFIX_MODULE_NAME, PREFIX_MODULE_VERSION, PREFIX_MODULE_URL);
  MODULE_REGISTER(THIS_MODULE, PREFIX_MODULE);
*/

#define _ZLIB_MODULE_INFO {"PREFIX=net.zli", "NAME=zlib", "VERSION=1.2.11", "URL=https://zlib.net/"}

// define a valid version [0-9][1-9]*(\.[0-9][1-9]*(\.[0-9][1-9]*[a-zA-Z]*)?)?
#define MODULE_MAKE_VERSION(MAJOR, MINOR, MICRO, BUILD) "" MAJOR "." MINOR "." MICRO "/" BUILD // TAG: how should we handle extra version into like build

// TAG: need to be able to get report of all modules, get all licenses, get all recalled modules, get all modules with newer releases - get info on recall severity - security/privacy/critical issue/normal issue - no longer maintained
// TAG: add service for checking modules - need open API - create secret tokens - can we detect recalls from debian db

// TAG: detect stable vs unstable
// TAG: check syntax for all given data
// TAG: add build also for version
// TAG: handle static/shared linking of modules - more if module is distributed with this module

#define THIS_MODULE "com.azure.dev:base" // TAG: can we auto fill consumer module - use guid
MODULE_REGISTER(THIS_MODULE, _ZLIB_MODULE_INFO);
MODULE_REGISTER_EXPLICIT(THIS_MODULE, "net.zlib", "zlib", "1.2.11", "https://zlib.net/");
MODULE_REGISTER_EXPLICIT(THIS_MODULE, "org.openssl", "OpenSSL", "1.1.1", "https://www.openssl.org/");

// TAG: detect c/c++ library - in particular if static build
#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
MODULE_REGISTER_EXPLICIT(THIS_MODULE, "com.microsoft", "C++Runtime", "16", "https://www.microsoft.com/");
#endif

// TAG: add service url for this module - which gets asked by default - before urls for individual modules

#if 0 && defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(ModuleManager) : public UnitTest {
public:

  TEST_PRIORITY(10);

  void run() override
  {
    auto& manager = ModuleManager::getManager();
    TEST_ASSERT(manager.traverseModules()); // TAG: connect to online service to check if recalled
  }
};

TEST_REGISTER(ModuleManager);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
