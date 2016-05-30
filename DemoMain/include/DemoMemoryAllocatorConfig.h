#pragma once

#include "OgreMemoryAllocatorConfig.h"

namespace Demo
{
	enum MemoryCategory
	{
		MEMCATEGORY_GENERAL = ::Ogre::MEMCATEGORY_GENERAL
	};

	/// Allocate space for one primitive type, external type or non-virtual type with constructor parameters
#define DEMO_NEW_T(T, category) new (::Ogre::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T))) T
	/// Free the memory allocated with DEMO_NEW_T. Category is required to be restated to ensure the matching policy is used
#define DEMO_DELETE_T(ptr, T, category) if(ptr){(ptr)->~T(); ::Ogre::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr);}

	// new / delete for classes deriving from Ogre::AllocatedObject (alignment determined by per-class policy)
#define DEMO_NEW new 
#define DEMO_DELETE delete
}