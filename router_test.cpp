#include "defines.h"
#include "router.h"
#include "route.h"
#include "route_matcher_impl.h"
#include "gtest/gtest.h"

/****************** HttpServerRequestTest ************************************/

class HttpServerRequestTest : public HR_NS::HttpServerRequest
{
public:
	HttpServerRequestTest(std::string method, std::string path, std::string host = "localhost", std::string scheme = "https") :
		_method(method),
		_path(path),
		_host(host),
		_scheme(scheme)
	{}

	virtual std::string getMethod() const
	{
		return _method;
	}
	virtual std::string getPath() const
	{
		return _path;
	}
	virtual std::string getHost() const
	{
		return _host;
	}
	virtual std::string getScheme() const
	{
		return _scheme;
	}
	virtual bool hasHeader(const std::string& key) const
	{
		return _headers.find(key) != _headers.end();
	}
	virtual std::string getHeader(const std::string& key, const std::string& defaultValue = std::string())
	{
		auto itr = _headers.find(key);
		if (itr != _headers.end() && itr->second.size() > 0)
		{
			return itr->second[0];
		}
		return defaultValue;
	}
	virtual std::vector<std::string> getHeaders(const std::string& key) const
	{
		auto itr = _headers.find(key);
		if (itr != _headers.end())
		{
			return itr->second;
		}
		return std::vector<std::string>();
	}
	virtual bool hasParameter(const std::string& key) const
	{
		return _params.find(key) != _params.end();
	}
	virtual std::string getParameter(const std::string& name, const std::string& defaultValue = std::string()) const
	{
		auto itr = _params.find(name);
		if (itr != _params.end() && itr->second.size() > 0)
		{
			return itr->second[0];
		}
		return defaultValue;
	}
	virtual std::vector<std::string> getParameters(const std::string& name) const
	{
		auto itr = _params.find(name);
		if (itr != _params.end())
		{
			return itr->second;
		}
		return std::vector<std::string>();
	}

public:
	std::string _method;
	std::string _path;
	std::string _host;
	std::string _scheme;
	std::map<std::string, std::vector<std::string> > _headers;
	std::map<std::string, std::vector<std::string> > _params;
};

/****************** Tests ****************************************************/

TEST(Main, MethodMatcher)
{
	HR_NS::RouteMethodMatcher m("GET");
	HR_NS::RouteMatch rm;

	HttpServerRequestTest req("GET", "/", "localhost");
	ASSERT_TRUE(m.match(req, rm));
}

TEST(Main, PathPrefixMatcher)
{
	HR_NS::RoutePathPrefixMatcher m("/service");
	HR_NS::RouteMatch rm;

	HttpServerRequestTest req("GET", "/service/search", "localhost");
	ASSERT_TRUE(m.match(req, rm));
}

TEST(Main, PathMatcher)
{
	HR_NS::RoutePathMatcher m("/products");
	HR_NS::RouteMatch rm;

	HttpServerRequestTest req("GET", "/products", "localhost");
	ASSERT_TRUE(m.match(req, rm));

	//HttpServerRequestTest req2("GET", "/products/", "localhost");
	//ASSERT_TRUE(m.match(req2, rm));
}

TEST(Main, PathMatcher2)
{
	HR_NS::RoutePathMatcher m("/products/{id}");

	HttpServerRequestTest req("GET", "/products/42", "localhost");
	HR_NS::RouteMatch rm;
	ASSERT_TRUE(m.match(req, rm));
	ASSERT_STREQ(rm.vars.find("id")->second.c_str(), "42");

	HttpServerRequestTest req2("GET", "/products/one", "localhost");
	HR_NS::RouteMatch rm2;
	ASSERT_TRUE(m.match(req2, rm2));
	ASSERT_STREQ(rm2.vars.find("id")->second.c_str(), "one");
}

TEST(Main, PathMatcher3)
{
	HR_NS::RoutePathMatcher m("/products/{id:[0-9]+}");

	HttpServerRequestTest req("GET", "/products/42", "localhost");
	HR_NS::RouteMatch rm;
	ASSERT_TRUE(m.match(req, rm));
	ASSERT_STREQ(rm.vars.find("id")->second.c_str(), "42");

	HttpServerRequestTest req2("GET", "/products/one", "localhost");
	HR_NS::RouteMatch rm2;
	ASSERT_FALSE(m.match(req2, rm2));
	ASSERT_TRUE(rm2.vars.find("id") == rm2.vars.end());
}

TEST(Main, HeaderMatcher)
{
	HR_NS::RouteHeaderMatcher m("X-ApiKey", "1234");

	HttpServerRequestTest req("GET", "/");
	req._headers.insert(std::pair<std::string, std::vector<std::string> >("X-ApiKey", { "1234" }));
	HR_NS::RouteMatch rm;
	ASSERT_TRUE(m.match(req, rm));

	HttpServerRequestTest req2("GET", "/");
	req2._headers.insert(std::pair<std::string, std::vector<std::string> >("X-ApiKey", { "nope!" }));
	HR_NS::RouteMatch rm2;
	ASSERT_FALSE(m.match(req2, rm2));
}

TEST(Main, HeaderMatcher2)
{
	HR_NS::RouteHeaderMatcher m("X-ApiKey", "");

	HttpServerRequestTest req("GET", "/");
	req._headers.insert(std::pair<std::string, std::vector<std::string> >("X-ApiKey", { "doesn't matter!" }));
	HR_NS::RouteMatch rm;
	ASSERT_TRUE(m.match(req, rm));
}

TEST(Main, HostMatcher)
{
	HR_NS::RouteHostMatcher m("www.example.de");

	HttpServerRequestTest req("GET", "/", "www.example.de");
	HR_NS::RouteMatch rm;
	ASSERT_TRUE(m.match(req, rm));
}

TEST(Main, HostMatcher2)
{
	HR_NS::RouteHostMatcher m("{subdomain}.example.de");

	HttpServerRequestTest req("GET", "/", "sub1.example.de");
	HR_NS::RouteMatch rm;
	ASSERT_TRUE(m.match(req, rm));
	ASSERT_STREQ(rm.vars.find("subdomain")->second.c_str(), "sub1");
}

TEST(Main, HostMatcher3)
{
	HR_NS::RouteHostMatcher m("{part1:[a-z]+}.example.de");

	HttpServerRequestTest req("GET", "/", "one.example.de");
	HR_NS::RouteMatch rm;
	ASSERT_TRUE(m.match(req, rm));

	HttpServerRequestTest req2("GET", "/", "one1.example.de");
	HR_NS::RouteMatch rm2;
	ASSERT_FALSE(m.match(req2, rm2));
}

TEST(Main, QueryMatcher)
{
	HR_NS::RouteQueryParamsMatcher m(std::map<std::string, std::string> {std::pair<std::string, std::string>("foo", "bar")});

	HttpServerRequestTest req("GET", "/");
	req._params.insert(std::pair<std::string, std::vector<std::string> >("foo", std::vector<std::string> {"bar"}));
	HR_NS::RouteMatch rm;
	ASSERT_TRUE(m.match(req, rm));
}

TEST(Main, SchemeMatcher)
{
	HR_NS::RouteSchemeMatcher m(std::vector<std::string> { "http" });

	HttpServerRequestTest req("GET", "/", "example.de", "http");
	HR_NS::RouteMatch rm;
	ASSERT_TRUE(m.match(req, rm));

	HttpServerRequestTest req2("GET", "/", "example.de", "https");
	HR_NS::RouteMatch rm2;
	ASSERT_FALSE(m.match(req2, rm2));
}

TEST(Main, Router)
{
	HR_NS::Router router;

	auto& r = router.newRoute();
	r.byMatcher(std::make_shared<HR_NS::RouteMethodMatcher>("GET"));
	r.byMatcher(std::make_shared<HR_NS::RoutePathPrefixMatcher>("/service"));

	EXPECT_EQ(1, 1);
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}