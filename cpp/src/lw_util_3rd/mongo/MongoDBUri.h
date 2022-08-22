#pragma once
#include <string>
#include <vector>
#include "lw_util_3rd.h"

typedef struct _mongoc_uri_t mongoc_uri_t;

class LWUtil3rd_API MongoDBUri
{
public:
	struct LWUtil3rd_API Host {
		std::string host;
		std::uint16_t port;
	};

	static const std::string k_default_uri;

public:
	MongoDBUri();
	MongoDBUri(MongoDBUri&& r);
	~MongoDBUri();

	static bool GetUri(const std::string& uri_string, MongoDBUri& uri);

public:
	MongoDBUri& operator = (const MongoDBUri& r);

public:
	const mongoc_uri_t* GetMongocUri() const;

	std::vector<Host> GetHosts();

	const char* GetUserName() const;
	void SetUserName(const std::string& str);

	const char* GetPassword() const;
	void SetPassword(const std::string& str);

	bool Ssl() const;
	void EnableSsh(bool v);

private:
	mongoc_uri_t* m_uri;
};
