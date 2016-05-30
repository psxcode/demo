#pragma once

#if defined DBS_DELEGATE_DON_CLUGSTON
#include "DonClugstonDelegate.h"
#elif defined DBS_DElEGATE_SERGEY_RYAZANOV
#endif


namespace Demo
{
	//------------------------------------------------------------
	class delegate_traits
	{
	public:
		//--------------------------------//
		template< typename D >
		class container
		{
		public:
			//--------------------------------//
			class traits
			{
			public:
				typedef typename shared_ptr_traits<D>::type del_t;
				typedef typename type_traits< del_t >::cref del_in;
				typedef typename std::set< del_t > del_cont;
			};
			//--------------------------------//
			virtual ~container() {}

			inline bool empty()const { return delContainer.empty(); }
			inline void add(typename traits::del_in del) { delContainer.insert(del); }
			inline void remove(typename traits::del_in del) { delContainer.erase(del); }
			inline void clear() { delContainer.clear(); }
			inline void merge(const container<D>& in) {
				for (auto iter = in.delContainer.cbegin(), end_iter = in.delContainer.cend(); iter != end_iter; ++iter)
					delContainer.insert(*iter);
			}

		protected:
			container() {}
			typename traits::del_cont delContainer;
		};
		//--------------------------------//

	public:
		//--------------------------------//
		template<typename Signature>
		class idel;
		//--------------------------------//
		template<typename R>
		class idel<R()>
		{
		public:
			virtual ~idel() {}
			virtual R operator() ()const = 0;
		};
		//--------------------------------//
		template<typename R, typename... Args>
		class idel<R(Args...)>
		{
		public:
			virtual ~idel() {}
			virtual R operator() (Args...)const = 0;
		};
		//--------------------------------//
		//--------------------------------//
		template<typename Signature>
		class composite;
		//--------------------------------//
		template<>
		class composite<void()> : public container<idel<void()>>, idel<void()>
		{
		public:
			virtual ~composite() {}
			virtual void operator() ()const {
				for (auto it = delContainer.begin(), eit = delContainer.end(); it != eit; ++it)
					(**it)();
			}
		};
		//--------------------------------//
		template<typename... Args>
		class composite<void(Args...)> : public container<idel<void(Args...)>>, idel<void(Args...)>
		{
		public:
			virtual ~composite() {}
			virtual void operator() (Args... args)const {
				for (auto it = delContainer.begin(), eit = delContainer.end(); it != eit; ++it)
					(**it)(args...);
			}
		};
		//--------------------------------//
		//--------------------------------//
		template<typename Signature>
		class del;
		//--------------------------------//
		template<typename R>
		class del<R()>
#ifdef DBS_DELEGATE_DON_CLUGSTON
			: public fastdelegate::FastDelegate<R>
#endif
			, public idel<R()>
		{
		public:
			/* Some Class may want to use fast baseType without interface and virtuals
			* so just create delegate with del<>::type line
			*/
#ifdef DBS_DELEGATE_DON_CLUGSTON
			typedef fastdelegate::FastDelegate<R> type;
#endif
			virtual ~del() {}

			del() : type() {}

			template < class X, class Y >
			del(Y * pthis, R(X::*function_to_bind)())
				: type(pthis, function_to_bind)	{}

			template < class X, class Y >
			del(const Y *pthis,	R(X::*function_to_bind)() const)
				: type(pthis, function_to_bind)	{}

			del(R(*function_to_bind)())
				: type(function_to_bind) {}

			virtual R operator() ()const { return type::operator() (); }
		};
		//--------------------------------//
		template<typename R, typename... Args>
		class del<R(Args...)>
#ifdef DBS_DELEGATE_DON_CLUGSTON
			: public fastdelegate::FastDelegate<R(Args...)>
#endif
			, public idel<R(Args...)>
		{
		public:
			/* Some Class may want to use fast baseType without interface and virtuals
			* so just create delegate with del<>::type line
			*/
#ifdef DBS_DELEGATE_DON_CLUGSTON
			typedef fastdelegate::FastDelegate<R(Args...)> type;
#endif
			virtual ~del() {}

			del() : type() {}

			template < class X, class Y >
			del(Y * pthis, R(X::*function_to_bind)(Args...))
				: type(pthis, function_to_bind)	{}

			template < class X, class Y >
			del(const Y *pthis, R(X::*function_to_bind)(Args...) const)
				: type(pthis, function_to_bind)	{}

			del(R(*function_to_bind)(Args...))
				: type(function_to_bind) {}

			virtual R operator() (Args... args)const { return type::operator() (args...); }
		};
		//--------------------------------//
		//--------------------------------//
		template<typename Signature>
		class active_del;
		//--------------------------------//
		template<typename R>
		class active_del<R()> : public del<R()>
		{
		public:
			virtual ~active_del() {}

			active_del() : del(), active(true) {}
			explicit active_del(bool active) : del(), active(active) {}

			template < class X, class Y >
			active_del(Y * pthis,
				R(X::* function_to_bind)())
				: del(pthis, function_to_bind)	{}

			template < class X, class Y >
			active_del(const Y *pthis,
				R(X::* function_to_bind)() const)
				: del(pthis, function_to_bind)	{}

			active_del(R(*function_to_bind)())
				: del(function_to_bind) {}

			virtual R operator() ()const { if (active) return type::operator() (); }

			bool active;
		};
		//--------------------------------//
		template<typename R, typename... Args>
		class active_del<R(Args...)> : public del<R(Args...)>
		{
		public:
			virtual ~active_del() {}

			active_del() : del(), active(true) {}
			explicit active_del(bool active) : del(), active(active) {}

			template < class X, class Y >
			active_del(Y * pthis,
				R(X::* function_to_bind)(Args...))
				: del(pthis, function_to_bind)	{}

			template < class X, class Y >
			active_del(const Y *pthis,
				R(X::* function_to_bind)(Args...) const)
				: del(pthis, function_to_bind)	{}

			active_del(R(*function_to_bind)(Args...))
				: del(function_to_bind) {}

			virtual R operator() (Args... args)const { if (active) return type::operator() (args...); }

			bool active;
		};
		//--------------------------------//
	};
	//------------------------------------------------------------
}