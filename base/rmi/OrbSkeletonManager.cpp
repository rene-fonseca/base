/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/OrbSkeletonManager.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

OrbSkeletonManager::OrbSkeletonManager() throw(MemoryException) {
}

void OrbSkeletonManager::add(Reference<OrbSkeleton> skeleton) throw(MemoryException) {
  const OrbReference reference(Cast::getOffset(skeleton.getValue()));
  const String name = skeleton->getOrbName();
  names.add(name, skeleton);
  try {
    skeletons.add(reference, skeleton);
  } catch (...) {
    skeletons.remove(reference);
    throw;
  }
}
  
void OrbSkeletonManager::remove(Reference<OrbSkeleton> skeleton) throw(InvalidKey) {
  const OrbReference reference(Cast::getOffset(skeleton.getValue()));
  const String name = skeleton->getOrbName();
  names.remove(name);
  skeletons.remove(reference);
}

void OrbSkeletonManager::remove(const String& name) throw(InvalidKey) {
  const Reference<OrbSkeleton> skeleton = names[name];
  const OrbReference reference(Cast::getOffset(skeleton.getValue()));
  names.remove(name);
  skeletons.remove(reference);
}

OrbSkeletonManager::~OrbSkeletonManager() throw() {
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
