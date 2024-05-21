init(8000);

route("/", function(request)
	return { status = 200, content_type = "text/plain", body = "Opa!" };
end);
maskroute("/public", "test");

listen(20);
