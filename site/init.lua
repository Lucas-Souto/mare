init();
listen(128);

require("elements.define");

-- Rotas
maskroute("/css", "public/css");
maskroute("/js", "public/js");
maskroute("/img", "public/img")
route("/", function(request)
	clear();

	return 
	{
		status = 200,
		content_type = "text/html",
		body = render("pages/index.html", 
		{
			updated = "01/06/2024",
			show_secret = false
		})
	};
end);

local subpages =
{
	about = "pages/about.html",
	madewith = "pages/madewith.html"
}

route("/getcard", function(request)
	if #request.route_args == 0 then return nil;
	else
		subpage = subpages[request.route_args[1]];

		if (subPage ~= nil) then return nil;
		else
			clear();

			return
			{
				status = 200,
				content_type = "text/html",
				body = render(subpage, {})
			}
		end
	end
end);
