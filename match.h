#pragma once

#include <string>
#include <regex>
#include "defines.h"

LIBHTTPROUTE_NS_BEGIN
class Match;

class MatchTplGroup
{
public:
	int begin = -1;
	int end = -1;
	std::string group;
	std::string name;
	std::string validation;
};

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
	MatchRouteRegexp(const std::wstring& tpl, bool matchHost, bool matchPath);
	~MatchRouteRegexp();

	/*
		checks whether this `MatchRouteRegexp` matches the current request
		`req` and stores collected data to `m`.

		\thread-safe
	*/
	bool match(const HttpServerRequest& req, Match& m) const noexcept;

private:
	std::string _tpl;
	bool _matchHost = false;
	bool _matchPath = false;
	std::vector<MatchTplGroup> _groups;
	std::regex _rx;
};

LIBHTTPROUTE_NS_END