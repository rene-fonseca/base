/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__XML_TREE_H
#define _DK_SDU_MIP__BASE_XML__XML_TREE_H

#include <base/Object.h>
#include <base/xml/XMLNode.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  XML tree.

  @short XML tree.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class XMLTree : public Object {
friend class XMLTreeImpl;
private:

  /** Specifies that the XML tree has been initialized. */
  bool initialized;
  /** The root node of the XML tree. */
  XMLNode* root;

  void release() throw();
public:

  /**
    Initializes empty XML tree.
  */
  XMLTree() throw();

//  /**
//    Initializes XML tree from XML file.
//
//    @param filename The file name of the XML file.
//  */
//  XMLTree(const String& filename) throw();

  /**
    Initializes XML tree from other XML tree.
  */
  XMLTree(const XMLTree& copy) throw();

  /**
    Returns true if the XML tree has been initialized.
  */
  inline bool isInitialized() const throw() {
    return initialized;
  }

  /**
    Returns the root of the XML tree.
  */
  inline XMLNode* getRoot() const throw() {
    return root;
  }

  /**
    Sets the root of the XML tree.
  */
  void setRoot(XMLNode* node) throw();

  /**
    Reads the XML tree from the specified string.
  */
  bool read(const String& buffer) throw(MemoryException);

  /**
    Write the XML tree to the specified string.
  */
  bool write(String& buffer) const throw(MemoryException);

  /**
    Destroys the XML tree.
  */
  ~XMLTree() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
