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
    size_t const Nm = (N == 1) ? 5 : 1;

    D d1a(N), d1b(Nm * N), d1c(N);
#if TEST_STD_VER >= 11
    d1a.erase(d1a.cbegin(), d1a.cend());
    d1b.erase(d1b.cbegin()+1);
    d1c.erase(d1c.cend()-1);
#else
    d1a.erase(d1a.begin(), d1a.end());
    d1b.erase(d1b.begin()+1);
    d1c.erase(d1c.end()-1);
#endif

    assert(is_contiguous_container_asan_correct(d1a));
    assert(is_contiguous_container_asan_correct(d1b));
    assert(is_contiguous_container_asan_correct(d1c));

    D d2a(Nm * N+1), d2b(Nm * N+1), d2c(Nm * N+1), d2d(Nm * N+1);

    d2a.erase(d2a.begin(), d2a.end()-2);
    d2b.erase(d2b.begin()+1, d2b.end());
    d2c.erase(d2c.begin()+2);
    d2d.erase(d2d.end()-3);

    assert(is_contiguous_container_asan_correct(d2a));
    assert(is_contiguous_container_asan_correct(d2b));
    assert(is_contiguous_container_asan_correct(d2c));
    assert(is_contiguous_container_asan_correct(d2d));

    d2c.erase(d2c.begin()+2);
    d2d.erase(d2d.end()-3);
    assert(is_contiguous_container_asan_correct(d2c));
    assert(is_contiguous_container_asan_correct(d2d));

    d2c.erase(d2c.begin()+2);
    d2d.erase(d2d.end()-3);
    assert(is_contiguous_container_asan_correct(d2c));
    assert(is_contiguous_container_asan_correct(d2d));

    D d3a(32*N+1), d3b(32*N+1), d3c(32*N+1), d3d(32*N+1);

    d3a.erase(d3a.begin()+N+1, d3a.end()-N-2);
    d3b.erase(d3b.begin()+N-1, d3b.end());

    assert(is_contiguous_container_asan_correct(d3a));
    assert(is_contiguous_container_asan_correct(d3b));
    while(d3c.size() > 3)
    {
        d3c.erase(d3c.begin()+2);
        assert(is_contiguous_container_asan_correct(d3c));
    }
    while(d3d.size() > 4)
    {
        d3d.erase(d3d.end()-3);
        assert(is_contiguous_container_asan_correct(d3d));
    }

    D d4a(32*N + 3), d4b(32*N + 3);
    while(d4a.size() > 5)
    {
        d4a.erase(d4a.begin() + 2, d4a.begin() + 4);
        assert(is_contiguous_container_asan_correct(d4a));
    }

    while(d4b.size() > 5)
    {
        d4b.erase(d4b.end() - 4, d4b.end() - 2);
        assert(is_contiguous_container_asan_correct(d4b));
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
