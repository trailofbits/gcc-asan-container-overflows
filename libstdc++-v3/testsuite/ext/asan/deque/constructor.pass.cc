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

    D empty_d1;
    D short_d1a(1), short_d1b(2);
    D mid_d1a(N-1), mid_d1b(N), mid_d1c(N+1);
    D long_d1a(17*N-1), long_d1b(8*N), long_d1c(11*N + 1);

    assert(is_contiguous_container_asan_correct(empty_d1));
    assert(is_contiguous_container_asan_correct(short_d1a));
    assert(is_contiguous_container_asan_correct(short_d1b));
    assert(is_contiguous_container_asan_correct(mid_d1a));
    assert(is_contiguous_container_asan_correct(mid_d1b));
    assert(is_contiguous_container_asan_correct(mid_d1c));
    assert(is_contiguous_container_asan_correct(long_d1a));
    assert(is_contiguous_container_asan_correct(long_d1b));
    assert(is_contiguous_container_asan_correct(long_d1c));

    D empty_d2(empty_d1);
    D short_d2a(short_d1a), short_d2b(short_d1b);
    D mid_d2a(mid_d1a), mid_d2b(mid_d1b), mid_d2c(mid_d1c);
    D long_d2a(long_d1a), long_d2b(long_d1b), long_d2c(long_d1c);

    assert(is_contiguous_container_asan_correct(empty_d2));
    assert(is_contiguous_container_asan_correct(short_d2a));
    assert(is_contiguous_container_asan_correct(short_d2b));
    assert(is_contiguous_container_asan_correct(mid_d2a));
    assert(is_contiguous_container_asan_correct(mid_d2b));
    assert(is_contiguous_container_asan_correct(mid_d2c));
    assert(is_contiguous_container_asan_correct(long_d2a));
    assert(is_contiguous_container_asan_correct(long_d2b));
    assert(is_contiguous_container_asan_correct(long_d2c));    

    D empty_d3(std::move(empty_d2));
    D short_d3a(std::move(short_d2a)), short_d3b(std::move(short_d2b));
    D mid_d3a(std::move(mid_d2a)), mid_d3b(std::move(mid_d2b)), mid_d3c(std::move(mid_d2c));
    D long_d3a(std::move(long_d2a)), long_d3b(std::move(long_d2b)), long_d3c(std::move(long_d2c));

    assert(is_contiguous_container_asan_correct(empty_d3));
    assert(is_contiguous_container_asan_correct(short_d3a));
    assert(is_contiguous_container_asan_correct(short_d3b));
    assert(is_contiguous_container_asan_correct(mid_d3a));
    assert(is_contiguous_container_asan_correct(mid_d3b));
    assert(is_contiguous_container_asan_correct(mid_d3c));
    assert(is_contiguous_container_asan_correct(long_d3a));
    assert(is_contiguous_container_asan_correct(long_d3b));
    assert(is_contiguous_container_asan_correct(long_d3c));    

    assert(is_contiguous_container_asan_correct(empty_d2));
    assert(is_contiguous_container_asan_correct(short_d2a));
    assert(is_contiguous_container_asan_correct(short_d2b));
    assert(is_contiguous_container_asan_correct(mid_d2a));
    assert(is_contiguous_container_asan_correct(mid_d2b));
    assert(is_contiguous_container_asan_correct(mid_d2c));
    assert(is_contiguous_container_asan_correct(long_d2a));
    assert(is_contiguous_container_asan_correct(long_d2b));
    assert(is_contiguous_container_asan_correct(long_d2c));

#if TEST_STD_VER >= 11
    D empty_d4(empty_d1.cbegin(), empty_d1.cend());
    D short_d4a(short_d1a.cbegin(), short_d1a.cend()), short_d4b(short_d1b.cbegin(), short_d1b.cend());
    D mid_d4a(mid_d1a.cbegin(), mid_d1a.cend()), mid_d4b(mid_d1b.cbegin(), mid_d1b.cend()), mid_d4c(mid_d1c.cbegin(), mid_d1c.cend());
    D long_d4a(long_d1a.cbegin(), long_d1a.cend()), long_d4b(long_d1a.cbegin(), long_d1a.cend()), long_d4c(long_d1a.cbegin(), long_d1a.cend());
#else
    D empty_d4(empty_d1.begin(), empty_d1.end());
    D short_d4a(short_d1a.begin(), short_d1a.end()), short_d4b(short_d1b.begin(), short_d1b.end());
    D mid_d4a(mid_d1a.begin(), mid_d1a.end()), mid_d4b(mid_d1b.begin(), mid_d1b.end()), mid_d4c(mid_d1c.begin(), mid_d1c.end());
    D long_d4a(long_d1a.begin(), long_d1a.end()), long_d4b(long_d1a.begin(), long_d1a.end()), long_d4c(long_d1a.begin(), long_d1a.end());
#endif

    assert(is_contiguous_container_asan_correct(empty_d4));
    assert(is_contiguous_container_asan_correct(short_d4a));
    assert(is_contiguous_container_asan_correct(short_d4b));
    assert(is_contiguous_container_asan_correct(mid_d4a));
    assert(is_contiguous_container_asan_correct(mid_d4b));
    assert(is_contiguous_container_asan_correct(mid_d4c));
    assert(is_contiguous_container_asan_correct(long_d4a));
    assert(is_contiguous_container_asan_correct(long_d4b));
    assert(is_contiguous_container_asan_correct(long_d4c));    

    assert(is_contiguous_container_asan_correct(empty_d1));
    assert(is_contiguous_container_asan_correct(short_d1a));
    assert(is_contiguous_container_asan_correct(short_d1b));
    assert(is_contiguous_container_asan_correct(mid_d1a));
    assert(is_contiguous_container_asan_correct(mid_d1b));
    assert(is_contiguous_container_asan_correct(mid_d1c));
    assert(is_contiguous_container_asan_correct(long_d1a));
    assert(is_contiguous_container_asan_correct(long_d1b));
    assert(is_contiguous_container_asan_correct(long_d1c));

#if TEST_STD_VER >= 11
    D empty_d5(empty_d1.crbegin(), empty_d1.crend());
    D short_d5a(short_d1a.crbegin(), short_d1a.crend()), short_d5b(short_d1b.crbegin(), short_d1b.crend());
    D mid_d5a(mid_d1a.crbegin(), mid_d1a.crend()), mid_d5b(mid_d1b.crbegin(), mid_d1b.crend()), mid_d5c(mid_d1c.crbegin(), mid_d1c.crend());
    D long_d5a(long_d1a.crbegin(), long_d1a.crend()), long_d5b(long_d1a.crbegin(), long_d1a.crend()), long_d5c(long_d1a.crbegin(), long_d1a.crend());
#else
    D empty_d5(empty_d1.rbegin(), empty_d1.rend());
    D short_d5a(short_d1a.rbegin(), short_d1a.rend()), short_d5b(short_d1b.rbegin(), short_d1b.rend());
    D mid_d5a(mid_d1a.rbegin(), mid_d1a.rend()), mid_d5b(mid_d1b.rbegin(), mid_d1b.rend()), mid_d5c(mid_d1c.rbegin(), mid_d1c.rend());
    D long_d5a(long_d1a.rbegin(), long_d1a.rend()), long_d5b(long_d1a.rbegin(), long_d1a.rend()), long_d5c(long_d1a.rbegin(), long_d1a.rend());
#endif

    assert(is_contiguous_container_asan_correct(empty_d5));
    assert(is_contiguous_container_asan_correct(short_d5a));
    assert(is_contiguous_container_asan_correct(short_d5b));
    assert(is_contiguous_container_asan_correct(mid_d5a));
    assert(is_contiguous_container_asan_correct(mid_d5b));
    assert(is_contiguous_container_asan_correct(mid_d5c));
    assert(is_contiguous_container_asan_correct(long_d5a));
    assert(is_contiguous_container_asan_correct(long_d5b));
    assert(is_contiguous_container_asan_correct(long_d5c));    

    assert(is_contiguous_container_asan_correct(empty_d1));
    assert(is_contiguous_container_asan_correct(short_d1a));
    assert(is_contiguous_container_asan_correct(short_d1b));
    assert(is_contiguous_container_asan_correct(mid_d1a));
    assert(is_contiguous_container_asan_correct(mid_d1b));
    assert(is_contiguous_container_asan_correct(mid_d1c));
    assert(is_contiguous_container_asan_correct(long_d1a));
    assert(is_contiguous_container_asan_correct(long_d1b));
    assert(is_contiguous_container_asan_correct(long_d1c));

    D empty_d6(0, T());
    D short_d6a(1, T()), short_d6b(2, T());
    D mid_d6a(N-1, T()), mid_d6b(N, T()), mid_d6c(N+1, T());
    D long_d6a(17*N-1, T()), long_d6b(8*N, T()), long_d6c(11*N + 1, T());

    assert(is_contiguous_container_asan_correct(empty_d6));
    assert(is_contiguous_container_asan_correct(short_d6a));
    assert(is_contiguous_container_asan_correct(short_d6b));
    assert(is_contiguous_container_asan_correct(mid_d6a));
    assert(is_contiguous_container_asan_correct(mid_d6b));
    assert(is_contiguous_container_asan_correct(mid_d6c));
    assert(is_contiguous_container_asan_correct(long_d6a));
    assert(is_contiguous_container_asan_correct(long_d6b));
    assert(is_contiguous_container_asan_correct(long_d6c));
 
#if TEST_STD_VER >= 11
    D empty_d7({});
    D short_d7a({T()}), short_d7b({T(), T()});
    D mid_d7a({T(), T(), T(), T(), T() ,T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T()});
    D mid_d7b({T(), T(), T(), T(), T() ,T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T()});

    assert(is_contiguous_container_asan_correct(empty_d7));
    assert(is_contiguous_container_asan_correct(short_d7a));
    assert(is_contiguous_container_asan_correct(short_d7b));
    assert(mid_d7a.size() == 16);
    assert(is_contiguous_container_asan_correct(mid_d7a));
    assert(mid_d7b.size() == 17);
    assert(is_contiguous_container_asan_correct(mid_d7b));
#endif
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
