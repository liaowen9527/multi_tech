#pragma once
#include <vector>
#include <map>
#include <string>

#include "acl_cpp/lib_acl.hpp"

class AclStringHelper
{
public:
	void to_string(const acl::string& str, std::string& buf);
	std::string to_string(const acl::string& str);

	void to_string_vector(const std::vector<acl::string>& strs, std::vector<std::string>& out);
	std::vector<std::string> to_string_vector(const std::vector<acl::string>& strs);

	void to_aclstring_vector(const std::vector<std::string>& strs, std::vector<acl::string>& out);
	std::vector<acl::string> to_aclstring_vector(const std::vector<std::string>& strs);

	void to_string_map(const std::map<acl::string, acl::string>& strs, std::map<std::string, std::string>& out);
	std::map<std::string, std::string> to_string_map(const std::map<acl::string, acl::string>& strs);

	void to_aclstring_map(const std::map<std::string, std::string>& strs, std::map<acl::string, acl::string>& out);
	std::map<acl::string, acl::string> to_aclstring_map(const std::map<std::string, std::string>& strs);

	std::vector<const char*> to_vector_chars(const std::vector<std::string>& orgins);
	void to_vector_chars(const std::vector<std::string>& orgins, std::vector<const char*>& results, std::vector<size_t>& results_len);

	void to_keys_values(const std::map<std::string, std::string>& objs, std::vector<const char*>& keys, std::vector<size_t>& keys_len
		, std::vector<const char*>& values, std::vector<size_t>& values_len);

	void to_keys_values(const std::vector<std::string>& orgin_keys, const std::vector<std::string>& orgin_values
		, std::vector<const char*>& keys, std::vector<size_t>& keys_len
		, std::vector<const char*>& values, std::vector<size_t>& values_len);
};