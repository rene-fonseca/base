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
#include <base/io/File.h>
#include <base/string/InvalidFormat.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Huffman compression.
  
  @short Huffman compression.
  @ingroup compression
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Huffman : public Object {
private:

  enum {
    CODE_SIZE = 8,
    ALPHABET_SIZE = 1 << CODE_SIZE
  };
  
  struct Node {
    /** The frequency of the symbol (and possibly linked list). */
    unsigned int frequency;
    /** The next node in the linked list. */
    unsigned int next;
    /** The length of the Huffman code. */
    unsigned int length;
    /** The Huffman code. */
    unsigned int code;
  };
  
  /** The symbols. */
  Node nodes[ALPHABET_SIZE];
public:

  /**
    Initializes the object.
  */
  Huffman() throw();

  /**
    Encodes the content of the specified buffer.
    
    @param stream The output stream.
    @param buffer The buffer.
    @param size The size of the buffer in bytes.
  */
  void encode(OutputStream& stream, const uint8* buffer, unsigned int size) throw();
  
  /**
    Decodes the content of the specified buffer.
    
    @param stream The output stream.
    @param buffer The buffer.
    @param size The size of the buffer in bytes.
  */
  void decode(OutputStream& stream, const uint8* buffer, unsigned int size) throw(InvalidFormat);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
