local serjao = require("serjao_berranteiro/serjao_berranteiro")


local num = 0
set_server.single_process = true;

---@param request Request
local function teste(request)

   if request.route == "/teste" then
    	return html(body(h1("aaaaa")))
   end

   return {
        nome="mateus"
   }
end

serjao.server(3000,4000,teste)