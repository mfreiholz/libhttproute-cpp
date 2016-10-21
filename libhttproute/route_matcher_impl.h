#pragma once

#include <string>
#include <vector>
#include "defines.h"
#include "route_matcher.h"
#include "route_match.h"
#include "request.h"
#include "match.h"

LIBHTTPROUTE_NS_BEGIN

/*
	Matches by HTTP request type (GET, PUT, POST, ...).

	For example:
	RouteMethodMatcher("GET");
*/
class RouteMethodMatcher : public RouteMatcher
{
public:
	RouteMethodMatcher(const std::string& method)
		: _method(method)
	{}

	virtual ~RouteMethodMatcher()
	{}

	virtual bool match(const HttpServerRequest& req, RouteMatch& rm) const
	{
		return req.getMethod().compare(_method) == 0;
	}

private:
	std::string _method;
};


/*
	Matches HTTP request header values.
	It validates to `true`, if at least one specified value matches the header
	`key` (matches by OR-condition).

	Matching a route by AND-condition is possible by adding multiple instances
	of this class.

	For example:
	...
*/
class RouteHeaderMatcher : public RouteMatcher
{
public:
	RouteHeaderMatcher(const std::string& key, const std::vector<std::string>& values) :
		_key(key), _values(values)
	{}

	RouteHeaderMatcher(const std::string& key, const std::string& value) :
		_key(key), _values()
	{
		_values.push_back(value);
	}

	virtual ~RouteHeaderMatcher()
	{}

	virtual bool match(const HttpServerRequest& req, RouteMatch& rm) const
	{
		if (req.hasHeader(_key))
		{
			if (_values.size() <= 0 || (_values.size() == 1 && _values[0].length() == 0))
				return true;

			const auto hvals = req.getHeaders(_key);
			for (const auto& hval : hvals)
			{
				for (const auto& val : _values)
				{
					if (hval.compare(val) == 0)
						return true;
				}
			}
		}
		return false;
	}

private:
	std::string _key;
	std::vector<std::string> _values;
};

/*
	THIS is a matcher for the URL's host. It accepts a template with zero or more
	URL variables enclosed by {}. Variables can define an optional regexp pattern
	to be matched:
	- {name} matches anyhting until the next dot.
	- {name:pattern} matches the given regexp pattern.

	For example:
	"www.example.de"
	"{subdomain}.domain.com"
	"{subdomain:[a-z]+}.domain.com"

	Note: The host will never contain port information!
*/
class RouteHostMatcher : public RouteMatcher
{
public:
	RouteHostMatcher(std::string tpl) :
		_tpl(tpl),
		_rx(tpl, true, false)
	{}

	virtual ~RouteHostMatcher()
	{}

	virtual bool match(const HttpServerRequest& req, RouteMatch& rm) const
	{
		return _rx.match(req, rm);
	}

private:
	std::string _tpl;
	MatchRouteRegexp _rx;
};

/*
	THIS is a matcher for the URL's path. It accepts a template with zero or more
	URL variables enclosed by {}. The template must start with a "/".
	Variables can define an otional regexp pattern to be matched:
	- {name} matches anything until the next slash.
	- {name:pattern} matches the given regexp pattern.

	For example:
	"/products/"
	"/products/{key}"
	"/articles/{category}/{id:[0-9]+}"

	Paths with trailing slash will match, too.
*/
class RoutePathMatcher : public RouteMatcher
{
public:
	RoutePathMatcher(const std::string& tpl) :
		_tpl(tpl),
		_rx(tpl, false, true)
	{}

	virtual ~RoutePathMatcher()
	{}

	virtual bool match(const HttpServerRequest& req, RouteMatch& rm) const
	{
		return _rx.match(req, rm);
	}

private:
	std::string _tpl;
	MatchRouteRegexp _rx;
};


/*
*/
class RoutePathPrefixMatcher : public RouteMatcher
{
public:
	RoutePathPrefixMatcher(const std::string& prefix) :
		_prefix(prefix)
	{}

	virtual ~RoutePathPrefixMatcher()
	{}

	virtual bool match(const HttpServerRequest& req, RouteMatch& rm) const
	{
		return req.getPath().find(_prefix) != std::string::npos;
	}

private:
	std::string _prefix;
};


/*
	Adds a matcher for URL query values.
	It accepts a sequence of key/value pairs.
	Values may define variables.

	For example:
	...

	The above route will only match if the URL contains the defined queries values, e.g.: ?foo=bar
	It the value is an empty string, it will match any value if the key is set.

	\todo Make it possible to use RegExp as value, e.g.: h.insert("id", "{id:[0-9]+}");
*/
class RouteQueryParamsMatcher : public RouteMatcher
{
public:
	RouteQueryParamsMatcher(std::map<std::string, std::string> params) :
		_params(std::move(params))
	{}

	virtual ~RouteQueryParamsMatcher()
	{}

	virtual bool match(const HttpServerRequest& req, RouteMatch& rm) const
	{
		for (auto itr : _params)
		{
			if (!req.hasParameter(itr.first))
				return false;
			if (itr.second.length() <= 0)
				continue;
			if (req.getParameter(itr.first) != itr.second)
				return false;
		}
		return true;
	}

private:
	std::map<std::string, std::string> _params;
};


/*
*/
class RouteSchemeMatcher : public RouteMatcher
{
public:
	RouteSchemeMatcher(const std::vector<std::string>& schemes) :
		_schemes(schemes)
	{}

	RouteSchemeMatcher(const std::string& scheme) :
		_schemes()
	{
		_schemes.push_back(scheme);
	}

	virtual ~RouteSchemeMatcher()
	{}

	virtual bool match(const HttpServerRequest& req, RouteMatch& rm) const
	{
		for (const auto& scheme : _schemes)
		{
			if (scheme.compare(req.getScheme()) == 0)
				return true;
		}
		return false;
	}

private:
	std::vector<std::string> _schemes;
};

LIBHTTPROUTE_NS_END