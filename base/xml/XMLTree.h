/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__XML_TREE_H
#define _DK_SDU_MIP__BASE_XML__XML_TREE_H

#include <base/Object.h>
#include <base/xml/XMLNode.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @short XML tree.
  @author René Møller Fonseca
*/

class XMLTree : public Object {
friend class XMLTreeImpl;
private:

  /** Specifies that the XML tree has been initialized. */
  bool initialized;
  /** The root node the the XML tree. */
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
  inline bool isInitialized() const throw() {return initialized;}

  /**
    Returns the root of the XML tree.
  */
  inline XMLNode* getRoot() const throw() {return root;}

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
