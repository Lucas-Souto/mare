init();
listen(20);

element("card", "card.html");

local index = render("index.html", 
{
	title = "Meu título",
	test = { number = 128, other = "sus" },
	check = true,
	subcheck = false
});

route("/", function(request)
	return
	{
		status = 200,
		content_type = "text/html",
		body = index
	};
end);

route("users", function(request)
	if (#request.route_args == 0) then return nil; end;

	return { status = 200, content_type = "text/html", body = "<h1>Opa!" .. tostring(request.route_args[1]) };
end);

maskroute("/assets", "public");

route404("<h1>Nothing</h1>");

print("Tudo pronto!");
