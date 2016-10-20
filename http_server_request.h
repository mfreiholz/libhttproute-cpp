#pragma once

#include <string>
#include <vector>
#include "defines.h"

LIBHTTPROUTE_NS_BEGIN

class HttpServerRequest
{
public:
	/*
		\return Returns the type of request,
			e.g. "GET", "POST", ...
	*/
	virtual std::string getMethod() const = 0;

	/*
		\return Returns the path of URI,
			e.g. "/service/myfile.jpg"
	*/
	virtual std::string getPath() const = 0;

	/*
		\return Returns the host. It may also include the port,
			e.g. "localhost", "localhost:1337"
	*/
	virtual std::string getHost() const = 0;

	/*
		\return Returns the scheme of the request,
			e.g. "http", "https", "ftp"
	*/
	virtual std::string getScheme() const = 0;

	virtual bool hasHeader(const std::string& key) const = 0;
	virtual std::string getHeader(const std::string& key, const std::string& defaultValue = std::string()) = 0;
	virtual std::vector<std::string> getHeaders(const std::string& key) const = 0;

	virtual bool hasParameter(const std::string& key) const = 0;
	virtual std::string getParameter(const std::string& name, const std::string& defaultValue = std::string()) const = 0;
	virtual std::vector<std::string> getParameters(const std::string& name) const = 0;
};

LIBHTTPROUTE_NS_END