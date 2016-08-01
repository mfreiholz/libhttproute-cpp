#include "route.h"

LIBHTTPROUTE_NS_BEGIN

Route::Route() :
	d(std::make_unique<Private>())
{
}

Route::~Route()
{
}

LIBHTTPROUTE_NS_END