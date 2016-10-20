#pragma once

#include <memory>
#include "defines.h"
#include "route_handler.h"

LIBHTTPROUTE_NS_BEGIN

class Route
{
public:
	Route();
	~Route();

	/*
		Adds a rule (matcher), which needs to match to resolve this route.
		A `Route` only `matches()==true`, if all rules matches (AND-condition).

		\param matcher
			The matcher/rule, which needs to match in order to use this `Route`.
		\return
			Returns itself for chained method calls.
	*/
	Route& byMatcher(std::shared_ptr<RouteMatcher> matcher);

	/*
		Defines a `RouteHandler` for this route.

		\param handler
	*/
	Route& withHandler(std::shared_ptr<RouteHandler> handler);

	/*
		Helper for `withHandler` but can be used with a lambda.
		Example:
		`.withHandlerFunc([](const HttpServerRequest& req, const RouteMatch& m) { ... }`
	*/
	Route& withHandlerFunc(RouteHandlerFunc fn);

	/*
		Sets a custom object for this route.
	*/
	Route& withObject(void* obj);

	/*
		\return Returns the `RouteHandler` of this `Route`.
	*/
	std::shared_ptr<RouteHandler> handler() const;

	/*
		\return Returns the custom `any` object of this route or NULL.
	*/
	void* object() const;

	/*
		Matches checks whether `this` Route matches with the request `req`
		and stores matched variables from request inside the `rm` object.
	*/
	bool matches(const HttpServerRequest& req, RouteMatch& rm) const;

private:
	class Private;
	std::unique_ptr<Private> d;
};

LIBHTTPROUTE_NS_END