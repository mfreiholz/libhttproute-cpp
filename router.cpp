#include <list>
#include "router.h"
#include "route.h"
#include "route_match.h"
#include "route_handler.h"

LIBHTTPROUTE_NS_BEGIN

class Router::Private
{
public:
	std::list<std::shared_ptr<Route> > routes;
};

Router::Router() :
	d(std::make_unique<Private>())
{}

Router::~Router()
{}

Route& Router::newRoute()
{
	auto r = std::make_shared<Route>();
	d->routes.push_back(r);
	return *r.get();
}

bool Router::handleRequest(const HttpServerRequest& req)
{
	try
	{
		// find matching route
		RouteMatch rm;
		std::shared_ptr<Route> route;
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
		if (!route)
		{
			return false;
		}

		// handle request with matching route
		auto handler = route->handler();
		if (!handler)
			return false;

		handler->handle(req, rm);
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