/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/compression/Huffman.h>
#include <base/Cast.h>
#include <base/string/FormatOutputStream.h>
#include <base/io/FileOutputStream.h>
#include <base/io/FilterOutputStream.h>
#include <base/UnexpectedFailure.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Huffman::Huffman() throw() {
}

class FixedBlockOutputStream : public FilterOutputStream {
public:

  /** The default size of the buffer. */
  static const unsigned int DEFAULT_BUFFER_SIZE = 4096;
  /** The minimum buffer size. */
  static const unsigned int MINIMUM_BUFFER_SIZE = 1024;
  
  typedef Allocator<char>::Iterator Iterator;
protected:

  /** The buffer. */
  Allocator<char> buffer;
  /** Beginning of buffer. */
  char* begin;
  /** End of buffer. */
  const char* end;
  /** The write head. */
  char* writeHead;
public:

  FixedBlockOutputStream(OutputStream& out, unsigned int size = DEFAULT_BUFFER_SIZE, unsigned int headroom = 0) throw(BindException, MemoryException)
    : FilterOutputStream(out),
      buffer(maximum(size, MINIMUM_BUFFER_SIZE) + headroom),
      begin(buffer.getElements()),
      end(begin + size),
      writeHead(begin) {
  }
  
  inline void flush() throw(IOException) {
    FilterOutputStream::write(begin, writeHead - begin);
    writeHead = begin;
  }

  inline bool isEmpty() const throw() {
    return writeHead == begin;
  }
  
  inline unsigned int getAvailable() const throw() {
    return end - writeHead;
  }

  inline void undo(unsigned int size) throw() {
    writeHead -= size;
  }
  
  inline Iterator getWriteIterator(unsigned int size = 0) throw() {
    ASSERT(size <= getAvailable());
    char* previous = writeHead;
    writeHead += size;
    return Iterator(previous);
  }
  
  inline void write(char value) throw(IOException) {
    if (writeHead == end) {
      flush();
    }
    *writeHead++ = value;
  }

  inline void unfoldValue(char value, unsigned int size) throw() {
    // fill internal buffer if possible
    unsigned int bytesToWrite = minimum(size, getAvailable()); // could be 0
    fill(writeHead, bytesToWrite, value);
    writeHead += bytesToWrite;
    size -= bytesToWrite;

    if (size) {
      flush();
      
      // write block directly from external buffer
      if (size/(end - begin)) {
        unsigned int blocksToWrite = size/(end - begin);
        size -= blocksToWrite*(end - begin);
        fill(begin, (end - begin) - bytesToWrite, value); // init uninitialized elements in buffer
        while (blocksToWrite--) {
          FilterOutputStream::write(begin, end - begin);
        }
      }
      
      // copy to internal buffer
      ASSERT((size < getAvailable()) && (getAvailable() == (end - begin)));
      writeHead += size; // elements already initialized
    }
  }
  
  void write(const char* buffer, unsigned int size) throw(IOException) {
    // fill internal buffer if possible
    unsigned int bytesToCopy = minimum(size, getAvailable()); // could be 0
    copy(writeHead, buffer, bytesToCopy);
    writeHead += bytesToCopy;
    buffer += bytesToCopy;
    size -= bytesToCopy;
    
    if (size) {
      flush();
      
      // write block directly from external buffer
      unsigned int bytesToWrite = (size/(end - begin))*(end - begin);
      FilterOutputStream::write(buffer, bytesToWrite);
      buffer += bytesToWrite;
      size -= bytesToWrite;
      
      // copy to internal buffer
      ASSERT((size < getAvailable()) && (getAvailable() == (end-begin)));
      copy(writeHead, buffer, size);
      writeHead += size;
    }
  }
  
  ~FixedBlockOutputStream() throw(IOException) {
    flush();
  }
};



class Encoder {
private:

  enum {
    CODE_SIZE = 8,
    ALPHABET_SIZE = 1 << CODE_SIZE
  };

  struct Node {
    /** The frequency of the symbol. */
    unsigned int frequency;
    /** The frequency of the linked list. */
    unsigned int listFrequency;
    /** The next node in the linked list. */
    unsigned int next;
    /** The length of the Huffman code. */
    unsigned int length;
    /** The Huffman code. */
    unsigned int code;
  };
  
  FixedBlockOutputStream os;

  /** The symbols. */
  Node nodes[ALPHABET_SIZE];
  /** The number of symbols for each code length. */
  unsigned int numberOfSymbols[ALPHABET_SIZE];
public:

  void updateCodeLengths(const uint8* buffer, unsigned int size) throw() {
    fill<char>(Cast::getCharAddress(nodes), sizeof(nodes), 0);
    
    // mark end of linked lists
    for (int i = ALPHABET_SIZE - 1; i >= 0; --i) {
      nodes[i].next = i;
    }
    
    // determine frequencies
    const uint8* src = buffer;
    const uint8* end = src + size;
    while (src < end) {
      nodes[*src++].frequency++; // this would be the proper place to calculate a 32 bit CRC
    }
    
    unsigned int lists[ALPHABET_SIZE]; // linked lists
    
    // insert nodes in sorted list (frequency)
    int lastNode = ALPHABET_SIZE - 1;
    for (; (lastNode >= 0) && (nodes[lastNode].frequency == 0); --lastNode); // find non-zero frequency
    ASSERT(lastNode >= 0); // size > 0 => lastNode >= 0
    
    unsigned int lastList = 0; // the index of the last linked list
    lists[lastList] = lastNode;
    for (int node = lastNode - 1; node >= 0; --node) {
      const unsigned int frequency = nodes[node].frequency;
      nodes[node].listFrequency = frequency;
      
      if (frequency == 0) {
        continue; // ignore symbols with zero frequency
      }
      
      // find the proper place
      unsigned int emptySlot = ++lastList;
      for (; (emptySlot > 0) && (frequency < nodes[lists[emptySlot - 1]].listFrequency); --emptySlot) {
        lists[emptySlot] = lists[emptySlot - 1]; // bubble down empty slot
      }
    
      lists[emptySlot] = node; // insert new linked list
    }
    
    // merge 2 lowest frequencies until only one list is left
    for (unsigned int list = 0; list < lastList; ++list) {
      const unsigned int leftChild = lists[list];
      const unsigned int rightChild = lists[list + 1];
      
      // TAG: use inc/dec for lengths (need to store end of list - but only update end after sorted)
      
      unsigned int node = rightChild;
      while (true) {
        nodes[node].length++;
        if (node == nodes[node].next) {
          break;
        }
        node = nodes[node].next;
      }
      
      nodes[node].next = leftChild; // merge lists
      
      node = leftChild;
      while (true) {
        nodes[node].length++;
        if (node == nodes[node].next) {
          break;
        }
        node = nodes[node].next;
      }
      
      const unsigned int frequency = nodes[leftChild].listFrequency + nodes[rightChild].listFrequency;
      nodes[rightChild].listFrequency = frequency;
      
      // sort lists - partial bubble sort - could do a binary search
      for (unsigned int i = list + 1; (i < lastList) && (frequency > nodes[lists[i + 1]].listFrequency); ++i) {
        swapper(lists[i + 1], lists[i]);
      }
    }
  }

  void generateCodes() throw() {
    // determine the canonical Huffman codes
    
    // count number of codes per code length
    fill<unsigned int>(numberOfSymbols, getArraySize(numberOfSymbols), 0);
    for (unsigned int node = 0; node < ALPHABET_SIZE; ++node) {
      numberOfSymbols[nodes[node].length]++;
    }
    
    unsigned int currentCodes[ALPHABET_SIZE]; // maximum code length is ALPHABET_SIZE - 1
    {
      unsigned int length = getArraySize(currentCodes) - 1;
      while (numberOfSymbols[length] == 0) {
        --length;
      }
      
      unsigned int code = 0;
      unsigned int lastNumberOfCodes = 0;
      while (length > 0) {
        code = (code + lastNumberOfCodes) >> 1;
        currentCodes[length] = code;
        lastNumberOfCodes = numberOfSymbols[length];
        --length;
      }
    }
    
    // assign codes to symbols
    for (unsigned int symbol = 0; symbol < ALPHABET_SIZE; ++symbol) {
      // nodes[symbol].length could be 0
      nodes[symbol].code = currentCodes[nodes[symbol].length]++;
    }
    
//     for (unsigned int symbol = 0; symbol < ALPHABET_SIZE; ++symbol) {
//       if (nodes[symbol].length == 0) {
//         continue;
//       }
//       fout << MESSAGE("symbol=") << symbol << MESSAGE(" length=") << nodes[symbol].length << MESSAGE(" code=") << BIN << NOPREFIX << ZEROPAD << setWidth(nodes[symbol].length) << nodes[symbol].code << EOL;
//     }
//     fout << ENDL;
  }
  
  void externalizeSymbolTable() throw() {
    // reduce code length range
    unsigned int minimumLength = 1;
    while (numberOfSymbols[minimumLength] == 0) {
      ++minimumLength;
    }
    
    // translate counts into initial offsets in the sorted symbol table (below)
    unsigned int offsets[ALPHABET_SIZE];
    unsigned int offset = 0; // skip symbols with frequency 0
    unsigned int length = minimumLength;
    const unsigned int presentSymbols = ALPHABET_SIZE - numberOfSymbols[0];
    while (offset < presentSymbols) {
      offsets[length] = offset;
      offset += numberOfSymbols[length];
      ++length;
    }
    offsets[0] = offset; // write symbols with frequency 0 at end of sorted symbol table
    unsigned int maximumLength = length - 1;
    
    unsigned int sortedSymbols[ALPHABET_SIZE];
    unsigned int encodedBits = 0;
    for (unsigned int symbol = 0; symbol < ALPHABET_SIZE; ++symbol) {
      if (nodes[symbol].length) {
        sortedSymbols[offsets[nodes[symbol].length]++] = symbol; // length = 0 allowed
        encodedBits += (nodes[symbol].frequency % 8) * nodes[symbol].length; // overflow not possible
      }
    }
    unsigned int garbageBits = (8 - encodedBits % 8) % 8;

    uint8 header[1 + 1 + (ALPHABET_SIZE-1) + ALPHABET_SIZE + 1]; // min len + max len + lengths + symbols + garbage bits
    uint8* dest = header;
    
//     bool used[ALPHABET_SIZE]; // use for symbol prediction
//     fill(used, getArraySize(used), false);
    
    {
      unsigned int offset = 0;
      *dest++ = minimumLength;
      *dest++ = maximumLength;
      for (unsigned int length = minimumLength; length <= maximumLength; ++length) {
        *dest++ = numberOfSymbols[length];
        for (unsigned int symbol = numberOfSymbols[length]; symbol > 0; --symbol) {
          *dest++ = sortedSymbols[offset++]; // TAG: check prediction sequence here

//           // apply prediction filter
//           unsigned int prediction = symbol;
//           while ((prediction < ALPHABET_SIZE) && (!used[prediction])) {
//             ++prediction;
//           }
//           if ((prediction - symbol) > 2) { // we need a prediction sequence of minimum 3 symbols
//             // find the longest run-length which can be represented by some seen symbol
//             // write prediction sequence length (offset in sorted table) and first symbol of sequence
//           }
//          used[sortedSymbols[offset++]] = true; // mark symbol as seen
        }
      }
      *dest++ = garbageBits;
    }

    ASSERT((dest - header) <= getArraySize(header));
    
    os.write(Cast::getCharAddress(header), dest - header);
  }

  Encoder(OutputStream& stream, const uint8* buffer, unsigned int size) throw(IOException)
    : os(stream) {
    
    updateCodeLengths(buffer, size);
    generateCodes();
    externalizeSymbolTable();
    
    const uint8* src = buffer;
    const uint8* end = src + size;
    
    const unsigned int BUFFER_SIZE = sizeof(unsigned long) * 8;
    
    unsigned long bitBuffer = 0;
    unsigned int availableBits = BUFFER_SIZE;
    
    while (src < end) {
      unsigned int code = nodes[*src].code;
      unsigned int length = nodes[*src].length;

      if (length <= availableBits) {
        bitBuffer <<= length;
        bitBuffer |= code;
        availableBits -= length;
      } else {
        
        unsigned int bits = length;
        if (bits > 8) { // store zero bits
          unsigned int zeroBits = bits - 8;
          bits -= zeroBits;
          if (zeroBits <= availableBits) {
            bitBuffer <<= zeroBits;
            availableBits -= zeroBits;
          } else {
            bitBuffer <<= availableBits;
            zeroBits -= availableBits;
            availableBits = 0;
            
            // flush bit buffer
            for (unsigned int i = 1; i <= sizeof(bitBuffer); ++i) {
              os.write(bitBuffer >> ((sizeof(bitBuffer) - i) * 8));
              availableBits += 8;
            }
            
            os.unfoldValue(0, zeroBits/8);
            
            // write remaining zero bits
            bitBuffer = 0;
            availableBits -= zeroBits%8;
          }
        }
        
        ASSERT(bits <= 8);
        
        if (bits <= availableBits) {
          bitBuffer <<= bits;
          bitBuffer |= code;
          availableBits -= bits;
        } else {
          bitBuffer <<= availableBits;
          bits -= availableBits;
          bitBuffer |= code >> bits; // store most significant bits
          availableBits = 0;
          
          // flush bit buffer
          for (unsigned int i = 1; i <= sizeof(bitBuffer); ++i) {
            os.write(bitBuffer >> ((sizeof(bitBuffer) - i) * 8));
            availableBits += 8;
          }
          
          bitBuffer = code; // store least significant bits (buffer just flushed)
          availableBits -= bits;
        }
      }
      
      ++src;
    }
    
    // flush anything left in buffer
    if (availableBits < (sizeof(bitBuffer) * 8)) {
      bitBuffer <<= availableBits;
      for (unsigned int i = 1; i <= (sizeof(bitBuffer) - availableBits/8); ++i) {
        os.write(bitBuffer >> ((sizeof(bitBuffer) - i) * 8));
      }
    }
  }
  
};

void Huffman::encode(OutputStream& stream, const uint8* buffer, unsigned int size) throw() {
  Encoder encoder(stream, buffer, size);
}

class Decoder {
private:

  enum {
    CODE_SIZE = 8,
    ALPHABET_SIZE = 1 << CODE_SIZE
  };

  /** Symbol descriptor. */
  struct Symbol {
    unsigned int length;
  };

  /** Output stream. */
  FixedBlockOutputStream os;
  /** Lookup table for number of symbols per code length. */
  unsigned int numberOfSymbols[ALPHABET_SIZE];
  /** Specifies the symbols. */
  Symbol symbols[ALPHABET_SIZE];
  /** The start code for a given code length. */
  unsigned int startCodes[ALPHABET_SIZE];
  /** The end code for a given code length. */
  unsigned int endCodes[ALPHABET_SIZE];
  /** The least significant bits of the code. */
  unsigned int codes[ALPHABET_SIZE];
  /** Lookup table for offsets into the codes lookup table. */
  int offsets[ALPHABET_SIZE];
  /** Specifies the minimum code length. */
  unsigned int minimumLength;
  /** Specifies the maximum code length. */
  unsigned int maximumLength;
  /** The number of different symbols in use. */
  unsigned int totalNumberOfSymbols;
  /** The number of garbage bits at end of buffer. */
  unsigned int garbageBits;
public:

  inline unsigned int readSymbols(const uint8* buffer, unsigned int size) throw(InvalidFormat) {
    fill<char>(Cast::getCharAddress(symbols), sizeof(symbols), 0);
    
    const uint8* src = buffer;
    const uint8* end = src + size;
    totalNumberOfSymbols = 0;
    
    assert(src < end, InvalidFormat("Invalid header", this));
    minimumLength = *src++;
    assert(src < end, InvalidFormat("Invalid header", this));
    maximumLength = *src++;
    assert((minimumLength <= maximumLength) && (maximumLength <= (ALPHABET_SIZE - 1)), InvalidFormat(this));

    for (unsigned int length = minimumLength; length <= maximumLength; ++length) {
      assert(src < end, InvalidFormat("Invalid header", this));
      numberOfSymbols[length] = *src++;
      assert((src + numberOfSymbols[length]) < end, InvalidFormat("Invalid header", this));
      totalNumberOfSymbols += numberOfSymbols[length];
      for (unsigned int i = numberOfSymbols[length]; i > 0; --i) {
        unsigned int symbol = *src++;
        assert(symbols[symbol].length == 0, InvalidFormat("Invalid header", this)); // is symbol already read
        symbols[symbol].length = length;
      }
    }

    assert(src < end, InvalidFormat("Invalid header", this));
    garbageBits = *src++;
    
    // reduce code length range if stupid
    while (numberOfSymbols[minimumLength] == 0) {
      ++minimumLength;
    }
    while (numberOfSymbols[maximumLength] == 0) {
      --maximumLength;
    }
    
    return src - buffer; // success
  }
  
  inline void generateCodes() throw() {
    // determine the canonical Huffman codes

    unsigned int currentCodes[ALPHABET_SIZE]; // maximum code length is ALPHABET_SIZE - 1
    unsigned int code = 0; // start code for longest code length
    unsigned int lastNumberOfCodes = 0;
    int offset = 0;
    for (unsigned int length = maximumLength; length >= minimumLength; --length) {
      code = (code + lastNumberOfCodes) >> 1;
      currentCodes[length] = code; // store start code for this length
      lastNumberOfCodes = numberOfSymbols[length];
      
      startCodes[length] = code;
      endCodes[length] = code + numberOfSymbols[length] - 1;
      
      offsets[length] = offset - code; // offset in code lookup table
      offset += numberOfSymbols[length];
    }
    
    // assign codes to symbols
    for (unsigned int symbol = 0; symbol < ALPHABET_SIZE; ++symbol) {
      if (symbols[symbol].length) {
        unsigned int code = currentCodes[symbols[symbol].length]++; // length could be 0
        // symbols[symbol].code = code;
        codes[code + offsets[symbols[symbol].length]] = symbol; // add symbol to code lookup table
      }
    }
  }
  
  Decoder(OutputStream& stream, const uint8* buffer, unsigned int size) throw(InvalidFormat)
    : os(stream) {
    const uint8* src = buffer;
    const uint8* end = src + size;
    
    src += readSymbols(src, size);
    generateCodes();
    
    unsigned long bitBuffer = 0;
    unsigned int availableBits = 0;
    const uint8* wordEnd = src + (((end - src) * 8 - garbageBits)/(sizeof(bitBuffer) * 8)) * sizeof(bitBuffer);
    
    ASSERT(minimumLength <= 8);
    assert(minimumLength <= 8, UnexpectedFailure());
    
    // TAG: should be optimized (use initial 8 bit lookup table to predict length of code - garbage does not matter)
    while (true) {
      unsigned int length = minimumLength;
      bitBuffer &= (1 << availableBits) - 1; // mask out garbage bits
      unsigned int code;
      
      // read minimum length bits
      if (availableBits >= length) {
        availableBits -= length;
        code = bitBuffer >> availableBits;
      } else {
        const unsigned int count = length - availableBits; // required number of bits
        code = bitBuffer;
        availableBits = 0;
        
        if (src >= wordEnd) {
          if ((src == end) && (count == length)) {
            return; // we are done
          }
          assert(src < end, InvalidFormat("Invalid Huffman code", this));
          while (src < end) {
            bitBuffer <<= 8;
            bitBuffer |= *src++;
            availableBits += 8;
          }
          bitBuffer >>= garbageBits;
          availableBits -= garbageBits;
          assert(availableBits >= count, InvalidFormat("Invalid Huffman code", this));
        } else {
          for (unsigned int i = sizeof(bitBuffer); i > 0; --i) { // fill buffer
            bitBuffer <<= 8;
            bitBuffer |= *src++;
            availableBits += 8;
          }
        }
        
        availableBits -= count;
        code = (code << count) | (bitBuffer >> availableBits);
      }
      
      while (true) {
        if ((code >= startCodes[length]) && (code <= endCodes[length])) { // TAG: must be false if no symbols for length
          os.write(codes[code + offsets[length]]);
          break;
        }
        
        if (availableBits == 0) { // do we need to fill the bit buffer
          if (src >= wordEnd) {
            assert(src < end, InvalidFormat("Invalid Huffman code", this));
            while (src < end) {
              bitBuffer <<= 8;
              bitBuffer |= *src++;
              availableBits += 8;
            }
            bitBuffer >>= garbageBits;
            availableBits -= garbageBits;
            assert(availableBits, InvalidFormat("Invalid Huffman code", this));
          } else {
            for (unsigned int i = sizeof(bitBuffer); i > 0; --i) {
              bitBuffer <<= 8;
              bitBuffer |= *src++;
              availableBits += 8;
            }
          }
        }
        --availableBits;
        code = (code << 1) | (bitBuffer >> availableBits) & 1; // get one bit
        
        ++length;
        assert(length <= maximumLength, InvalidFormat("Invalid Huffman code", this));
      }
    }
  }
  
};

void Huffman::decode(OutputStream& stream, const uint8* buffer, unsigned int size) throw(InvalidFormat) {
  Decoder decoder(stream, buffer, size);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
