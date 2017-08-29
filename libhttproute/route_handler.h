#pragma once

#include <functional>
#include "defines.h"

LIBHTTPROUTE_NS_BEGIN

class LIBHTTPROUTE_EXPORT_API RouteHandler
{
public:
	virtual void handle(const HttpServerRequest& req, const RouteMatch& m) = 0;
};
typedef std::function<void(const HttpServerRequest& req, const RouteMatch& m)> RouteHandlerFunc;

LIBHTTPROUTE_NS_END