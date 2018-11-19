local redis = require("resty.redis")
local red = redis:new()

red:set_timeout(1000)

local ok, emsg = red:connect("127.0.0.1", 6379)
if not ok then
	--Not show this message in webserver
	ngx.say("<p>failed to connect: ", emsg, "</p>")
	return
end

local res
res, emsg = red:get("blogSums")
if not res then
	ngx.say("<p>Failed to pull blog summary: ", emsg, "</p>")
elseif res == ngx.null then
	ngx.say("<p>Blog summary not exist.</p>")
else
	ngx.say(res)
end

ok, emsg = red:close()
if not ok then
	ngx.say("<p>Failed to close redis connection:", emsg, "</p>")
end
