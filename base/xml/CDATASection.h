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

#include <base/xml/Text.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class Document;

/**
  CDATA sections are used to escape blocks of text containing characters that
  would otherwise be regarded as markup. The only delimiter that is recognized
  in a CDATA section is the "]]>" string that ends the CDATA section. CDATA
  sections cannot be nested. Their primary purpose is for including material
  such as XML fragments, without needing to escape all the delimiters.
  
  The getData() method returns the text that is contained by the CDATA section.
  Note that this may contain characters that need to be escaped outside of
  CDATA sections and that, depending on the character encoding ("charset")
  chosen for serialization, it may be impossible to write out some characters
  as part of a CDATA section.
  
  The CDATASection class inherits from the CharacterData class through the Text
  class. Adjacent CDATASection nodes are not merged by use of the normalize()
  method of the Node class.
  
  @short DOM CDATA section node.
  @ingroup xml
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API CDATASection : public Text {
  friend class Document;
protected:

  inline CDATASection(void* context) noexcept : Text(context) {
  }
public:

  inline CDATASection(Node node) throw(CastException)
    : Text(node) {
    bassert(
      node.isInvalid() || (node.getType() == CDATA_SECTION_NODE),
      CastException(this)
    );
  }
  
  inline CDATASection(Text node) throw(CastException)
    : Text(node) {
    bassert(
      node.isInvalid() || (node.getType() == CDATA_SECTION_NODE),
      CastException(this)
    );
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
