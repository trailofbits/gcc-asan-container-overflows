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
#include <array>
#include <testsuite_asan_testing.h>

template<typename T>
void test() {
    typedef std::deque<T> D;
    size_t const __size = sizeof(T);
    size_t const N = (__size < _GLIBCXX_DEQUE_BUF_SIZE ? size_t(_GLIBCXX_DEQUE_BUF_SIZE / __size) : size_t(1));

    D single_d(1);
    single_d.pop_back();
    assert(is_contiguous_container_asan_correct(single_d));

    D d1(34*N);
    while(!d1.empty())
    {
        d1.pop_back();
        assert(is_contiguous_container_asan_correct(d1));
    }
}

int main(int, char**)
{

    {
        typedef char T;
        test<T>();
    }
    {
        typedef short T;
        test<T>();
    }
    {
        typedef int T;
        test<T>();
    }
    {
        typedef std::array<char, 3> T;
        test<T>();
    }
    {
        typedef std::array<char, 5> T;
        test<T>();
    }
    {
        typedef std::array<char, 11> T;
        test<T>();
    }
    {
        typedef std::array<char, 8000> T;
        test<T>();
    }
    {
        typedef std::array<int, 8000> T;
        test<T>();
    }
    {
        typedef std::array<std::array<char, 3>, 8000> T;
        test<T>();
    }
/*
    // Testing interface does not support situations,
    // when object memory may be poisoned (SSO case).
    {
        typedef std::string T;
        typedef std::deque<T> D;
        test<D>();
    }
*/
    return 0;
}
