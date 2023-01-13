#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <cassert>
#include <string>
#include <vector>

#include "doctest/doctest.h"
#include "base64.h"

TEST_CASE("test_nums")
{
	const std::string s = "012345678910";
	const size_t enc_size = base64::calc_encoded_size(s.size());

	std::string res;
	res.resize(enc_size);
	const size_t flag = base64::encode(s, res);
	REQUIRE(res == "MDEyMzQ1Njc4OTEw");
	REQUIRE(flag == 0);
}
