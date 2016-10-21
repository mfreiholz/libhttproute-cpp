#include <regex>
#include <vector>
#include "match.h"
#include "route_match.h"
#include "request.h"

LIBHTTPROUTE_NS_BEGIN

/****************** MatchRouteRegexp *****************************************/

class MatchTplGroup
{
public:
	int begin = -1;
	int end = -1;
	std::string group;
	std::string name;
	std::string validation;
};

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

	void parseGroups()
	{
		const auto indices = braceIndices(_tpl);
		for (size_t i = 0; i < indices.size(); i = i + 2)
		{
			MatchTplGroup g;
			g.begin = indices[i];
			g.end = indices[i + 1];
			g.group = _tpl.substr(g.begin, g.end + 1 - g.begin);

			size_t pos = std::string::npos;
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

	void buildRegexp()
	{
		std::string str;
		if (_groups.size() <= 0)
		{
			str = mf_regex_escape(_tpl);
			if (_matchPath /*&& _matchPathTrailingSlash*/)
			{
				str.append("[/]*");
			}
			_rx = std::regex(str);
			return;
		}

		size_t gindex = 0;
		for (size_t i = 0; i < _tpl.length();)
		{
			if (gindex < _groups.size() && i == _groups[gindex].begin)
			{
				const auto& g = _groups[gindex];
				if (g.validation.size() > 0)
				{
					str.append("(");
					str.append(g.validation);
					str.append(")");
				}
				else
				{
					if (_matchHost)
					{
						str.append("(");
						str.append("[^.]+");
						str.append(")");
					}
					else if (_matchPath)
					{
						str.append("(");
						str.append("[^/]+");
						str.append(")");
					}
				}
				i = g.end + 1;
				gindex++;
			}
			else
			{
				str += mf_regex_escape(std::string(&_tpl.at(i), 1));
				i++;
			}
		}
		if (_matchPath/*&& _matchPathTrailingSlash*/)
		{
			str.append("[/]*");
		}
		_rx = std::regex(str);
	}

	bool matchString(const std::string& s, RouteMatch& rm)
	{
		auto regex = _rx;
		std::smatch sm;
		if (!std::regex_match(s, sm, regex))
		{
			return false;
		}
		for (size_t i = 0; i < _groups.size(); ++i)
		{
			const auto k = _groups[i].name;
			const auto v = sm.str(i + 1);
			rm.vars.insert(std::pair<std::string, std::string>(k, v));
		}
		return true;
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

bool MatchRouteRegexp::match(const HttpServerRequest& req, RouteMatch& rm) const noexcept
{
	if (d->_matchHost)
	{
		std::string host = req.getHost();
		size_t pos;
		if ((pos = host.find(':')) != std::string::npos)
		{
			host = host.substr(pos);
		}
		return d->matchString(host, rm);
	}
	else if (d->_matchPath)
	{
		return d->matchString(req.getPath(), rm);
	}
	return false;
}

LIBHTTPROUTE_NS_END
