/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/mem/Allocator.h>
#include <base/mem/ReferenceCountedAllocator.h>
#include <base/string/Format.h>
#include <base/collection/Functor.h>
#include <base/ValidifiedResult.h>

using namespace com::azure::dev::base;

template<class TYPE>
class Validate {
private:

  TYPE value;
public:
  
  inline Validate(const TYPE& _value, const TYPE& expected)
    : value(_value) {
    if (_value != expected) {
      ferr << "Error: expected " << expected << " but got " << value << ENDL;
    }
  }

  inline operator const TYPE&() const {
    return value;
  }

  inline FormatOutputStream& operator<<(FormatOutputStream& stream) const
  {
    return stream << static_cast<const TYPE&>(value);
  }
};

template<class TYPE>
inline FormatOutputStream& operator<<(FormatOutputStream& stream, const Validate<TYPE>& value)
{
  return value.operator<<(stream);
}

/** Invert case of character. */
class InvertCase : public UnaryOperation<char, char> {
public:
  inline char operator()(char value) const {
    if ((value >= 'A') && (value <= 'Z')) {
      return value - 'A' + 'a';
    } else if ((value >= 'a') && (value <= 'z')) {
      return value - 'a' + 'A';
    } else {
      return value;
    }
  }
};



class StringApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  StringApplication()
    : Application("String")
  {
  }
  
  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;
    
    Allocator<char> a(1234);
    fout << "Size of Allocator<char> (a): " << a.getSize() << ENDL;
    Allocator<char>::ReadEnumerator enua = a.getReadEnumerator();
    unsigned int counta = 0;
    while (enua.hasNext()) {
      enua.next();
      ++counta;
    }
    fout << "Counted number of elements in (a): " << counta << ENDL;
    fout << ENDL;

    ReferenceCountedAllocator<char> ra(4321);
    fout << "Size of ReferenceCountedAllocator<char> (ra): " << ra.getSize() << ENDL;
    ReferenceCountedAllocator<char>::ReadEnumerator enura = ra.getReadEnumerator();
    unsigned int countra = 0;
    while (enura.hasNext()) {
      enura.next();
      ++countra;
    }
    fout << "Counted number of elements in (ra): " << countra << ENDL;
    fout << ENDL;

    ValidifiedResult<uint64> test1(12345678);
    fout << "ValidifiedResult<uint64> " << test1 << ENDL;

    ReferenceCountedAllocator<char> rb(1234/*, 256*/);
    fout << "ReferenceCountedAllocator<char>::getSize(): "
         << rb.getSize() << ENDL;
    // fout << "ReferenceCountedAllocator<char>::getGranularity(): " << rb.getGranularity() << ENDL;
    fout << "ReferenceCountedAllocator<char>::getCapacity(): "
         << rb.getCapacity() << ENDL;
    ReferenceCountedAllocator<char>::ReadEnumerator enurb =
      rb.getReadEnumerator();
    unsigned int countrb = 0;
    while (enurb.hasNext()) {
      enurb.next();
      ++countrb;
    }
    fout << "Counted number of elements in (rb): " << countrb << ENDL;
    rb.garbageCollect();
    fout << "ReferenceCountedAllocator<char>::getCapacity(): "
         << rb.getCapacity() << ENDL;
    fout << ENDL;

    fout << "Initializing empty string str1" << ENDL;
    String str1;
    fout << "Length of str1: " << str1.getLength() << ENDL;
    fout << "Value of str1: " << str1 << ENDL;

    fout << "Explicit initialization of string str2" << ENDL;
    String str2 = "Hello, World!";
    fout << "Length of str2: " << str2.getLength() << ENDL;
    fout << "Value of str2: " << str2 << ENDL;

    fout << "Inverting case of characters" << ENDL;
    InvertCase ii;
    transform(str2, ii);
    fout << "str2: " << str2 << ENDL;

    fout << "Concatenation: " << String("first") + String("SECOND") << ENDL;
    fout << ENDL;

    String string("This is a string");
    const char* temp1 = string.getElements();
    fout << "Testing direct read access (This is a string): "
         << temp1 << ENDL;

    char* temp2 = string.getElements();
    fout << "Testing direct read and write access (This is a string): "
         << temp2 << ENDL;

    fout << "Test: " << getFormalName() << ENDL;
    
    fout << "String(\"prefix suffix\").startsWith(\"prefix\"): "
         << Validate<bool>(String("prefix suffix").startsWith("prefix"), true) << ENDL;
    fout << "String(\"prefix suffix\").startsWith(\"suffix\"): "
         << Validate<bool>(String("prefix suffix").startsWith("suffix"), false) << ENDL;
    fout << "String(\"prefix suffix\").endsWith(\"prefix\"): "
         << Validate<bool>(String("prefix suffix").endsWith("prefix"), false) << ENDL;
    fout << "String(\"prefix suffix\").endsWith(\"suffix\"): "
         << Validate<bool>(String("prefix suffix").endsWith("suffix"), true) << ENDL;
    
    fout << String("String literal") << ENDL;
    
    fout << "String literal" << ENDL;

    fout << "Substitution OK: " << Format::subst(MESSAGE("My name is %1 and I'm %2 years old."), "John", 18) << ENDL;
    fout << "Substitution OK: " << Format::subst(MESSAGE("Current percent is %1%%."), 45) << ENDL;
    fout << "Substitution OK: " << Format::subst(MESSAGE("My name is %1 %2 and I'm %3 years old. You can call me %1."), "John", "Doe", 18) << ENDL;
    fout << "Substitution BAD: " << Format::subst(MESSAGE("Get invalid argument %111."), "John", 18) << ENDL;
    fout << "Substitution BAD: " << Format::subst(MESSAGE("Request substitution index 0 is bad (%0)."), MESSAGE("Note")) << ENDL;
    fout << "Substitution BAD: " << Format::subst(MESSAGE("Request substitution index 0 is bad (%0)."), "Note") << ENDL;
    // fout << "Substitution BAD: " << Format::subst(MESSAGE("Bad substitution string %.")) << ENDL;
    // fout << "Substitution BAD: " << Format::subst(MESSAGE("Bad substitution string %")) << ENDL;
    fout << "Substitution BAD: " << Format::subst(MESSAGE("My full name is %1 %2 and I'm %3 years old."), String("John"), String("Doe")) << ENDL;
    fout << "Substitution OK: " << Format::subst(MESSAGE("My full name is %1 %2 and I like %3 but not %4."), MESSAGE("John"), MESSAGE("Doe"), MESSAGE("football"), MESSAGE("tennis")) << ENDL;



    fout << "Formatting 1: " << Format::subst(
      "The value is %1 = %2 = %3.",
      format() << 12354, format() << HEX << 12354, format() << ZEROPAD << setWidth(10) << 12354
    ) << ENDL;

    format f;
    fout << "Formatting 2: " << Format::subst(
      "The value is %1 = %2 = %3.",
      f << 12354, f << HEX << 12354, f << ZEROPAD << setWidth(10) << 12354
    ) << ENDL;

  }
};

APPLICATION_STUB(StringApplication);
