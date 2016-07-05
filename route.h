#pragma once

#include <map>
#include <string>
#include "defines.h"

LIBHTTPROUTE_NS_BEGIN

class HttpRequest
{
public:
	const std::string& getMethod() const = 0;
	const std::string& getPath() const = 0;
	const std::string& getHost() const = 0;
	const std::string& getScheme() const = 0;

	bool hasHeader(const std::string& key) const = 0;
	const std::string& getHeader(const std::string& key, const std::string& defaultValue = std::string()) = 0;
	const std::list<std::string> getHeaders(const std::string& key) const = 0;

	bool hasParameter(const std::string& key) const = 0;
	const std::string& getParameter(const std::string& name, const std::string& defaultValue = std::string()) const = 0;
	const std::list<std::string> getParameters(const std::string& name) const = 0;
};

class Match
{
public:
	std::map<std::string, std::string> vars;
};

class Matcher
{
public:
	virtual bool match(PWebRequest& req, RouteMatch& rm) const = 0;
};

LIBHTTPROUTE_NS_END