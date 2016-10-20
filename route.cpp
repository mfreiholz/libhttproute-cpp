#include <list>
#include "route.h"
#include "route_matcher.h"
#include "route_handler.h"
#include "http_server_request.h"

LIBHTTPROUTE_NS_BEGIN

/****************** RouteHandlerFuncImpl *************************************/

class RouteHandlerFuncImpl : public RouteHandler
{
public:
	RouteHandlerFuncImpl(RouteHandlerFunc f) :
		RouteHandler(),
		_fn(f)
	{}

	virtual ~RouteHandlerFuncImpl()
	{}

	virtual void handle(const HttpServerRequest& req, const RouteMatch& rm)
	{
		_fn(req, rm);
	}

private:
	RouteHandlerFunc _fn;
};

/****************** Route ****************************************************/

class Route::Private
{
public:
	Private() : obj(NULL) {}

public:
	std::list<std::shared_ptr<RouteMatcher> > matchers;
	std::shared_ptr<RouteHandler> handler;
	void* obj;
};

Route::Route() :
	d(std::make_unique<Private>())
{}

Route::~Route()
{}

Route& Route::byMatcher(std::shared_ptr<RouteMatcher> matcher)
{
	d->matchers.push_back(matcher);
	return *this;
}

Route& Route::withHandler(std::shared_ptr<RouteHandler> handler)
{
	d->handler = handler;
	return *this;
}

Route& Route::withHandlerFunc(RouteHandlerFunc fn)
{
	d->handler = std::make_shared<RouteHandlerFuncImpl>(fn);
	return *this;
}

Route& Route::withObject(void* obj)
{
	d->obj = obj;
	return *this;
}

std::shared_ptr<RouteHandler> Route::handler() const
{
	return d->handler;
}

void* Route::object() const
{
	return d->obj;
}

bool Route::matches(const HttpServerRequest& req, RouteMatch& rm) const
{
	for (const auto& m : d->matchers)
	{
		if (!m->match(req, rm))
		{
			return false;
		}
	}
	return true;
}

LIBHTTPROUTE_NS_END