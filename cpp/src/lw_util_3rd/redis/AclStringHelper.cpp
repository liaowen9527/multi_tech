#include "AclStringHelper.h"

void AclStringHelper::to_string(const acl::string& str, std::string& buf)
{
	buf.append(str.c_str(), str.length());
}

std::string AclStringHelper::to_string(const acl::string& str)
{
	std::string buf;
	buf.append(str.c_str(), str.length());

	return std::move(buf);
}

void AclStringHelper::to_string_vector(const std::vector<acl::string>& strs, std::vector<std::string>& out)
{
	for (const acl::string& acl_str : strs)
	{
		out.push_back(to_string(acl_str));
	}
}

std::vector<std::string> AclStringHelper::to_string_vector(const std::vector<acl::string>& strs)
{
	std::vector<std::string> out;
	to_string_vector(strs, out);

	return std::move(out);
}

void AclStringHelper::to_aclstring_vector(const std::vector<std::string>& strs, std::vector<acl::string>& out)
{
	for (const std::string& str : strs)
	{
		out.push_back(acl::string());
		out.back().append(str.c_str(), str.length());
	}
}

std::vector<acl::string> AclStringHelper::to_aclstring_vector(const std::vector<std::string>& strs)
{
	std::vector<acl::string> out;
	to_aclstring_vector(strs, out);

	return std::move(out);
}

void AclStringHelper::to_string_map(const std::map<acl::string, acl::string>& strs, std::map<std::string, std::string>& out)
{
	for (const auto& pair : strs)
	{
		out.insert(std::make_pair(to_string(pair.first), to_string(pair.second)));
	}
}

std::map<std::string, std::string> AclStringHelper::to_string_map(const std::map<acl::string, acl::string>& strs)
{
	std::map<std::string, std::string> out;
	to_string_map(strs, out);

	return std::move(out);
}

void AclStringHelper::to_aclstring_map(const std::map<std::string, std::string>& strs, std::map<acl::string, acl::string>& out)
{
	for (const auto& pair : strs)
	{
		const std::string& key = pair.first;
		const std::string& value = pair.second;
		
		out.insert(std::make_pair(acl::string(key.c_str(), key.length()), acl::string(value.c_str(), value.length())));
	}
}

std::map<acl::string, acl::string> AclStringHelper::to_aclstring_map(const std::map<std::string, std::string>& strs)
{
	std::map<acl::string, acl::string> out;
	to_aclstring_map(strs, out);

	return std::move(out);
}

std::vector<const char*> AclStringHelper::to_vector_chars(const std::vector<std::string>& orgins)
{
	std::vector<const char*> results;
	for (const std::string& str : orgins)
	{
		results.push_back(str.c_str());
	}

	return std::move(results);
}

void AclStringHelper::to_vector_chars(const std::vector<std::string>& orgins, std::vector<const char*>& results, std::vector<size_t>& results_len)
{
	for (const std::string& str : orgins)
	{
		results.push_back(str.c_str());
		results_len.push_back(str.length());
	}
}

void AclStringHelper::to_keys_values(const std::map<std::string, std::string>& objs, 
	std::vector<const char*>& keys, std::vector<size_t>& keys_len, 
	std::vector<const char*>& values, std::vector<size_t>& values_len)
{
	for (const auto& pair : objs)
	{
		const std::string& key = pair.first;
		const std::string& value = pair.second;

		keys.push_back(key.c_str());
		keys_len.push_back(key.length());

		values.push_back(value.c_str());
		values_len.push_back(value.length());
	}
}

void AclStringHelper::to_keys_values(const std::vector<std::string>& orgin_keys, const std::vector<std::string>& orgin_values, 
	std::vector<const char*>& keys, std::vector<size_t>& keys_len, 
	std::vector<const char*>& values, std::vector<size_t>& values_len)
{
	to_vector_chars(orgin_keys, keys, keys_len);
	to_vector_chars(orgin_values, values, values_len);
}

