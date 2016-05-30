#pragma once

#include "DemoBuildSettings.h"

#include <OgrePrerequisites.h>

#include <boost/algorithm/string.hpp>
#include <boost/type_traits.hpp>



namespace Demo
{
	//------------------------------------------------------------
	template<typename T>
	class type_traits
	{
	public:
		//bare type (T)
		typedef typename ::boost::remove_pointer<typename
			::boost::remove_const<typename
			::boost::remove_reference<typename
			::boost::remove_cv<T>::type>::type>::type>::type		type;
		//constant (T const)
		typedef typename ::boost::add_const<type>::type				ctype;
		//pointer to type (T *)
		typedef typename ::boost::add_pointer<type>::type			ptr;
		//constant data pointer (T const *)
		typedef typename ::boost::add_pointer<ctype>::type			cd_ptr;
		//constant address pointer (T * const)
		typedef typename ::boost::add_const<ptr>::type				cp_ptr;
		//const data and address pointer (T const * const)
		typedef typename ::boost::add_const<cd_ptr>::type			cdp_ptr;
		//reference to type (T &)
		typedef typename ::boost::add_reference<type>::type			ref;
		//constant reference(T const &)
		typedef typename ::boost::add_const<ref>::type				cref;
	};
	//------------------------------------------------------------
}
	
#include "DemoSharedPtr.h"

namespace Demo
{
	//------------------------------------------------------------
	namespace detail
	{
		enum ParamType
		{
			PARAMTYPE_VALUE,
			PARAMTYPE_REFERENCE,
			PARAMTYPE_POINTER
		};
		template<typename T, ParamType>
		class param_traits;

		template<typename T>
		class param_traits<T, PARAMTYPE_VALUE> : public type_traits<T>
		{
			typedef type in;
			typedef ref out;
			typedef ref inout;
			typedef type rtn;
		};

		template<typename T>
		class param_traits<T, PARAMTYPE_REFERENCE> : public type_traits<T>
		{
			typedef cref in;
			typedef ref out;
			typedef ref inout;
			typedef type rtn;
		};

		template<typename T>
		class param_traits<T, PARAMTYPE_POINTER> : public type_traits<T>
		{
			typedef type in;
			typedef type out;
			typedef type inout;
			typedef type rtn;
		};
	}//detail
	//------------------------------------------------------------
#ifdef DBS_STRING_STL
	class string_traits : public detail::param_traits<std::string, detail::PARAMTYPE_REFERENCE>
	{
	};
#elif defined DBS_STRING_OGRE
	class string_traits : public detail::param_traits<Ogre::String, detail::PARAMTYPE_REFERENCE>
	{
	};
#endif
}//Demo

// Containers (by-value only)
// Will  be of the form:
// typedef STLAllocator<T, DefaultAllocPolicy, Category> TAlloc;
// for use in vector<T, TAlloc>::type

/*
template <typename T, typename A = STLAllocator<T, GeneralAllocPolicy> >
struct vector
{
#if OGRE_CONTAINERS_USE_CUSTOM_MEMORY_ALLOCATOR
	typedef typename std::vector<T, A> type;
#else
	typedef typename std::vector<T> type;
#endif
};*/

#include "DemoDelegate.h"