#include "base64.h"
#include <cassert>
#include <string>
#include <vector>

size_t test_nums()
{
	const std::string s = "012345678910";
	const size_t enc_size = base64::calc_encoded_size(s.size());

	std::string res{};
	res.resize(enc_size);
	size_t N = base64::encode(s, res);
	assert(res == "MDEyMzQ1Njc4OTEw");
	return N;
}