#pragma once

/* namespace defines */

#define LIBHTTPROUTE_NS_BEGIN namespace mf { namespace httproute {
#define LIBHTTPROUTE_NS_END }}
#define HR_NS(X) ::mf::httproute

/* declarations */

LIBHTTPROUTE_NS_BEGIN
class HttpServerRequest;
class RouteMatcher;
class RouteHandler;
class RouteMatch;
LIBHTTPROUTE_NS_END