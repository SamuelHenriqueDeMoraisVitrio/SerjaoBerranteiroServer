local serjao = require("serjao_berranteiro/serjao_berranteiro")


local num = 0
set_server.single_process = true;

---@param request Request
local function teste(request)


   if request.route == "/teste" then
    	return html(body(h1("aaaaa")))
   end
   if request.route == "/a" then
   	  local v = nil .. 10
   end
   return {
        nome="mateus"
   },404
end

serjao.server(3010, teste)