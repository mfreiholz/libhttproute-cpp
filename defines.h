#pragma once

#include <exception>
#include <string>

/* namespace defines */

#define LIBHTTPROUTE_NS_BEGIN namespace mf { namespace httproute {
#define LIBHTTPROUTE_NS_END }}
#define HR_NS ::mf::httproute

/* declarations */

LIBHTTPROUTE_NS_BEGIN
class HttpServerRequest;
class RouteMatcher;
class RouteHandler;
class RouteMatch;

class Exception : public ::std::exception
{
public:
	Exception(const std::string& message) :
		std::exception(),
		_message(message)
	{}

	virtual ~Exception()
	{}

	virtual const std::string& message() const
	{
		return _message;
	}

	/*virtual const char* what() const
	{
		return _message.c_str();
	}*/

private:
	std::string _message;
};

LIBHTTPROUTE_NS_END
