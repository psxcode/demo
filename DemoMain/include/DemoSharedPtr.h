#pragma once

#ifdef DBS_SHARED_PTR_STL
#	include <memory>
#elif defined DBS_SHARED_PTR_OGRE
#	include <OgreSharedPtr.h>
#elif defined DBS_SHARED_PTR_BOOST
#	include <boost/shared_ptr.hpp>
#else
#	error
#endif

#include "DemoMemoryAllocatorConfig.h"

namespace Demo
{
namespace detail
{
	template<typename T>
	class shared_ptr_delete
	{
	public:
		void operator()(T *ptr)
		{
			DEMO_DELETE(ptr);
		}
	};
	template<typename T, MemoryCategory C>
	class shared_ptr_delete_t
	{
	public:
		void operator()(T *ptr)
		{
			DEMO_DELETE_T(ptr, T, C);
		}
	};
}//detail
	//------------------------------------------------------------
	template<typename T>
	class shared_ptr_traits
	{
	public:
		typedef typename type_traits<T>::type element_type;
		typedef typename type_traits<T>::ctype element_ctype;
		typedef typename type_traits<T>::ptr element_ptr;
		typedef typename type_traits<T>::cd_ptr element_cptr;

#ifdef DBS_SHARED_PTR_STL
		typedef ::std::shared_ptr<element_type> type;
		typedef ::std::shared_ptr<element_ctype> ctype;
#elif defined DBS_SHARED_PTR_OGRE
		typedef ::Ogre::SharedPtr<element_type> type;
		typedef ::Ogre::SharedPtr<element_ctype> ctype;
#elif defined DBS_SHARED_PTR_BOOST
		typedef ::boost::shared_ptr<element_type> type;
		typedef ::boost::shared_ptr<element_ctype> ctype;
#else
#error
#endif
	};
	//------------------------------------------------------------
	template<typename T>
	inline typename shared_ptr_traits<T>::type make_shared(T *ptr)
	{
#if defined DBS_SHARED_PTR_STL || defined DBS_SHARED_PTR_BOOST
		return shared_ptr_traits<T>::type(ptr, detail::shared_ptr_delete<T>());
#elif defined DBS_SHARED_PTR_OGRE
		return shared_ptr_traits<T>::type(ptr);
#endif
	}
	//------------------------------------------------------------
	template<MemoryCategory C, typename T>
	inline typename shared_ptr_traits<T>::type make_shared(T *ptr)
	{
#if defined DBS_SHARED_PTR_STL || defined DBS_SHARED_PTR_BOOST
		return shared_ptr_traits<T>::type(ptr, detail::shared_ptr_delete_t<T, C>());
#elif defined DBS_SHARED_PTR_OGRE
		shared_ptr_traits<T>::type sp;
		sp.bind<C>(ptr, SPFM_DELETE_T);
		return sp;
#endif
	}
	//------------------------------------------------------------
	template<typename U, typename T>
	inline typename shared_ptr_traits<U>::type shared_ptr_dynamic_cast(typename shared_ptr_traits<T>::type const &shptr)
	{
#ifdef DBS_SHARED_PTR_OGRE
		return shptr->dynamicCast<U>();
#elif defined DBS_SHARED_PTR_BOOST
		return ::boost::dynamic_pointer_cast<U>(shptr);
#elif defined DBS_SHARED_PTR_STL
		return ::std::dynamic_pointer_cast<U>(shptr);
#endif
	}
	//------------------------------------------------------------
	template<typename U, typename T>
	inline typename shared_ptr_traits<U>::type shared_ptr_static_cast(typename shared_ptr_traits<T>::type const &shptr)
	{
#ifdef DBS_SHARED_PTR_OGRE
		return shptr->staticCast<U>();
#elif defined DBS_SHARED_PTR_BOOST
		return ::boost::static_pointer_cast<U>(shptr);
#elif defined DBS_SHARED_PTR_STL
		return ::std::static_pointer_cast<U>(shptr);
#endif
	}
	//------------------------------------------------------------
}