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

// This tests exists to check if strings work well with deque, as those
// may be partialy annotated, we cannot simply call
// is_contiguous_container_asan_correct with deque, as it assumes that
// object memory inside is not annotated, so we check everything in more careful way.

template<typename D>
bool verify_inside(D const &d) {
    for(size_t i = 0; i < d.size(); ++i) {
        if(!is_contiguous_container_asan_correct(d[i])) return false;
    }

    return true;
}

template<typename T, size_t N>
T get_s(char c) {
    T s;
    for(size_t i = 0; i < N; ++i)
        s.push_back(c);

    return s;
}

template<typename T>
void test() {
    typedef std::deque<T> D;
    size_t const __size = sizeof(T);
    size_t const N = (__size < _GLIBCXX_DEQUE_BUF_SIZE ? size_t(_GLIBCXX_DEQUE_BUF_SIZE / __size) : size_t(1));

    {
        D d1a(1), d1b(N), d1c(N+1), d1d(32*N);
        assert(verify_inside(d1a));
        assert(verify_inside(d1b));
        assert(verify_inside(d1c));
        assert(verify_inside(d1d));
    }
    {
        D d2;
        for(size_t i = 0; i < 16*N; ++i)
        {
            d2.push_back(get_s<T, 1>(i%10 + 'a'));
            assert(verify_inside(d2));
            d2.push_back(get_s<T, 222>(i%10 + 'b'));
            assert(verify_inside(d2));

            d2.pop_front();
            assert(verify_inside(d2));
        }
    }
    {
        D d3;
        for(size_t i = 0; i < 16*N; ++i)
        {
            d3.push_front(get_s<T, 1>(i%10 + 'a'));
            assert(verify_inside(d3));
            d3.push_front(get_s<T, 222>(i%10 + 'b'));
            assert(verify_inside(d3));

            d3.pop_back();
            assert(verify_inside(d3));
        }
    }
    {
        D d4;
        for(size_t i = 0; i < 16*N; ++i)
        {
            // When there is no SSO, all elements inside should not be poisoned,
            // se we can verify deque poisoning.
            d4.push_front(get_s<T, 333>(i%10 + 'a'));
            assert(verify_inside(d4));
            assert(is_contiguous_container_asan_correct(d4));
            d4.push_back(get_s<T, 222>(i%10 + 'b'));
            assert(verify_inside(d4));
            assert(is_contiguous_container_asan_correct(d4));
        }
    }
    {
        D d5;
        for(size_t i = 0; i < 5*N; ++i)
        {
            // In d4 we never had poisoned memory inside deque.
            // Here we start with SSO, so part of the inside of the container,
            // will be poisoned.
            d5.push_front(T());
            assert(verify_inside(d5));
       }
        for(size_t i = 0; i < d5.size(); ++i)
        {
            // We change the size to have long string.
            // Memory owne by deque should not be poisoned by string.
            d5[i].resize(1000);
            assert(verify_inside(d5));
        }

        assert(is_contiguous_container_asan_correct(d5));

        d5.erase(d5.begin() + 2);
        assert(verify_inside(d5));

        d5.erase(d5.end() - 2);
        assert(verify_inside(d5));

        assert(is_contiguous_container_asan_correct(d5));
    }
    {
        D d6a; 
        assert(is_contiguous_container_asan_correct(d6a));

        D d6b(N+2, get_s<T, 1000>('a'));
        d6b.push_front(get_s<T, 1001>('b'));
        while(!d6b.empty())
        {
            d6b.pop_back();
            assert(is_contiguous_container_asan_correct(d6b));
        }

        D d6c(N+2, get_s<T, 1002>('c'));
        while(!d6c.empty())
        {
            d6c.pop_back();
            assert(is_contiguous_container_asan_correct(d6c));
        }
    }
    {
        D d7(9*N+2);

        d7.insert(d7.begin() + 1, T());
        assert(verify_inside(d7));

        d7.insert(d7.end() - 3, T());
        assert(verify_inside(d7));

        d7.insert(d7.begin() + 2 * N, get_s<T, 1>('a'));
        assert(verify_inside(d7));

        d7.insert(d7.end() - 2 * N, get_s<T, 1>('b'));
        assert(verify_inside(d7));

        d7.insert(d7.begin() + 2 * N, 3*N, get_s<T, 1>('c'));
        assert(verify_inside(d7));

        // It may not be short for big element types, but we are ok with it:
        d7.insert(d7.end() - 2 * N, 3*N, get_s<T, 2>('d'));
        assert(verify_inside(d7));

        d7.erase(d7.begin() + 2);
        assert(verify_inside(d7));

        d7.erase(d7.end() - 2);
        assert(verify_inside(d7));
   }
}

int main(int, char**)
{
    // Those tests supports only types based on std::basic_string.

    {
        typedef std::string T;
        test<T>();
    }
    {
        typedef std::wstring T;
        test<T>();
    }
#if TEST_STD_VER >= 11
    {
        typedef std::u16string T;
        test<T>();
    }
    {
        typedef std::u32string T;
        test<T>();
    }
#endif
#if TEST_STD_VER >= 20
    {
        typedef std::u8string T;
        test<T>();
    }
#endif
    return 0;
}
