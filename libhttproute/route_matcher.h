#pragma once

#include "defines.h"

LIBHTTPROUTE_NS_BEGIN

class LIBHTTPROUTE_EXPORT_API RouteMatcher
{
public:
	virtual bool match(const HttpServerRequest& req, RouteMatch& rm) const = 0;
};

LIBHTTPROUTE_NS_END