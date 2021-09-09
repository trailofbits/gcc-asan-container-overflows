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

    D empty_d1a, empty_d1b;
    D short_d1a(1), short_d1b;
    D mid_d1a(N), mid_d1b(N+1);
    D long_d1a(8*N), long_d1b(11*N + 1);

    std::swap(empty_d1a, empty_d1b);
    assert(is_contiguous_container_asan_correct(empty_d1a));
    assert(is_contiguous_container_asan_correct(empty_d1b));
    std::swap(empty_d1a, empty_d1b);
    assert(is_contiguous_container_asan_correct(empty_d1a));
    assert(is_contiguous_container_asan_correct(empty_d1b));

    std::swap(empty_d1a, short_d1a);
    assert(is_contiguous_container_asan_correct(empty_d1a));
    assert(is_contiguous_container_asan_correct(short_d1a));
    std::swap(empty_d1a, short_d1a);
    assert(is_contiguous_container_asan_correct(empty_d1a));
    assert(is_contiguous_container_asan_correct(short_d1a));

    std::swap(empty_d1a, mid_d1a);
    assert(is_contiguous_container_asan_correct(empty_d1a));
    assert(is_contiguous_container_asan_correct(mid_d1a));
    std::swap(empty_d1a, mid_d1a);
    assert(is_contiguous_container_asan_correct(empty_d1a));
    assert(is_contiguous_container_asan_correct(mid_d1a));

    std::swap(empty_d1a, mid_d1b);
    assert(is_contiguous_container_asan_correct(empty_d1a));
    assert(is_contiguous_container_asan_correct(mid_d1b));
    std::swap(empty_d1a, mid_d1b);
    assert(is_contiguous_container_asan_correct(empty_d1a));
    assert(is_contiguous_container_asan_correct(mid_d1b));

    std::swap(empty_d1a, long_d1a);
    assert(is_contiguous_container_asan_correct(empty_d1a));
    assert(is_contiguous_container_asan_correct(long_d1a));
    std::swap(empty_d1a, long_d1a);
    assert(is_contiguous_container_asan_correct(empty_d1a));
    assert(is_contiguous_container_asan_correct(long_d1a));

    std::swap(empty_d1a, long_d1b);
    assert(is_contiguous_container_asan_correct(empty_d1a));
    assert(is_contiguous_container_asan_correct(long_d1b));
    std::swap(empty_d1a, long_d1b);
    assert(is_contiguous_container_asan_correct(empty_d1a));
    assert(is_contiguous_container_asan_correct(long_d1b));

    std::swap(short_d1a, short_d1b);
    assert(is_contiguous_container_asan_correct(short_d1a));
    assert(is_contiguous_container_asan_correct(short_d1b));
    std::swap(short_d1a, short_d1b);
    assert(is_contiguous_container_asan_correct(short_d1a));
    assert(is_contiguous_container_asan_correct(short_d1b));

    std::swap(short_d1a, mid_d1b);
    assert(is_contiguous_container_asan_correct(short_d1a));
    assert(is_contiguous_container_asan_correct(mid_d1b));
    std::swap(short_d1a, mid_d1b);
    assert(is_contiguous_container_asan_correct(short_d1a));
    assert(is_contiguous_container_asan_correct(mid_d1b));

    std::swap(short_d1a, long_d1b);
    assert(is_contiguous_container_asan_correct(short_d1a));
    assert(is_contiguous_container_asan_correct(long_d1b));
    std::swap(short_d1a, long_d1b);
    assert(is_contiguous_container_asan_correct(short_d1a));
    assert(is_contiguous_container_asan_correct(long_d1b));

    std::swap(mid_d1a, mid_d1b);
    assert(is_contiguous_container_asan_correct(mid_d1a));
    assert(is_contiguous_container_asan_correct(mid_d1b));
    std::swap(mid_d1a, mid_d1b);
    assert(is_contiguous_container_asan_correct(mid_d1a));
    assert(is_contiguous_container_asan_correct(mid_d1b));

    std::swap(mid_d1a, long_d1b);
    assert(is_contiguous_container_asan_correct(mid_d1a));
    assert(is_contiguous_container_asan_correct(long_d1b));
    std::swap(mid_d1a, long_d1b);
    assert(is_contiguous_container_asan_correct(mid_d1a));
    assert(is_contiguous_container_asan_correct(long_d1b));

    std::swap(long_d1a, long_d1b);
    assert(is_contiguous_container_asan_correct(long_d1a));
    assert(is_contiguous_container_asan_correct(long_d1b));
    std::swap(long_d1a, long_d1b);
    assert(is_contiguous_container_asan_correct(long_d1a));
    assert(is_contiguous_container_asan_correct(long_d1b));

    std::swap(empty_d1a, empty_d1a);
    assert(is_contiguous_container_asan_correct(empty_d1a));

    std::swap(short_d1a, short_d1a);
    assert(is_contiguous_container_asan_correct(short_d1a));
    std::swap(short_d1b, short_d1b);
    assert(is_contiguous_container_asan_correct(short_d1b));

    std::swap(long_d1a, long_d1a);
    assert(is_contiguous_container_asan_correct(long_d1a));
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
