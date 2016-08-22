#pragma once

#include <string>
#include <memory>
#include "defines.h"

LIBHTTPROUTE_NS_BEGIN

/*
	builds a single big RegExp to match against host or path of current HTTP
	request.
*/
class MatchRouteRegexp
{
public:
	/*
		\pre matchHost != matchPath
		\throw Exception
	*/
	MatchRouteRegexp(const std::string& tpl, bool matchHost, bool matchPath);
	~MatchRouteRegexp();

	/*
		checks whether this `MatchRouteRegexp` matches the current request
		`req` and stores collected data to `m`.

		\thread-safe
	*/
	bool match(const HttpServerRequest& req, RouteMatch& m) const noexcept;

private:
	class Private;
	std::unique_ptr<Private> d;
};

LIBHTTPROUTE_NS_END