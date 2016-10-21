#include "request_impl.h"

LIBHTTPROUTE_NS_BEGIN

HttpServerRequestImpl::HttpServerRequestImpl() :
	HttpServerRequest()
{}

HttpServerRequestImpl::HttpServerRequestImpl(std::string method, std::string path, std::string host, std::string scheme) :
	HttpServerRequest(),
	_method(method),
	_path(path),
	_host(host),
	_scheme(scheme)
{}

HttpServerRequestImpl::~HttpServerRequestImpl()
{}

std::string
HttpServerRequestImpl::getMethod() const
{
	return _method;
}

std::string
HttpServerRequestImpl::getPath() const
{
	return _path;
}

std::string
HttpServerRequestImpl::getHost() const
{
	return _host;
}

std::string
HttpServerRequestImpl::getScheme() const
{
	return _scheme;
}

bool
HttpServerRequestImpl::hasHeader(const std::string& key) const
{
	return _headers.find(key) != _headers.end();
}

std::string
HttpServerRequestImpl::getHeader(const std::string& key, const std::string& defaultValue)
{
	auto itr = _headers.find(key);
	if (itr != _headers.end() && itr->second.size() > 0)
	{
		return itr->second[0];
	}
	return defaultValue;
}

std::vector<std::string>
HttpServerRequestImpl::getHeaders(const std::string& key) const
{
	auto itr = _headers.find(key);
	if (itr != _headers.end())
	{
		return itr->second;
	}
	return std::vector<std::string>();
}

bool
HttpServerRequestImpl::hasParameter(const std::string& key) const
{
	return _params.find(key) != _params.end();
}

std::string
HttpServerRequestImpl::getParameter(const std::string& name, const std::string& defaultValue) const
{
	auto itr = _params.find(name);
	if (itr != _params.end() && itr->second.size() > 0)
	{
		return itr->second[0];
	}
	return defaultValue;
}

std::vector<std::string>
HttpServerRequestImpl::getParameters(const std::string& name) const
{
	auto itr = _params.find(name);
	if (itr != _params.end())
	{
		return itr->second;
	}
	return std::vector<std::string>();
}

LIBHTTPROUTE_NS_END