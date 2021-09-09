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
void test_assign() {
    typedef std::deque<T> D;
    size_t const __size = sizeof(T);
    size_t const N = (__size < _GLIBCXX_DEQUE_BUF_SIZE ? size_t(_GLIBCXX_DEQUE_BUF_SIZE / __size) : size_t(1));

    // void assign( size_type count, const T& value );

    D empty_d1;
    D short_d1a(1), short_d1b(N-1);
    D mid_d1(N+1);
    D long_d1(32*N+1);

    empty_d1.assign(2, T());
    short_d1a.assign(2, T());
    short_d1b.assign(2, T());
    mid_d1.assign(2, T());
    long_d1.assign(2, T());

    assert(is_contiguous_container_asan_correct(empty_d1));
    assert(is_contiguous_container_asan_correct(short_d1a));
    assert(is_contiguous_container_asan_correct(short_d1b));
    assert(is_contiguous_container_asan_correct(mid_d1));
    assert(is_contiguous_container_asan_correct(long_d1));

    empty_d1.assign(2*N, T());
    short_d1a.assign(2*N, T());
    short_d1b.assign(2*N, T());
    mid_d1.assign(2*N, T());
    long_d1.assign(2*N, T());

    assert(is_contiguous_container_asan_correct(empty_d1));
    assert(is_contiguous_container_asan_correct(short_d1a));
    assert(is_contiguous_container_asan_correct(short_d1b));
    assert(is_contiguous_container_asan_correct(mid_d1));
    assert(is_contiguous_container_asan_correct(long_d1));

    D empty_d2;
    D short_d2a(1), short_d2b(N-1);
    D mid_d2(N+1);
    D long_d2(32*N+1);

    empty_d2.assign(42*N, T());
    short_d2a.assign(42*N, T());
    short_d2b.assign(42*N, T());
    mid_d2.assign(42*N, T());
    long_d2.assign(42*N, T());

    assert(is_contiguous_container_asan_correct(empty_d2));
    assert(is_contiguous_container_asan_correct(short_d2a));
    assert(is_contiguous_container_asan_correct(short_d2b));
    assert(is_contiguous_container_asan_correct(mid_d2));
    assert(is_contiguous_container_asan_correct(long_d2));

    // template< class InputIt >
    // void assign( InputIt first, InputIt last );
    D source_d(50*N);

    D empty_d3;
    D short_d3a(1), short_d3b(N-1);
    D mid_d3(N+1);
    D long_d3(32*N+1);

    empty_d3.assign(source_d.begin() + N - 1, source_d.begin() + N + 1);
    short_d3a.assign(source_d.begin() + N - 1, source_d.begin() + N + 1);
    short_d3b.assign(source_d.begin() + N - 1, source_d.begin() + N + 1);
    mid_d3.assign(source_d.begin() + N - 1, source_d.begin() + N + 1);
    long_d3.assign(source_d.begin() + N - 1, source_d.begin() + N + 1);

    assert(is_contiguous_container_asan_correct(empty_d3));
    assert(is_contiguous_container_asan_correct(short_d3a));
    assert(is_contiguous_container_asan_correct(short_d3b));
    assert(is_contiguous_container_asan_correct(mid_d3));
    assert(is_contiguous_container_asan_correct(long_d3));
    assert(is_contiguous_container_asan_correct(source_d));

    empty_d3.assign(source_d.begin() + N, source_d.begin() + 3*N);
    short_d3a.assign(source_d.begin() + N, source_d.begin() + 3*N);
    short_d3b.assign(source_d.begin() + N, source_d.begin() + 3*N);
    mid_d3.assign(source_d.begin() + N, source_d.begin() + 3*N);
    long_d3.assign(source_d.begin() + N, source_d.begin() + 3*N);

    assert(is_contiguous_container_asan_correct(empty_d3));
    assert(is_contiguous_container_asan_correct(short_d3a));
    assert(is_contiguous_container_asan_correct(short_d3b));
    assert(is_contiguous_container_asan_correct(mid_d3));
    assert(is_contiguous_container_asan_correct(long_d3));
    assert(is_contiguous_container_asan_correct(source_d));

    D empty_d4;
    D short_d4a(1), short_d4b(N-1);
    D mid_d4(N+1);
    D long_d4(32*N+1);

    empty_d4.assign(source_d.end() - 42*N - 1, source_d.end() - 1);
    short_d4a.assign(source_d.end() - 42*N - 1, source_d.end() - 1);
    short_d4b.assign(source_d.end() - 42*N - 1, source_d.end() - 1);
    mid_d4.assign(source_d.end() - 42*N - 1, source_d.end() - 1);
    long_d4.assign(source_d.end() - 42*N - 1, source_d.end() - 1);

    assert(is_contiguous_container_asan_correct(empty_d4));
    assert(is_contiguous_container_asan_correct(short_d4a));
    assert(is_contiguous_container_asan_correct(short_d4b));
    assert(is_contiguous_container_asan_correct(mid_d4));
    assert(is_contiguous_container_asan_correct(long_d4));
    assert(is_contiguous_container_asan_correct(source_d));

#if TEST_STD_VER >= 11
    // void assign( std::initializer_list<T> ilist );
    D empty_d5;
    D short_d5a(1), short_d5b(N-1);
    D mid_d5(N+1);
    D long_d5(32*N+1);

    empty_d5.assign({T(), T()});
    short_d5a.assign({T(), T()});
    short_d5b.assign({T(), T()});
    mid_d5.assign({T(), T()});
    long_d5.assign({T(), T()});

    assert(is_contiguous_container_asan_correct(empty_d5));
    assert(is_contiguous_container_asan_correct(short_d5a));
    assert(is_contiguous_container_asan_correct(short_d5b));
    assert(is_contiguous_container_asan_correct(mid_d5));
    assert(is_contiguous_container_asan_correct(long_d5));

    empty_d5.assign({T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T()});
    short_d5a.assign({T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T()});
    short_d5b.assign({T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T()});
    mid_d5.assign({T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T()});
    long_d5.assign({T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T()});

    assert(empty_d5.size() > 16);
    assert(short_d5a.size() > 16);
    assert(short_d5b.size() > 16);
    assert(mid_d5.size() > 16);
    assert(long_d5.size() > 16);

    assert(is_contiguous_container_asan_correct(empty_d5));
    assert(is_contiguous_container_asan_correct(short_d5a));
    assert(is_contiguous_container_asan_correct(short_d5b));
    assert(is_contiguous_container_asan_correct(mid_d5));
    assert(is_contiguous_container_asan_correct(long_d5));

    D empty_d6;
    D short_d6a(1), short_d6b(N-1);
    D mid_d6(N+1);
    D long_d6(32*N+1);

    empty_d6.assign({T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T()});
    short_d6a.assign({T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T()});
    short_d6b.assign({T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T()});
    mid_d6.assign({T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T()});
    long_d6.assign({T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T(), T()});

    assert(empty_d6.size() > 16);
    assert(short_d6a.size() > 16);
    assert(short_d6b.size() > 16);
    assert(mid_d6.size() > 16);
    assert(long_d6.size() > 16);

    assert(is_contiguous_container_asan_correct(empty_d6));
    assert(is_contiguous_container_asan_correct(short_d6a));
    assert(is_contiguous_container_asan_correct(short_d6b));
    assert(is_contiguous_container_asan_correct(mid_d6));
    assert(is_contiguous_container_asan_correct(long_d6));
#endif
}

template<typename T>
void test_operator() {
    typedef std::deque<T> D;
    size_t const N = sizeof(T) < 256 ? (4096 / sizeof(T)) : 16;

    D empty_d1, empty_d2;
    D mid_d1(N), mid_d2(N);
    D long_d1(32*N), long_d2(32*N);

    empty_d1 = empty_d2;
    assert(is_contiguous_container_asan_correct(empty_d1));
    assert(is_contiguous_container_asan_correct(empty_d2));
    empty_d1 = mid_d1;
    assert(is_contiguous_container_asan_correct(empty_d1));
    assert(is_contiguous_container_asan_correct(mid_d1));

    empty_d1 = empty_d2;
    (void) empty_d1[0];
    empty_d1.push_back(T()); 
    empty_d1.pop_back();
    empty_d1 = empty_d2;
    assert(is_contiguous_container_asan_correct(empty_d1));
    assert(is_contiguous_container_asan_correct(empty_d2));
    empty_d1 = long_d1;
    assert(is_contiguous_container_asan_correct(empty_d1));
    assert(is_contiguous_container_asan_correct(long_d1));
    empty_d1 = empty_d2;
    assert(is_contiguous_container_asan_correct(empty_d1));
    assert(is_contiguous_container_asan_correct(empty_d2));

    empty_d2 = long_d1;
    assert(is_contiguous_container_asan_correct(empty_d2));

    mid_d1 = long_d1;
    assert(is_contiguous_container_asan_correct(mid_d1));
    mid_d1 = empty_d2;
    assert(is_contiguous_container_asan_correct(mid_d1));
    mid_d1 = long_d1;
    assert(is_contiguous_container_asan_correct(mid_d1));
    mid_d1 = mid_d2;
    assert(is_contiguous_container_asan_correct(mid_d1));


    long_d1 = long_d2;
    assert(is_contiguous_container_asan_correct(long_d1));
    long_d1 = empty_d1;
    assert(is_contiguous_container_asan_correct(long_d1));
    long_d1 = long_d2;
    assert(is_contiguous_container_asan_correct(long_d1));

    empty_d1 = D();
    assert(is_contiguous_container_asan_correct(empty_d1));
    mid_d1 = D();
    assert(is_contiguous_container_asan_correct(mid_d1));
    long_d1 = D();
    assert(is_contiguous_container_asan_correct(long_d1));

    mid_d1 = mid_d2;
    assert(is_contiguous_container_asan_correct(mid_d1));
    long_d1 = long_d2;
    assert(is_contiguous_container_asan_correct(long_d1));

#if TEST_STD_VER >= 11
    mid_d1 = std::move(mid_d2);
    assert(is_contiguous_container_asan_correct(mid_d1));
    assert(is_contiguous_container_asan_correct(mid_d2));
    long_d1 = std::move(long_d2);
    assert(is_contiguous_container_asan_correct(long_d1));
    assert(is_contiguous_container_asan_correct(long_d2));

    D mid_d3a(N), mid_d3b(N);
    D long_d3a(32*N), long_d3b(32*N);

    mid_d3a = std::move(long_d3a);
    long_d3b = std::move(mid_d3b);
    assert(is_contiguous_container_asan_correct(mid_d3a));
    assert(is_contiguous_container_asan_correct(mid_d3b));
    assert(is_contiguous_container_asan_correct(long_d3a));
    assert(is_contiguous_container_asan_correct(long_d3b));

    D empty_d4;
    D long_d4a(32*N), long_d4b(32*N);

    long_d4a = std::move(empty_d4);
    long_d4b = std::move(empty_d1);
    assert(is_contiguous_container_asan_correct(empty_d1));
    assert(is_contiguous_container_asan_correct(empty_d4));
    assert(is_contiguous_container_asan_correct(long_d4a));
    assert(is_contiguous_container_asan_correct(long_d4b));
#endif
}

template<typename T>
void test() {
    test_assign<T>();
    test_operator<T>();
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
