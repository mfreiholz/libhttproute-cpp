#include <list>
#include "route.h"
#include "route_matcher.h"
#include "route_handler.h"

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
	Private() {}

public:
	std::list<std::shared_ptr<RouteMatcher> > matchers;
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
	return *this;
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