#pragma once

#include <memory>
#include <tuple>
#include "defines.h"

LIBHTTPROUTE_NS_BEGIN
class Route;

class LIBHTTPROUTE_EXPORT_API Router
{
public:
	Router();
	~Router();

	/*
		newRoute creates a new `Route` and returns a reference to it.
		The return value, will always be valid.
	*/
	Route& newRoute();

	/*
		findRoute searches for the first matching `Route` of the request `req`.
		\param req
		\return Returns a valid `Route` as `std::get<0>(.)` result.
			If no route matches the request, an invalid `Route` object will be returned.
	*/
	std::tuple<std::shared_ptr<Route>, RouteMatch> findFirstRoute(const HttpServerRequest& req) const;

	/*
		findRoutes searches for all matching routes of the request `req`.
		\return Returns a list with valid tuples.
			If no route matches the request, the list will be empty.
	*/
	std::vector<std::tuple<std::shared_ptr<Route>, RouteMatch> > findRoutes(const HttpServerRequest& req) const;

	/*
		very basic implementation to call `RouteHandler::handle()` on the matching
		route.
		\param req
			the current HTTP request to handle
		\return Returns `true`, if a Route for the request has been found, otherwise `false`.
		\throw Exception
		\throw std::exception
	*/
	bool handleRequest(const HttpServerRequest& req);

private:
	class Private;
	std::unique_ptr<Private> d;
};

LIBHTTPROUTE_NS_END