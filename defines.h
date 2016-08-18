#pragma once

#include <exception>
#include <string>

/* namespace defines */

#define LIBHTTPROUTE_NS_BEGIN namespace mf { namespace httproute {
#define LIBHTTPROUTE_NS_END }}
#define HR_NS(X) ::mf::httproute

/* declarations */

LIBHTTPROUTE_NS_BEGIN
class HttpServerRequest;
class RouteMatcher;
class RouteHandler;
class RouteMatch;

class Exception : public std::exception
{
public:
	explicit Exception(const std::string& message) noexcept;
	virtual ~Exception();

	virtual const std::string& message() const noexcept
	{
		return _message;
	}

	virtual const char* what() const noexcept
	{
		return _message.c_str();
	}

private:
	std::string _message;
};

LIBHTTPROUTE_NS_END