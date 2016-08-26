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

	/*
		findRoute searches for the matching `Route` of the request `req`.
		\param req
		\return Returns a valid `Route` as `std::get<0>(.)` result.
			If no route matches the request, an invalid `Route` object will be returned.
	*/
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