// Copyright (C) 2018-2021 Free Software Foundation, Inc.
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

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

// { dg-options "-fsanitize=address" }
// { dg-do run }

#include <cassert>
#include <testsuite_asan_testing.h>
#include <string>

// min_allocator is not defined after C++03.
#if __cplusplus >= 201103L
#include <memory>

template<typename T>
class min_allocator : public std::allocator<T> {
};
#endif

int main(int, char**)
{
#if __cplusplus >= 201103L
    {
        // ASan is turned off for non standard allocator.
        // It is sanitycheck.
        typedef std::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
        S s;
        assert(is_contiguous_container_asan_correct(s));

        s.reserve(10000);
        for(size_t i = 0; i < 100; ++i) {
            s.push_back('a');
            assert(is_contiguous_container_asan_correct(s));
        }
        // Memory should not be poisoned at all.
        assert(s.capacity() > s.size());
        assert(__sanitizer_verify_contiguous_container(s.data(), s.data() + s.capacity(), s.data() + s.capacity()));

        s.clear();
        assert(is_contiguous_container_asan_correct(s));
    }
#endif

    return 0;
}
