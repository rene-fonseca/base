/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/SharedStrings.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if 0
template<class ENUMERATOR>
class EnumeratorToIterable {
private:

  ENUMERATOR& enumerator;
  typedef typename ENUMERATOR::Value Value;
public:

  inline EnumeratorToIterable(ENUMERATOR& _enumerator) noexcept
    : enumerator(&_enumerator)
  {
  }
  
  class Traits {
  };

  class ReadIterator : public Iterator<Traits> {
  private:
  
    ENUMERATOR& enumerator;
    const Value* value = nullptr;
  public:

    inline ReadIterator(ENUMERATOR& _enumerator) noexcept
      : enumerator(_enumerator)
    {
      if (enumerator.hasNext()) {
        value = enumerator.next();
      } else {
        value = nullptr;
      }
    }

    inline ReadIterator& operator++()
    {
      if (!value) {
        throw EndOfEnumeration();
      }
      if (enumerator.hasNext()) {
        value = enumerator.next();
      } else {
        value = nullptr;
      }
      return *this;
    }

    inline ReadIterator operator++(int)
    {
      ReadIterator result(*this);
      ++result;
      return result;
    }
    
    inline bool operator==(const ReadIterator& compare) const
    {
      return value == compare.value;
    }

    inline bool operator!=(const ReadIterator& compare) const
    {
      return value != compare.value;
    }

    inline const Value* operator->() const
    {
      return value;
    }

    inline const Value& operator*() const
    {
      return *value;
    }
  };

  inline ReadIterator begin() const noexcept
  {
    return ReadIterator(enumerator);
  }

  inline ReadIterator end() const noexcept
  {
    return ReadIterator(nullptr);
  }
};

template<class ENUMERATOR>
inline EnumeratorToIterable<ENUMERATOR> iterable(ENUMERATOR& enumerator) noexcept
{
  return EnumeratorToIterable<ENUMERATOR>(enumerator);
}

template<class TYPE>
inline EnumeratorToIterable<typename Set<TYPE>::ReadEnumerator> iterable(Set<TYPE>& container) noexcept
{
  return iterable(container.getReadEnumerator());
}

template<class TYPE>
inline EnumeratorToIterable<typename Set<TYPE>::ReadEnumerator> iterable(const Set<TYPE>& container) noexcept
{
  return iterable(container.getReadEnumerator());
}
#endif

MemorySize SharedStrings::getSize() const noexcept
{
  MemorySize count = 0;
  for (MemorySize i = 0; i < getArraySize(buckets); ++i) {
    count += buckets[i].strings.getSize();
  }
  return count;
}

void SharedStrings::clear()
{
  for (MemorySize i = 0; i < getArraySize(buckets); ++i) {
    buckets[i].strings = Set<String>();
  }
}

String SharedStrings::getString(const String& temp)
{
  if (!temp) {
    return temp; // ok since we reuse empty string already
  }
  const MemorySize i = temp.getLength() - 1; // shift to start with bucket 0
  if (i >= MAXIMUM_BUCKETS) {
    return temp; // not likely to reuse big strings
  }
  auto& strings = buckets[i].strings;
  auto added = strings.add(temp); // need to balance tree / could do bubble to top on hit
  if (added) {
    // ++performance.hits;
    return *added;
  }
  // ++performance.misses;
  if (strings.getSize() >= MAXIMUM_BUCKET_SIZE) {
#if 0
    for (auto s : strings) {
      fout << s << ENDL;
    }
#endif
    
    // TAG: use iterator to remove
    for (const auto& string : strings) {
      // we want the Node* so we can remove it directly
      if (!string.isMultiReferenced()) {
        strings.remove(string);
        break;
      }
    }
  }
  return temp;
}

void SharedStrings::garbageCollect()
{
  auto predicate = [](const String& string) {return !string.isMultiReferenced(); };
  for (MemorySize i = 0; i < getArraySize(buckets); ++i) {
    auto& strings = buckets[i].strings;
    strings.remove(predicate);
  }
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
