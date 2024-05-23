init(8000);

route("/", function(request)
	return { status = 200, content_type = "text/html", body = render("test/index.html", { title = "Meu título", test = { number = "10" } }) };
end);

listen(20);
