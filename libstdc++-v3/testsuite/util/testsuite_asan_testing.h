#define _GLIBCXX_SANITIZE_DEQUE 1
#define _GLIBCXX_SANITIZE_STRING 1
#define __LIBCPP_VERIFY_ASAN_DEQUE_ANNOTATIONS 1

#include <deque>
#include <string>

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

