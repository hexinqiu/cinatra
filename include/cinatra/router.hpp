
#pragma once

#include <string>
#include <functional>
#include "http_parser.hpp"
#include "request.hpp"
#include "response.hpp"

namespace cinatra
{
	class Cinatra;

	class Router
	{
	public:
		Router& method(http_method method)
		{
			method_ = method;
			return *this;
		}

		typedef std::function<void(const Request&, Response&)> handler_t;
		Router& set_handler(handler_t handler)
		{
			handler_ = handler;
			return *this;
		}

		Router& operator()(handler_t handler)
		{
			return set_handler(handler);
		}
	private:
		friend Cinatra;
		Router(const std::string& rule)
			:rule_(rule), method_((http_method)-1)
		{}

		bool handle(const Request& req, Response& res)
		{
			if (req.path != rule_)
			{
				return false;
			}

			if (method_ != (http_method)-1 && method_ != req.method)
			{
				return false;
			}

			handler_(req, res);
			return true;
		}
	private:
		std::string rule_;
		http_method method_;
		handler_t handler_;
	};
}