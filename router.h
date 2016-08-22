#pragma once

#include <memory>
#include "defines.h"

LIBHTTPROUTE_NS_BEGIN
class Route;

class Router
{
public:
	Router() noexcept;
	~Router();

	Route& newRoute() noexcept;

	/*
		\return Returns `true`, if a Route for the request has been found, otherwise `false`.
	*/
	bool handleRequest(const HttpServerRequest& req) noexcept;

private:
	class Private;
	std::unique_ptr<Private> d;
};

LIBHTTPROUTE_NS_END