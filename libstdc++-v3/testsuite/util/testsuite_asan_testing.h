// Components for manipulating sequences of characters -*- C++ -*-

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

/** 
 *  *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly.
 */

// That header is used only to test ASan annotations implementation.

#ifndef _TESTSUITE_ASAN_TESTING_H
#define _TESTSUITE_ASAN_TESTING_H 1

#define _GLIBCXX_SANITIZE_DEQUE 1
#define _GLIBCXX_SANITIZE_STRING 1
#define __LIBCPP_VERIFY_ASAN_DEQUE_ANNOTATIONS 1

#include <deque>
#include <string>
#include <vector>

#ifndef _GLIBCXX_DEQUE_BUF_SIZE
#define _GLIBCXX_DEQUE_BUF_SIZE 512
#endif

template <class _Tp, class _Allocator>
bool is_contiguous_container_asan_correct( const std::deque<_Tp, _Allocator> &c )
{
    return c.__verify_asan_annotations();
}

template<typename S>
bool __is_string_short(S const &s)
{
    return (void *) &s < (void *)s.data() && (void *)s.data() < (void *)(&s + 1);
}

template <typename ChrT, typename TraitsT, typename Alloc>
bool is_contiguous_container_asan_correct ( const std::basic_string<ChrT, TraitsT, Alloc> &c )
{
    if (
#if __cplusplus >= 201103L
        std::is_same<Alloc, std::allocator<ChrT> >::value &&
#endif
        c.data() != NULL)
    {
        if(__is_string_short(c))
        {
            if(sizeof(void*) == 8)
                return __sanitizer_verify_contiguous_container (
                    &c, c.data() + c.size() + 1, &c + 1) != 0;
            else
                return true;
        }
        else
            return __sanitizer_verify_contiguous_container (
                c.data(), c.data() + c.size() + 1, c.data() + c.capacity()+1) != 0;
    }
    return true;
}

template<typename S>
class ASanVerify {
    std::vector<const S*> watch_list;
public:

    void add(const S &s) {
        watch_list.push_back(&s);
    }

    bool verify() const {
        for(size_t i = 0; i < watch_list.size(); ++i) {
            if(!is_contiguous_container_asan_correct(*watch_list[i])) {
                return false;
            }
        }

        return true;
    }

    bool add_and_verify(const S &s) {
        add(s);

        return verify();
    }
};


#endif // _TESTSUITE_ASAN_TESTING_H