/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/UnitTest.h>
#include <iostream>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

UnitTest::UnitTest()
{
}

UnitTest::UnitTest(const String& _name) : name(_name)
{
}

void UnitTest::setName(const String& _name)
{
  name = _name;
}

void UnitTest::setDescription(const String& _description)
{
  description = _description;
}

void UnitTest::setSource(const String& _source)
{
  source = _source;
}

void UnitTest::run()
{
}

UnitTest::~UnitTest()
{
}

UnitTestManager::UnitTestManager()
{
}

UnitTestManager& UnitTestManager::getManager()
{
  return unitTestManager;
}

void UnitTestManager::addTest(Reference<UnitTest> test)
{
  fout << "TEST: Adding test '" << test->getName() << '"' << ENDL;
  tests.append(test);
}

bool UnitTestManager::runTest(Reference<UnitTest> test)
{
  try {
    test->run();
    ++passed;
  } catch (...) {
    ++failed;
    return false;
  }
  return true;
}

bool UnitTestManager::runTests(const String& pattern)
{
  // TAG: add support for pattern
  
  bool result = true;
  auto i = tests.getBeginIterator();
  auto end = tests.getEndIterator();
  while (i != end) {
    result &= runTest(*i);
    ++i;
  }
  return result;
}

RegisterTestInit::EntryNode* RegisterTestInit::nodes = nullptr;

RegisterTestInit::RegisterTestInit(EntryNode* node)
{
  if (node) {
    // std::cout << "ADDING TEST " << node->name << std::endl;
    
    static unsigned int numberOfTests = 0;
    auto previous = nodes;
    node->next = previous;
    nodes = node;
    ++numberOfTests;
  }
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
