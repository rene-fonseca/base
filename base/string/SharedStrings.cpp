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

#if 1
template<class ENUMERATOR>
class EnumeratorToIterable {
private:

  ENUMERATOR& enumerator;
  typedef typename ENUMERATOR::Value Value;
public:

  inline EnumeratorToIterable(ENUMERATOR& _enumerator) noexcept
    : enumerator(_enumerator)
  {
  }

  class ReadIterator {
  private:
  
    ENUMERATOR& enumerator;
    const Value* value = nullptr;
  public:

    inline ReadIterator(ENUMERATOR& _enumerator) noexcept
      : enumerator(_enumerator)
    {
    }

    inline ReadIterator& operator++()
    {
      if (enumerator.hasNext()) {
        value = enumerator.next();
      } else {
        value = nullptr;
      }
      return *this;
    }

#if 0
    inline bool operator==(const ReadIterator& compare) const
    {
      return !enumerator.hasNext();
    }

    inline bool operator!=(const ReadIterator& compare) const
    {
      return enumerator.hasNext();
    }
#endif

    inline bool operator==(std::nullptr_t) const
    {
      return !enumerator.hasNext();
    }

    inline bool operator!=(std::nullptr_t) const
    {
      return enumerator.hasNext();
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

  inline std::nullptr_t end() const noexcept
  {
    return nullptr;
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
    for (auto s : iterable(strings)) {
      fout << "!!!! s " << s << ENDL;
    }

    auto e = strings.getEnumerator(); // we want the Node* so we can remove it directly
    while (e.hasNext()) {
      String* s = e.next();
      if (!s->isMultiReferenced()) {
        strings.remove(*s);
        break;
      }
    }
  }
  return temp;
}

void SharedStrings::garbageCollect()
{
  // optimize using Set::remove(predicate)
  auto predicate = [](const String& string) {return string.isMultiReferenced(); };

  for (MemorySize i = 0; i < getArraySize(buckets); ++i) {
    auto& strings = buckets[i].strings;
    auto e = strings.getEnumerator(); // we want the Node* so we can remove it directly
    while (e.hasNext()) {
      String* s = e.next();
      if (!predicate(*s)) {
        strings.remove(*s);
      }
    }
  }
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
