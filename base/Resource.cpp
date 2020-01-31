/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Resource.h>
#include <base/ResourceHandle.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

String Resource::getDescription() const
{
  Reference<ResourceHandle> handle = this->handle.castChecked<ResourceHandle>();
  return handle->getDescription();
}

void Resource::setDescription(const String& description)
{
  Reference<ResourceHandle> handle = this->handle.castChecked<ResourceHandle>();
  handle->setDescription(description);
}

unsigned int Resource::getResourceId() const
{
  Reference<ResourceHandle> handle = this->handle.castChecked<ResourceHandle>();
  return handle->getResourceId();
}

unsigned int Resource::getCreatedById() const
{
  Reference<ResourceHandle> handle = this->handle.castChecked<ResourceHandle>();
  return handle->getCreatedById();
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Resource& resource)
{
  if (resource) {
    auto description = resource.getDescription();
    stream << "Resource: {" << EOL
      << indent(2) << "ID: " << resource.getResourceId() << EOL
      << indent(2) << "Description: " << (description ? description : "<NONE>") << EOL
      << indent(2) << "Created by: " << resource.getCreatedById() << EOL
      << '}';
  } else {
    stream << "Resource: {" << "<NULL>" << '}';
  }
  return stream;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
