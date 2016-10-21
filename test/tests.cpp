#include "libhttproute/defines.h"
#include "libhttproute/router.h"
#include "libhttproute/route.h"
#include "libhttproute/route_handler.h"
#include "libhttproute/route_matcher_impl.h"
#include "libhttproute/request_impl.h"
#include "gtest/gtest.h"

/****************** Matcher Tests ********************************************/

TEST(Main, MethodMatcher)
{
	HR_NS::RouteMethodMatcher m("GET");
	HR_NS::RouteMatch rm;

	HR_NS::HttpServerRequestImpl req("GET", "/", "localhost");
	ASSERT_TRUE(m.match(req, rm));
}

TEST(Main, PathPrefixMatcher)
{
	HR_NS::RoutePathPrefixMatcher m("/service");
	HR_NS::RouteMatch rm;

	HR_NS::HttpServerRequestImpl req("GET", "/service/search", "localhost");
	ASSERT_TRUE(m.match(req, rm));
}

TEST(Main, PathMatcher)
{
	HR_NS::RoutePathMatcher m("/products");
	HR_NS::RouteMatch rm;

	HR_NS::HttpServerRequestImpl req("GET", "/products", "localhost");
	ASSERT_TRUE(m.match(req, rm));

	HR_NS::HttpServerRequestImpl req2("GET", "/products/", "localhost");
	ASSERT_TRUE(m.match(req2, rm));
}

TEST(Main, PathMatcher2)
{
	HR_NS::RoutePathMatcher m("/products/{id}");

	HR_NS::HttpServerRequestImpl req("GET", "/products/42", "localhost");
	HR_NS::RouteMatch rm;
	ASSERT_TRUE(m.match(req, rm));
	ASSERT_STREQ(rm.vars.find("id")->second.c_str(), "42");

	HR_NS::HttpServerRequestImpl req2("GET", "/products/42/", "localhost");
	HR_NS::RouteMatch rm2;
	ASSERT_TRUE(m.match(req2, rm2));
	ASSERT_STREQ(rm2.vars.find("id")->second.c_str(), "42");

	HR_NS::HttpServerRequestImpl req3("GET", "/products/one", "localhost");
	HR_NS::RouteMatch rm3;
	ASSERT_TRUE(m.match(req3, rm3));
	ASSERT_STREQ(rm3.vars.find("id")->second.c_str(), "one");

	HR_NS::HttpServerRequestImpl req4("GET", "/products/one/", "localhost");
	HR_NS::RouteMatch rm4;
	ASSERT_TRUE(m.match(req4, rm4));
	ASSERT_STREQ(rm4.vars.find("id")->second.c_str(), "one");
}

TEST(Main, PathMatcher3)
{
	HR_NS::RoutePathMatcher m("/products/{id:[0-9]+}");

	HR_NS::HttpServerRequestImpl req("GET", "/products/42", "localhost");
	HR_NS::RouteMatch rm;
	ASSERT_TRUE(m.match(req, rm));
	ASSERT_STREQ(rm.vars.find("id")->second.c_str(), "42");

	HR_NS::HttpServerRequestImpl req2("GET", "/products/one", "localhost");
	HR_NS::RouteMatch rm2;
	ASSERT_FALSE(m.match(req2, rm2));
	ASSERT_TRUE(rm2.vars.find("id") == rm2.vars.end());
}

TEST(Main, HeaderMatcher)
{
	HR_NS::RouteHeaderMatcher m("X-ApiKey", "1234");

	HR_NS::HttpServerRequestImpl req("GET", "/");
	req._headers.insert(std::pair<std::string, std::vector<std::string> >("X-ApiKey", { "1234" }));
	HR_NS::RouteMatch rm;
	ASSERT_TRUE(m.match(req, rm));

	HR_NS::HttpServerRequestImpl req2("GET", "/");
	req2._headers.insert(std::pair<std::string, std::vector<std::string> >("X-ApiKey", { "nope!" }));
	HR_NS::RouteMatch rm2;
	ASSERT_FALSE(m.match(req2, rm2));
}

TEST(Main, HeaderMatcher2)
{
	HR_NS::RouteHeaderMatcher m("X-ApiKey", "");

	HR_NS::HttpServerRequestImpl req("GET", "/");
	req._headers.insert(std::pair<std::string, std::vector<std::string> >("X-ApiKey", { "doesn't matter!" }));
	HR_NS::RouteMatch rm;
	ASSERT_TRUE(m.match(req, rm));
}

TEST(Main, HostMatcher)
{
	HR_NS::RouteHostMatcher m("www.example.de");

	HR_NS::HttpServerRequestImpl req("GET", "/", "www.example.de");
	HR_NS::RouteMatch rm;
	ASSERT_TRUE(m.match(req, rm));

	HR_NS::HttpServerRequestImpl req2("GET", "/", "www-example-de");
	HR_NS::RouteMatch rm2;
	ASSERT_FALSE(m.match(req2, rm2));
}

TEST(Main, HostMatcher2)
{
	HR_NS::RouteHostMatcher m("{subdomain}.example.de");

	HR_NS::HttpServerRequestImpl req("GET", "/", "sub1.example.de");
	HR_NS::RouteMatch rm;
	ASSERT_TRUE(m.match(req, rm));
	ASSERT_STREQ(rm.vars.find("subdomain")->second.c_str(), "sub1");

	// tests regex
	HR_NS::HttpServerRequestImpl req2("GET", "/", "sub1.example-de");
	HR_NS::RouteMatch rm2;
	ASSERT_FALSE(m.match(req2, rm2));
}

TEST(Main, HostMatcher3)
{
	HR_NS::RouteHostMatcher m("{part1:[a-z]+}.example.de");

	HR_NS::HttpServerRequestImpl req("GET", "/", "one.example.de");
	HR_NS::RouteMatch rm;
	ASSERT_TRUE(m.match(req, rm));

	HR_NS::HttpServerRequestImpl req2("GET", "/", "one1.example.de");
	HR_NS::RouteMatch rm2;
	ASSERT_FALSE(m.match(req2, rm2));
}

TEST(Main, QueryMatcher)
{
	HR_NS::RouteQueryParamsMatcher m(std::map<std::string, std::string> {std::pair<std::string, std::string>("foo", "bar")});

	HR_NS::HttpServerRequestImpl req("GET", "/");
	req._params.insert(std::pair<std::string, std::vector<std::string> >("foo", std::vector<std::string> {"bar"}));
	HR_NS::RouteMatch rm;
	ASSERT_TRUE(m.match(req, rm));
}

TEST(Main, SchemeMatcher)
{
	HR_NS::RouteSchemeMatcher m(std::vector<std::string> { "http" });

	HR_NS::HttpServerRequestImpl req("GET", "/", "example.de", "http");
	HR_NS::RouteMatch rm;
	ASSERT_TRUE(m.match(req, rm));

	HR_NS::HttpServerRequestImpl req2("GET", "/", "example.de", "https");
	HR_NS::RouteMatch rm2;
	ASSERT_FALSE(m.match(req2, rm2));
}

/****************** Router Tests *********************************************/

class TestRouteHandler : public HR_NS::RouteHandler
{
public:
	virtual void handle(const HR_NS::HttpServerRequest& req, const HR_NS::RouteMatch& m)
	{}
};

TEST(Main, Router)
{
	HR_NS::Router router;

	auto h1 = std::make_shared<TestRouteHandler>();
	router.newRoute()
	.byMatcher(std::make_shared<HR_NS::RouteMethodMatcher>("GET"))
	.byMatcher(std::make_shared<HR_NS::RoutePathPrefixMatcher>("/service"))
	.withHandler(h1);

	auto h2 = std::make_shared<TestRouteHandler>();
	router.newRoute()
	.byMatcher(std::make_shared<HR_NS::RoutePathMatcher>("/article/{id:[0-9]+}"))
	.withHandler(h2);

	auto h3 = std::make_shared<TestRouteHandler>();
	router.newRoute()
	.byMatcher(std::make_shared<HR_NS::RoutePathMatcher>("/article/{id}"))
	.withHandler(h3);

	HR_NS::HttpServerRequestImpl r1("GET", "/service/search/123");
	ASSERT_TRUE(std::get<0>(router.findFirstRoute(r1))->handler() == h1);

	HR_NS::HttpServerRequestImpl r2("GET", "/article/123");
	ASSERT_TRUE(std::get<0>(router.findFirstRoute(r2))->handler() == h2);

	HR_NS::HttpServerRequestImpl r3("GET", "/article/one123");
	ASSERT_TRUE(std::get<0>(router.findFirstRoute(r3))->handler() == h3);
}

/****************** Test Main ************************************************/

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}