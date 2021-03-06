/**
 * @file   Unit-Tests/test-string-extra.cpp
 * @date   Started 2019-04-10
 * @author Lester J. Dowling
 */
#include "pch-unit-tests.hpp"
#define BOOST_TEST_MODULE String Extra Test Suite
#include <boost/test/unit_test.hpp>
#include "String-Extra/predicates.hpp"
#include "String-Extra/forming.hpp"
#include "String-Extra/convert-and-translate.hpp"
namespace utf = boost::unit_test;

BOOST_AUTO_TEST_SUITE(string_extra_test_suite, *utf::enabled())

/*
 * Unit tests to verify the correct operation of String-Extra.
 */

namespace a = boost::algorithm;
using std::string;

BOOST_AUTO_TEST_CASE(is_comma_1, *utf::enabled())
{
	BOOST_TEST(string_extra::is_comma(','));
	std::string all_commas{ ",,,," };
	BOOST_TEST(
	  std::all_of(all_commas.cbegin(), all_commas.cend(), &string_extra::is_comma));
	std::string any_commas{ "a,bcdef" };
	BOOST_TEST(
	  std::any_of(all_commas.cbegin(), all_commas.cend(), &string_extra::is_comma));
}

BOOST_AUTO_TEST_CASE(remove_commas_1, *utf::enabled())
{
	BOOST_TEST(string_extra::erase_commas("a,b,c") == "abc");
	BOOST_TEST(string_extra::erase_commas("abc") == "abc");
	BOOST_TEST(string_extra::erase_commas(",abc,") == "abc");
	BOOST_TEST(string_extra::erase_commas(",") == "");
	BOOST_TEST(string_extra::erase_commas(",,,,,,,,,,,") == "");
	BOOST_TEST(string_extra::erase_commas(",,,,,,,,,,,z") == "z");
	BOOST_TEST(string_extra::erase_commas("a,,,,,,,,,,,") == "a");
}

BOOST_AUTO_TEST_CASE(is_quoted_1, *utf::enabled())
{
	BOOST_TEST(string_extra::is_quoted("'abc'"));
	BOOST_TEST(string_extra::is_quoted("''"));
	BOOST_TEST(!string_extra::is_quoted("123"));
	BOOST_TEST(!string_extra::is_quoted(""));
}

BOOST_AUTO_TEST_CASE(append_quoted_1, *utf::enabled())
{
	std::string bdr;
	string_extra::append_quoted_if_not_number(bdr, "abc", "unexpected");
	BOOST_TEST(bdr == "'abc'");
	string_extra::append_quoted_if_not_number(bdr, "123", "unexpected");
	BOOST_TEST(bdr == "'abc'123");
	BOOST_REQUIRE_THROW(
	  string_extra::append_quoted_if_not_number(bdr, "", "expected"), std::runtime_error);
	string_extra::append_quoted_if_not_number(bdr, "Z", "unexpected");
	BOOST_TEST(bdr == "'abc'123'Z'");
}

BOOST_AUTO_TEST_CASE(test_convert_column_title_uppercase_letter, *utf::enabled())
{ //
	using string_extra::convert_column_title_uppercase_letter;
	const int expected_A = 1;
	BOOST_TEST(*convert_column_title_uppercase_letter("A") == expected_A);
	const int expected_Z = 26;
	BOOST_TEST(*convert_column_title_uppercase_letter("Z") == expected_Z);
	BOOST_TEST(!convert_column_title_uppercase_letter("Something else").has_value());
}

BOOST_AUTO_TEST_CASE(text_translate_column_title_uppercase_letter, *utf::enabled())
{ //
	using string_extra::translate_column_title_uppercase_letter;
	BOOST_TEST(translate_column_title_uppercase_letter("A") == string{ "1" });
	BOOST_TEST(translate_column_title_uppercase_letter("Z") == string{ "26" });
	BOOST_TEST(
	  translate_column_title_uppercase_letter("Something else") ==
	  string{ "Something else" });
}

BOOST_AUTO_TEST_SUITE_END()
