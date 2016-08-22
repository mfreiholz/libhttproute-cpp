#pragma once

#include <memory>
#include "defines.h"

LIBHTTPROUTE_NS_BEGIN

class Route
{
public:
	Route();
	~Route();

	Route& byMatcher(std::shared_ptr<RouteMatcher> matcher);

	Route& withHandler(std::shared_ptr<RouteHandler> handler);
	std::shared_ptr<RouteHandler> handler() const;

	virtual bool matches(const HttpServerRequest& req, RouteMatch& rm) const;

private:
	class Private;
	std::unique_ptr<Private> d;
};

LIBHTTPROUTE_NS_END