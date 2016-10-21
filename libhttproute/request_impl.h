#pragma once

#include "request.h"

#include <map>

LIBHTTPROUTE_NS_BEGIN

/*
	HttpServerRequestImpl is a default implementation of the
	AbstractHttpServerRequest. It is mainly used for testing, but you can use it
	for your real project, too. All members are public so you could store your
	parsed HTTP request directly into this class and use it for routing.
*/
class HttpServerRequestImpl :
	public HttpServerRequest
{
public:
	HttpServerRequestImpl();
	HttpServerRequestImpl(std::string method, std::string path, std::string host = "localhost", std::string scheme = "https");
	virtual ~HttpServerRequestImpl();

	virtual std::string getMethod() const;
	virtual std::string getPath() const;
	virtual std::string getHost() const;
	virtual std::string getScheme() const;

	virtual bool hasHeader(const std::string& key) const;
	virtual std::string getHeader(const std::string& key, const std::string& defaultValue = std::string());
	virtual std::vector<std::string> getHeaders(const std::string& key) const;

	virtual bool hasParameter(const std::string& key) const;
	virtual std::string getParameter(const std::string& name, const std::string& defaultValue = std::string()) const;
	virtual std::vector<std::string> getParameters(const std::string& name) const;

public:
	std::string _method;
	std::string _path;
	std::string _host;
	std::string _scheme;
	std::map<std::string, std::vector<std::string> > _headers;
	std::map<std::string, std::vector<std::string> > _params;
};

LIBHTTPROUTE_NS_END