#pragma once

#include <exception>
#include <string>
#include <regex>

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

private:
	std::string _message;
};

/*
	Escapes all regex based characters with a `\`.
	Characters: `. ^ $ | ( ) [ ] { } * + ? \`

	\source https://stackoverflow.com/questions/1252992/how-to-escape-a-string-for-use-in-boost-regex/1253004#1253004
*/
inline std::string
mf_regex_escape(std::string pattern)
{
	const std::regex esc("[.^$|()\\[\\]{}*+?\\\\]");
	const std::string rep("\\\\&");
	return std::regex_replace(pattern, esc, rep,
							  std::regex_constants::match_default |
							  std::regex_constants::format_sed);
}

LIBHTTPROUTE_NS_END