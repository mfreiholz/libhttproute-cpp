#pragma once

#include <string>
#include <vector>
#include "defines.h"

LIBHTTPROUTE_NS_BEGIN

class HttpServerRequest
{
public:
	virtual std::string getMethod() const = 0;
	virtual std::string getPath() const = 0;
	virtual std::string getHost() const = 0;
	virtual std::string getScheme() const = 0;

	virtual bool hasHeader(const std::string& key) const = 0;
	virtual std::string getHeader(const std::string& key, const std::string& defaultValue = std::string()) = 0;
	virtual std::vector<std::string> getHeaders(const std::string& key) const = 0;

	virtual bool hasParameter(const std::string& key) const = 0;
	virtual std::string getParameter(const std::string& name, const std::string& defaultValue = std::string()) const = 0;
	virtual std::vector<std::string> getParameters(const std::string& name) const = 0;
};

LIBHTTPROUTE_NS_END