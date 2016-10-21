#pragma once

#include <map>
#include <string>
#include "defines.h"

LIBHTTPROUTE_NS_BEGIN

/*
	holds information about the matched route,
	like template string defined variables in request path.
*/
class RouteMatch
{
public:
	std::map<std::string, std::string> vars;
};

LIBHTTPROUTE_NS_END