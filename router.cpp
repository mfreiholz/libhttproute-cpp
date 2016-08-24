#include <vector>
#include "router.h"
#include "route.h"
#include "route_match.h"
#include "route_handler.h"

LIBHTTPROUTE_NS_BEGIN

class Router::Private
{
public:
	std::vector<std::shared_ptr<Route> > routes;
};

Router::Router() :
	d(std::make_unique<Private>())
{}

Router::~Router()
{}

Route&
Router::newRoute()
{
	auto r = std::make_shared<Route>();
	d->routes.push_back(r);
	return *r.get();
}

std::tuple<std::shared_ptr<Route>, RouteMatch>
Router::findRoute(const HttpServerRequest& req) const
{
	std::shared_ptr<Route> route;
	RouteMatch rm;
	for (auto r : d->routes)
	{
		RouteMatch tmprm;
		if (r->matches(req, tmprm))
		{
			rm = std::move(tmprm);
			route = r;
			break;
		}
	}
	return std::make_tuple(route, std::move(rm));
}

bool
Router::handleRequest(const HttpServerRequest& req)
{
	try
	{
		// find matching route
		auto t = findRoute(req);
		if (!std::get<0>(t))
		{
			return false;
		}

		// handle request with matching route
		auto handler = std::get<0>(t)->handler();
		if (!handler)
			return false;

		handler->handle(req, std::get<1>(t));
		return true;
	}
	catch (Exception& e)
	{
		throw e;
	}
	catch (std::exception& e)
	{
		throw e;
	}
	catch (...)
	{
		throw;
	}
	return false;
}

LIBHTTPROUTE_NS_END