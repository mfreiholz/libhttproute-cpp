#include "defines.h"
#include "router.h"
#include "route.h"
#include "route_matcher_impl.h"

int main(int argc, char** argv)
{
	HR_NS::Router router;

	auto& r = router.newRoute();
	r.byMatcher(std::make_shared<HR_NS::RouteMethodMatcher>("GET"));
	r.byMatcher(std::make_shared<HR_NS::RoutePathPrefixMatcher>("/service"));

	return 0;
}