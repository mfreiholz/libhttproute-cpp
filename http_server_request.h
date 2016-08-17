#pragma once

#include <string>
#include <list>

class HttpServerRequest
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