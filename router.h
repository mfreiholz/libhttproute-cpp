#pragma once

#include <memory>
#include <tuple>
#include "defines.h"

LIBHTTPROUTE_NS_BEGIN
class Route;

class Router
{
public:
	Router();
	~Router();
	Route& newRoute();
	std::tuple<std::shared_ptr<Route>, RouteMatch> findRoute(const HttpServerRequest& req) const;

	/*
		\return Returns `true`, if a Route for the request has been found, otherwise `false`.
	*/
	bool handleRequest(const HttpServerRequest& req);

private:
	class Private;
	std::unique_ptr<Private> d;
};

LIBHTTPROUTE_NS_END