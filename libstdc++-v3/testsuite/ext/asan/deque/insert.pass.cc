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
void test_insert() {
    typedef std::deque<T> D;
    size_t const __size = sizeof(T);
    size_t const N = (__size < _GLIBCXX_DEQUE_BUF_SIZE ? size_t(_GLIBCXX_DEQUE_BUF_SIZE / __size) : size_t(1));

    T t = T();
    D d1a(2), d1b(2), d1c(2);

#if TEST_STD_VER >= 11
    d1a.insert(d1a.cbegin(), t);
    d1b.insert(d1b.cend(), t);
    d1c.insert(d1c.cbegin() + 1, t);
#else
    d1a.insert(d1a.begin(), t);
    d1b.insert(d1b.end(), t);
    d1c.insert(d1c.begin() + 1, t);
#endif

    assert(is_contiguous_container_asan_correct(d1a));
    assert(is_contiguous_container_asan_correct(d1b));
    assert(is_contiguous_container_asan_correct(d1c));

    D d2a(3*N), d2b(3*N), d2c(3*N), d2d(3*N);

    d2a.insert(d2a.begin(), t);
    d2b.insert(d2b.end(), t);
    d2c.insert(d2c.begin() + 1, t);
    d2d.insert(d2d.end() - 1, t);

    assert(is_contiguous_container_asan_correct(d2a));
    assert(is_contiguous_container_asan_correct(d2b));
    assert(is_contiguous_container_asan_correct(d2c));
    assert(is_contiguous_container_asan_correct(d2d));

    d2a.insert(d2a.begin(), 2*N+1, t);
    d2b.insert(d2b.end(), 2*N+1, t);
    d2c.insert(d2c.begin() + 1, 2*N + 1, t);
    d2d.insert(d2d.end() - 1, 2*N + 1, t);

    assert(is_contiguous_container_asan_correct(d2a));
    assert(is_contiguous_container_asan_correct(d2b));
    assert(is_contiguous_container_asan_correct(d2c));
    assert(is_contiguous_container_asan_correct(d2d));

    d2c.insert(d2c.begin() + N + 1, 2*N + 1, t);
    d2d.insert(d2d.end() - N - 1, 2*N + 1, t);

    assert(is_contiguous_container_asan_correct(d2c));
    assert(is_contiguous_container_asan_correct(d2d));

    D source(3*N);

    d2a.insert(d2a.begin(), source.begin() + 1, source.end() - 1);
    d2b.insert(d2b.end(), source.begin() + 1, source.end() - 1);
    d2c.insert(d2c.begin() + 1, source.begin() + 1, source.end() - 1);
    d2d.insert(d2d.end() - 1, source.begin() + 1, source.end() - 1);

    assert(is_contiguous_container_asan_correct(d2a));
    assert(is_contiguous_container_asan_correct(d2b));
    assert(is_contiguous_container_asan_correct(d2c));
    assert(is_contiguous_container_asan_correct(d2d));

    d2c.insert(d2c.begin() + N + 1, source.begin() + 1, source.end() - 1);
    d2d.insert(d2d.end() - N - 1, source.begin() + 1, source.end() - 1);

    assert(is_contiguous_container_asan_correct(d2c));
    assert(is_contiguous_container_asan_correct(d2d));

  

#if TEST_STD_VER >= 11
    d2a.insert(d2a.cbegin(), T());
    d2b.insert(d2b.cend(), T());
    d2c.insert(d2c.cbegin() + 1, T());
    d2d.insert(d2d.cend() - 1, T());

    assert(is_contiguous_container_asan_correct(d2a));
    assert(is_contiguous_container_asan_correct(d2b));
    assert(is_contiguous_container_asan_correct(d2c));
    assert(is_contiguous_container_asan_correct(d2d));

    T a = T(), b = T(), c = T(), d = T();
    d2a.insert(d2a.cbegin(), std::move(a));
    d2b.insert(d2b.cend(), std::move(b));
    d2c.insert(d2c.cbegin() + 1, std::move(c));
    d2d.insert(d2d.cend() - 1, std::move(d));

    assert(is_contiguous_container_asan_correct(d2a));
    assert(is_contiguous_container_asan_correct(d2b));
    assert(is_contiguous_container_asan_correct(d2c));
    assert(is_contiguous_container_asan_correct(d2d));

    D d3a(2), d3b(2), d3c(2);

    d3a.insert(d3a.cbegin(), {t});
    d3b.insert(d3b.cend(), {t});
    d3c.insert(d3c.cbegin() + 1, {t});

    assert(is_contiguous_container_asan_correct(d3a));
    assert(is_contiguous_container_asan_correct(d3b));
    assert(is_contiguous_container_asan_correct(d3c));

    d3a.insert(d3a.cbegin(), {t, t, t, t, t, T(), t, t, t, t, T(), t, t, t, t, t, t});
    d3b.insert(d3b.cend(), {t, t, t, t, t, T(), t, t, t, t, T(), t, t, t, t, t, t});
    d3c.insert(d3c.cbegin() + 1, {t, t, t, t, t, T(), t, t, t, t, T(), t, t, t, t, t, t});

    assert(d3a.size() > 16);
    assert(d3b.size() > 16);
    assert(d3c.size() > 16);

    assert(is_contiguous_container_asan_correct(d3a));
    assert(is_contiguous_container_asan_correct(d3b));
    assert(is_contiguous_container_asan_correct(d3c));
 
    d3a.insert(d3a.cbegin() + 1, {t, t, t, t, t, T(), t, t, t, t, T(), t, t, t, t, t, t});
    d3b.insert(d3b.cend() - 1, {t, t, t, t, t, T(), t, t, t, t, T(), t, t, t, t, t, t});
    d3c.resize(N-1);
    d3c.insert(d3c.cbegin() + N - 2, {t, t, t, t, t, t, t, T(), t, t, t, t, T(), t, t, t, t, t, t});

    assert(is_contiguous_container_asan_correct(d3a));
    assert(is_contiguous_container_asan_correct(d3b));
    assert(is_contiguous_container_asan_correct(d3c));
#endif
}

#if TEST_STD_VER >= 11
template<typename T>
void test_emplace() {
    typedef std::deque<T> D;
    size_t const __size = sizeof(T);
    size_t const N = (__size < _GLIBCXX_DEQUE_BUF_SIZE ? size_t(_GLIBCXX_DEQUE_BUF_SIZE / __size) : size_t(1));
 
    T t = T();
    D d1a(2), d1b(2), d1c(2);

    d1a.emplace(d1a.cbegin(), t);
    d1b.emplace(d1b.cend(), t);
    d1c.emplace(d1c.cbegin() + 1, t);

    assert(is_contiguous_container_asan_correct(d1a));
    assert(is_contiguous_container_asan_correct(d1b));
    assert(is_contiguous_container_asan_correct(d1c));

    D d2a(3*N), d2b(3*N), d2c(3*N), d2d(3*N);

    d2a.emplace(d2a.cbegin(), t);
    d2b.emplace(d2b.cend(), t);
    d2c.emplace(d2c.cbegin() + 1, t);
    d2d.emplace(d2d.cend() - 1, t);

    assert(is_contiguous_container_asan_correct(d2a));
    assert(is_contiguous_container_asan_correct(d2b));
    assert(is_contiguous_container_asan_correct(d2c));
    assert(is_contiguous_container_asan_correct(d2d));

    assert(is_contiguous_container_asan_correct(d2c));
    assert(is_contiguous_container_asan_correct(d2d));

    d2a.emplace(d2a.cbegin(), T());
    d2b.emplace(d2b.cend(), T());
    d2c.emplace(d2c.cbegin() + 1, T());
    d2d.emplace(d2d.cend() - 1, T());

    assert(is_contiguous_container_asan_correct(d2a));
    assert(is_contiguous_container_asan_correct(d2b));
    assert(is_contiguous_container_asan_correct(d2c));
    assert(is_contiguous_container_asan_correct(d2d));

    T a = T(), b = T(), c = T(), d = T();
    d2a.emplace(d2a.cbegin(), std::move(a));
    d2b.emplace(d2b.cend(), std::move(b));
    d2c.emplace(d2c.cbegin() + 1, std::move(c));
    d2d.emplace(d2d.cend() - 1, std::move(d));

    assert(is_contiguous_container_asan_correct(d2a));
    assert(is_contiguous_container_asan_correct(d2b));
    assert(is_contiguous_container_asan_correct(d2c));
    assert(is_contiguous_container_asan_correct(d2d));
}
#endif

template<typename T>
void test() {
    test_insert<T>();
#if TEST_STD_VER >= 11
    test_emplace<T>();
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
