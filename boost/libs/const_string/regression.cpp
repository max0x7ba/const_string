// Copyright (c) 2004 Maxim Yegorushkin
//
// Use, modification and distribution are subject to the 
// Boost Software License, Version 1.0. (See accompanying file 
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma warning(disable: 4786)

#pragma warning(push, 1)

#define NOMINMAX

#define BOOST_AUTO_TEST_MAIN
#include "boost/test/auto_unit_test.hpp"
//#include "boost/test/included/unit_test_framework.hpp"

#pragma warning(pop)

#include <vector>
#include <set>

#include "boost/const_string/const_string.hpp"
#include "boost/const_string/concatenation.hpp"
#include "boost/const_string/format.hpp"
#include "boost/const_string/io.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////

template<class CharT>
std::basic_string<CharT> gen_str(size_t n)
{
    std::basic_string<CharT> r(n, CharT());
    while(n--)
        r[n] = 'a' + std::rand() % 25;
    return r;
}

////////////////////////////////////////////////////////////////////////////////////////////////

template<class CharT>
struct literals
{
    static CharT const empty_string[];
    static CharT const some_string[];
    static CharT const line[];

    static CharT const fmt1[];
    static CharT const fmt2[];
    static CharT const fmt3[];
};

#define LITERALS(mem, lit) \
template<> char const literals<char>::mem[] = lit; \
template<> wchar_t const literals<wchar_t>::mem[] = L##lit;

LITERALS(empty_string, "")
LITERALS(some_string, "0123456789abcdefghijklmnopqrstuvwxyz")
LITERALS(line, "abc\n")

LITERALS(fmt1, "abcd")
LITERALS(fmt2, "0x%08x0x%08x0x%08x0x%08x")

template<> char const literals<char>::fmt3[] = "%.*s%.*s";
template<> wchar_t const literals<wchar_t>::fmt3[] = L"%.*ls%.*ls";


#undef LITERALS

////////////////////////////////////////////////////////////////////////////////////////////////

template<class const_string>
void do_test_comparison()
{
    typedef typename const_string::value_type CharT;
    typedef std::basic_string<CharT> std_string;
    using boost::cref;
    using boost::lit;
    
    CharT const(&empty)[1] = literals<CharT>::empty_string;
    CharT const(&some_string)[37] = literals<CharT>::some_string;

    { // constructors compile test

    // from string literal
    const_string cs101;
    const_string cs102(empty);
    const_string cs103(empty, size_t(0));
    const_string cs104(cref(empty));
    const_string cs105(cref(empty), size_t(0));
    const_string cs106(some_string);
    const_string cs107(some_string, size_t(0));
    const_string cs108(cref(some_string));
    const_string cs109(cref(some_string), size_t(0));
    const_string cs110(lit(empty));
    const_string cs111(lit(some_string));

    // from std::basic_string<>
    std_string ss21(gen_str<CharT>(8));
    const_string cs21(ss21);
    const_string cs22(ss21, 4);
    const_string cs23(ss21, 4, 2);
    const_string cs24(cref(ss21));
    const_string cs25(cref(ss21), 4);
    const_string cs26(cref(ss21), 4, 2);

    // from CharT const*
    CharT const* const cb(some_string);
    CharT const* b(some_string);
    CharT const* e(some_string + sizeof(some_string) / sizeof(*some_string));

    const_string cs301(b);
    const_string cs302(b, 10);
    const_string cs303(cref(b));
    const_string cs304(cref(b), 10);
    const_string cs305(cref(cb));
    const_string cs306(cref(cb), 10);
    const_string cs307(b, e);
    const_string cs308(cref(b), e);
    const_string cs309(cref(cb), e);

    // from const_string
    const_string cs41(cs301);
    const_string cs42(cs301, 10);
    const_string cs43(cs301, 10, 10);
    const_string cs44(cref(cs301));
    const_string cs45(cref(cs301), 10);
    const_string cs46(cref(cs301), 10, 10);

    // other
    const_string xxx(3, 'x');
    }

    { // equality / inequality
    const_string a;
    BOOST_CHECK(a == empty);
    BOOST_CHECK(!(a != empty));
    BOOST_CHECK(!(a < empty));
    BOOST_CHECK(!(a > empty));
    BOOST_CHECK(!a.size());
    BOOST_CHECK(a.empty());
    BOOST_CHECK(a.begin() == a.end());
    BOOST_CHECK(a.rbegin() == a.rend());

    const_string b(empty);
    BOOST_CHECK(b == empty);
    BOOST_CHECK(!(b != empty));
    BOOST_CHECK(!(b < empty));
    BOOST_CHECK(!(b > empty));
    BOOST_CHECK(!b.size());
    BOOST_CHECK(b.empty());
    BOOST_CHECK(b.begin() == b.end());
    BOOST_CHECK(b.rbegin() == b.rend());

    const_string c(cref(empty));
    BOOST_CHECK(c == empty);
    BOOST_CHECK(!(c != empty));
    BOOST_CHECK(!(c < empty));
    BOOST_CHECK(!(c > empty));
    BOOST_CHECK(!c.size());
    BOOST_CHECK(c.empty());
    BOOST_CHECK(c.begin() == c.end());
    BOOST_CHECK(c.rbegin() == c.rend());

    c = empty;
    c = cref(empty);
    c = const_string(empty);
    c = cref(const_string(empty));
    c = std_string(empty);
    c = cref(std_string(empty));
    }

    { // construction
    for(size_t n(128); n; --n)
    {
        std_string const ss1(gen_str<CharT>(n));

        const_string cs1(ss1);
        BOOST_CHECK(cs1 == ss1);
        BOOST_CHECK(!(cs1 != ss1));
        BOOST_CHECK(!(cs1 < ss1));
        BOOST_CHECK(!(cs1 > ss1));
        BOOST_CHECK(ss1.size() == cs1.size());
        BOOST_CHECK(ss1.size() == (size_t)std::distance(cs1.begin(), cs1.end()));
        BOOST_CHECK(ss1.size() == (size_t)std::distance(cs1.rbegin(), cs1.rend()));

        // less and greater
        BOOST_CHECK(cs1 > cs1.ref_substr(0, cs1.size() - 1));
        BOOST_CHECK(cs1.ref_substr(0, cs1.size() - 1) < cs1);

        const_string cs2(cref(ss1));
        BOOST_CHECK(cs2 == ss1);
        BOOST_CHECK(!(cs2 != ss1));
        BOOST_CHECK(!(cs2 < ss1));
        BOOST_CHECK(!(cs2 > ss1));
        BOOST_CHECK(ss1.size() == cs2.size());
        BOOST_CHECK(ss1.size() == (size_t)std::distance(cs2.begin(), cs2.end()));
        BOOST_CHECK(ss1.size() == (size_t)std::distance(cs2.rbegin(), cs2.rend()));

        CharT const* const p1(ss1.c_str());
        const_string cs3(p1);
        BOOST_CHECK(cs3 == p1);
        BOOST_CHECK(!(cs3 != p1));
        BOOST_CHECK(!(cs3 < p1));
        BOOST_CHECK(!(cs3 > p1));
        BOOST_CHECK(std::char_traits<CharT>::length(p1) == cs3.size());
        BOOST_CHECK(std::char_traits<CharT>::length(p1) == (size_t)std::distance(cs3.begin(), cs3.end()));
        BOOST_CHECK(std::char_traits<CharT>::length(p1) == (size_t)std::distance(cs3.rbegin(), cs3.rend()));

        const_string cs4(cref(p1));
        BOOST_CHECK(cs4 == p1);
        BOOST_CHECK(!(cs4 != p1));
        BOOST_CHECK(!(cs4 < p1));
        BOOST_CHECK(!(cs4 > p1));
        BOOST_CHECK(std::char_traits<CharT>::length(p1) == cs4.size());
        BOOST_CHECK(std::char_traits<CharT>::length(p1) == (size_t)std::distance(cs4.begin(), cs4.end()));
        BOOST_CHECK(std::char_traits<CharT>::length(p1) == (size_t)std::distance(cs4.rbegin(), cs4.rend()));

        BOOST_CHECK_THROW(const_string cst01(ss1, ss1.size() + 1), std::out_of_range);
        BOOST_CHECK_NO_THROW(const_string cst02(ss1, ss1.size()));
        BOOST_CHECK_NO_THROW(const_string cst03(ss1, ss1.size() - 1, 10));
        BOOST_CHECK_NO_THROW(const_string cst04(ss1, ss1.size() - 1, 1));
        BOOST_CHECK_NO_THROW(const_string cst03(ss1, 0, ss1.size() + 10));
        BOOST_CHECK_NO_THROW(const_string cst03(ss1, 0, ss1.size()));
    }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////

template<class const_string>
void do_test_basic_usage()
{
    typedef typename const_string::value_type CharT;
    typedef std::basic_string<CharT> std_string;
    using boost::cref;

    // swap, copy construction and assignment
    enum { N = 1024, M = 32 };
    std::multiset<std_string> original;
    std::vector<const_string> pretender_copy;
    std::vector<const_string> pretender_ref;
    for(size_t n(N); n--;)
    {
        typename std::multiset<std_string>::const_iterator const i(original.insert(gen_str<CharT>(rand() % 256)));
        pretender_copy.push_back(*i);
        pretender_ref.push_back(cref(*i));
    }
    BOOST_CHECK(pretender_copy == pretender_ref);
    BOOST_CHECK(!(pretender_copy != pretender_ref));

    struct local
    {
        static void shuffle(std::vector<const_string>& v)
        {
            for(size_t n(N * M); n--;)
            {
                size_t i1(std::rand() % (N - 1));
                size_t i2(std::rand() % (N - 1));
                using std::swap;
                swap(v[i1], v[i2]);
            }
        }
    };

    local::shuffle(pretender_copy);
    local::shuffle(pretender_ref);
    BOOST_CHECK(pretender_copy != pretender_ref);
    BOOST_CHECK(!(pretender_copy == pretender_ref));

    std::sort(pretender_copy.begin(), pretender_copy.end());
    std::sort(pretender_ref.begin(), pretender_ref.end());
    BOOST_CHECK(pretender_copy == pretender_ref);
    BOOST_CHECK(!(pretender_copy != pretender_ref));

    typename std::vector<const_string>::const_iterator i1(pretender_copy.begin());
    typename std::vector<const_string>::const_iterator i2(pretender_ref.begin());
    for(typename std::multiset<std_string>::const_iterator i(original.begin()), e(original.end()); i != e; ++i, ++i1, ++i2)
    {
        BOOST_CHECK(*i == *i1);
        BOOST_CHECK(*i == *i2);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////

template<class const_string>
void do_test_concatenation()
{
    typedef typename const_string::value_type CharT;
    typedef std::basic_string<CharT> std_string;
    using boost::cref;

    // concatenation
    std_string s1(gen_str<CharT>(8));
    std_string s2(gen_str<CharT>(8));
    std_string s3(gen_str<CharT>(8));
    std_string s4(gen_str<CharT>(8));
    std_string s5(gen_str<CharT>(8));
    std_string s6(gen_str<CharT>(8));
    std_string s7(gen_str<CharT>(8));
    std_string s8(gen_str<CharT>(8));
    CharT const* const p2(s2.c_str());
    CharT const* const p4(s4.c_str());
    CharT const* const p6(s6.c_str());
    CharT const* const p8(s8.c_str());

    std_string const a1(s1 + p2 + s3 + p4 + s5 + p6 + s7 + p8);
    const_string const b1((const_string(cref(s1))) + p2 + s3 + p4 + s5 + p6 + s7 + p8);
    BOOST_CHECK(a1 == b1);

    std_string a2;
    a2 += s1;
    a2 += p2;
    a2 += s3;
    a2 += p4;
    a2 += s5;
    a2 += p6;
    a2 += s7;
    a2 += p8;
    
    const_string b2;
    b2 += s1;
    b2 += p2;
    b2 += s3;
    b2 += p4;
    b2 += s5;
    b2 += p6;
    b2 += s7;
    b2 += p8;
    
    BOOST_CHECK(a2 == b2);

    a2 += a2 + a2;
    b2 += b2 + b2;
    BOOST_CHECK(a2 == b2);
}

////////////////////////////////////////////////////////////////////////////////////////////////

template<class const_string>
void do_test_format()
{
    typedef typename const_string::value_type CharT;
    typedef std::basic_string<CharT> std_string;
    using boost::cs_format;
    typedef ::literals<CharT> literals;

    const_string cs01(cs_format(literals::fmt1));
    const_string cs02(cs_format(literals::fmt2, 0, -1, 0x55555555, 0xaaaaaaaa));
    const_string cs03(cs_format(literals::fmt3, cs01.size(), cs01.data(), cs02.size(), cs02.data()));

    BOOST_CHECK(cs03 == (cs01 + cs02).str());
}

////////////////////////////////////////////////////////////////////////////////////////////////

template<class const_string>
void do_test_io()
{
    typedef typename const_string::value_type CharT;
    typedef std::basic_string<CharT> std_string;
    using boost::cref;

    // io quick and dirty test
    typedef std::basic_stringstream<CharT> stream;
    
    stream s;

    const_string a(gen_str<CharT>(0x40));
    s << a;
    const_string b;
    s >> b;
    BOOST_CHECK(a == b);

    s.str(literals<CharT>::line);
    s.clear();
    const_string c;
    getline(s, c);
    BOOST_CHECK(const_string(cref(literals<CharT>::line), sizeof(literals<CharT>::line) / sizeof(*literals<CharT>::line) - 2) == c);
}

////////////////////////////////////////////////////////////////////////////////////////////////

template class boost::const_string<char>;
template class boost::const_string<wchar_t>;

template<class CharT>
void do_unit_test()
{
    do_test_comparison<boost::const_string<CharT> >();
    do_test_basic_usage<boost::const_string<CharT> >();
    do_test_concatenation<boost::const_string<CharT> >();
    do_test_format<boost::const_string<CharT> >();
    do_test_io<boost::const_string<CharT> >();
}

////////////////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_UNIT_TEST(constant_string_regression_char)
{
    std::srand(0);
    do_unit_test<char>();
}

#ifndef BOOST_NO_CWCHAR

BOOST_AUTO_UNIT_TEST(constant_string_regression_wchar_t)
{
    std::srand(1);
    do_unit_test<wchar_t>();
}

#endif // BOOST_NO_CWCHAR

////////////////////////////////////////////////////////////////////////////////////////////////
