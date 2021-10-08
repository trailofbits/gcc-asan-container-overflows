// ASan API implementation for basic_string

// Copyright (C) 1997-2021 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file bits/basic_string.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{string}
 */


// This file may be included many times.
// It is here, as _Asan has to be implemented for both
// versions of basic_string, and implementation may be the same.

    private:

#if _GLIBCXX_SANITIZE_STD_ALLOCATOR && _GLIBCXX_SANITIZE_STRING
#if _GLIBCXX_USE_CXX11_ABI
	template<typename = _Char_alloc_type>
#else
	template<typename = _CharT_alloc_type>
#endif
	  struct _Asan
	  {
#if _GLIBCXX_USE_CXX11_ABI
	    typedef typename __gnu_cxx::__alloc_traits<_Char_alloc_type>
	      ::size_type size_type;
#else
	    typedef typename __gnu_cxx::__alloc_traits<_CharT_alloc_type>
	      ::size_type size_type;
#endif
	    static void _S_shrink(basic_string&, size_type) { }
	    static void _S_on_dealloc(basic_string&) { }

	    typedef basic_string& _Reinit;

	    struct _Grow
	    {
	      _Grow(basic_string&, size_type) { }
	      void _M_grew(size_type) { }
	    };
	  };

	// Enable ASan annotations for memory obtained from std::allocator.
	template<typename _Up>
	  struct _Asan<allocator<_Up> >
	  {
#if _GLIBCXX_USE_CXX11_ABI
	    typedef typename __gnu_cxx::__alloc_traits<_Char_alloc_type>
	      ::size_type size_type;
#else
	    typedef typename __gnu_cxx::__alloc_traits<_CharT_alloc_type>
	      ::size_type size_type;
#endif
	    // Adjust ASan annotation for [_M_start, _M_end_of_storage) to
	    // mark end of valid region as __curr instead of __prev.
	    static void
	    _S_adjust(basic_string& __str, pointer __prev, pointer __curr)
	    {
#if __cplusplus >= 202011L
              if(sizeof(__str)%SHADOW_GRANULARITY != 0 || ((uintptr_t)&__str) % SHADOW_GRANULARITY != 0)
#else
              if(sizeof(__str)%SHADOW_GRANULARITY != 0 || ((unsigned long long)&__str) % SHADOW_GRANULARITY != 0)
#endif
                return;
#if _GLIBCXX_USE_CXX11_ABI
              if(__str._M_is_local()) {
#else
              if(__str._M_rep()->_M_is_shared())
                return;
              if(false) {
#endif
                __sanitizer_annotate_contiguous_container(&__str,
                        &__str + 1, __prev + 1, __curr + 1);
             }
              else {
                __sanitizer_annotate_contiguous_container(__str._M_data(),
                        __str._M_data() + __str.capacity() + 1, __prev + 1, __curr + 1);
              }
	    }

	    static void
	    _S_grow(basic_string& __str, size_type __n)
	    { _S_adjust(__str, __str._M_data() + __str.size(), __str._M_data() + __str.size() + __n); }

	    static void
	    _S_shrink(basic_string& __str, size_type __n)
	    { _S_adjust(__str, __str._M_data() + __str.size() + __n, __str._M_data() + __str.size()); }

	    static void
	    _S_on_dealloc(basic_string& __str)
	    {
	      if (__str._M_data())
		  _S_adjust(__str, __str._M_data() + __str.size(), __str._M_data() + __str.capacity());
	    }

	    // Used on reallocation to tell ASan unused capacity is invalid.
	    struct _Reinit
	    {
	      explicit _Reinit(basic_string& __str) : _M_impl(__str)
	      {
		// Mark unused capacity as valid again before deallocating it.
		_S_on_dealloc(_M_impl);
	      }

	      ~_Reinit()
	      {
		// Mark unused capacity as invalid after reallocation.
		if (_M_impl._M_data())
		  _S_adjust(_M_impl, _M_impl._M_data() + _M_impl.capacity(),
			    _M_impl._M_data() + _M_impl.size());
	      }

	      basic_string& _M_impl;

#if __cplusplus >= 201103L
	      _Reinit(const _Reinit&) = delete;
	      _Reinit& operator=(const _Reinit&) = delete;
#endif
	    };

	    // Tell ASan when unused capacity is initialized to be valid.
	    struct _Grow
	    {
	      _Grow(basic_string& __impl, size_type __n)
	      : _M_impl(__impl), _M_n(__n)
	      { _S_grow(_M_impl, __n); }

	      ~_Grow() { if (_M_n) _S_shrink(_M_impl, _M_n); }

	      void _M_grew(size_type __n) { _M_n -= __n; }

#if __cplusplus >= 201103L
	      _Grow(const _Grow&) = delete;
	      _Grow& operator=(const _Grow&) = delete;
#endif
	    private:
	      basic_string& _M_impl;
	      size_type _M_n;
	    };
	  };

#define _GLIBCXX_ASAN_ANNOTATE_REINIT_2STRING(obj) \
  typename basic_string::template _Asan<>::_Reinit const \
	__attribute__((__unused__)) __reinit_guard_custom(obj)
#define _GLIBCXX_ASAN_ANNOTATE_REINIT_STRING \
  typename basic_string::template _Asan<>::_Reinit const \
	__attribute__((__unused__)) __reinit_guard(*this)
#define _GLIBCXX_ASAN_ANNOTATE_GROW_STRING(n) \
  typename basic_string::template _Asan<>::_Grow \
	__attribute__((__unused__)) __grow_guard(*this, (n))
#define _GLIBCXX_ASAN_ANNOTATE_GREW_STRING(n) __grow_guard._M_grew(n)
#define _GLIBCXX_ASAN_ANNOTATE_SHRINK_2STRING(obj, n) \
  basic_string::template _Asan<>::_S_shrink(obj, n)
#define _GLIBCXX_ASAN_ANNOTATE_SHRINK_STRING(n) \
  _GLIBCXX_ASAN_ANNOTATE_SHRINK_2STRING(*this, n)
#define _GLIBCXX_ASAN_ANNOTATE_BEFORE_DEALLOC_STRING \
  basic_string::template _Asan<>::_S_on_dealloc(*this)
#define _GLIBCXX_ASAN_ANNOTATE_STRING_COND(n) n

#else // ! (_GLIBCXX_SANITIZE_STD_ALLOCATOR && _GLIBCXX_SANITIZE_STRING)
#define _GLIBCXX_ASAN_ANNOTATE_REINIT_2STRING(obj)
#define _GLIBCXX_ASAN_ANNOTATE_REINIT_STRING
#define _GLIBCXX_ASAN_ANNOTATE_GROW_STRING(n)
#define _GLIBCXX_ASAN_ANNOTATE_GREW_STRING(n)
#define _GLIBCXX_ASAN_ANNOTATE_SHRINK_2STRING(obj, n)
#define _GLIBCXX_ASAN_ANNOTATE_SHRINK_STRING(n)
#define _GLIBCXX_ASAN_ANNOTATE_BEFORE_DEALLOC_STRING
#define _GLIBCXX_ASAN_ANNOTATE_STRING_COND(n)
#endif // _GLIBCXX_SANITIZE_STD_ALLOCATOR && _GLIBCXX_SANITIZE_STRING