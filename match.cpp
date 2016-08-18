#include <regex>
#include <vector>
#include "match.h"
#include "http_server_request.h"

LIBHTTPROUTE_NS_BEGIN

/****************** MatchRouteRegexp *****************************************/

class MatchRouteRegexp::Private
{
public:
	Private()
	{}

	~Private()
	{}

	/*
		searches for indices of first level brace pairs "{}".
		\param tpl string to search in, e.g.: "blah {blubb} lala"
		\throw Exception
	*/
	std::vector<int> braceIndices(const std::string& tpl) const
	{
		std::vector<int> indices;
		int beginIndex = -1;
		int level = 0;
		for (size_t i = 0; i < tpl.length(); ++i)
		{
			switch (tpl[i])
			{
				case '{':
					level++;
					if (level == 1)
						beginIndex = i;
					break;
				case '}':
					level--;
					if (level == 0 && beginIndex >= 0)
					{
						indices.push_back(beginIndex);
						indices.push_back(i);
					}
					else if (level < 0)
					{
						throw Exception("unbalanced braces in pattern: %1");
					}
					break;
			}
		}
		if (indices.size() % 2 != 0)
		{
			throw Exception("invalid count of indices");
		}
		return indices;
	}

	void parseGroups() noexcept
	{
		const auto indices = braceIndices(_tpl);
		for (size_t i = 0; i < indices.size(); i = i + 2)
		{
			MatchTplGroup g;
			g.begin = indices[i];
			g.end = indices[i + 1];
			g.group = _tpl.substr(g.begin, g.end + 1 - g.begin);

			int pos = std::string::npos;
			if ((pos = g.group.find(':')) != std::string::npos)
			{
				g.name = g.group.substr(1, pos - 1);
				g.validation = g.group.substr(pos + 1, g.group.length() - (pos + 2));
			}
			else
			{
				g.name = g.group.substr(1, g.group.length() - 2);
			}
			_groups.push_back(std::move(g));
		}
	}

	void buildRegexp() //throws Exception
	{}

	bool matchString(const std::string& s, RouteMatch& rm)
	{
		auto regex = _rx;
		return false;
	}

public:
	std::string _tpl;
	bool _matchHost = false;
	bool _matchPath = false;
	std::vector<MatchTplGroup> _groups;
	std::regex _rx;
};

MatchRouteRegexp::MatchRouteRegexp(const std::string& tpl, bool matchHost, bool matchPath) :
	d(std::make_unique<Private>())
{
	if (matchHost == matchPath)
		throw Exception("pre-condition failed: matchHost == matchPath");

	d->_tpl = tpl;
	d->_matchHost = matchHost;
	d->_matchPath = matchPath;
	d->parseGroups();
	d->buildRegexp();
}

MatchRouteRegexp::~MatchRouteRegexp()
{}

bool MatchRouteRegexp::match(const HttpServerRequest& req, RouteMatch& m) const noexcept
{
	if (d->_matchHost)
	{
		std::string host = req.getHost();
		int pos;
		if ((pos = host.find(':')) != std::string::npos)
		{
			host = host.substr(pos);
		}
		return true;//matchString(host, rm);
	}
	else if (d->_matchPath)
	{
		return true;//matchString(req.getPath(), rm);
	}
	return false;
}

LIBHTTPROUTE_NS_END