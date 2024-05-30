local serjao = require("serjao_berranteiro/serjao_berranteiro")
set_server.single_process = true

local function main_server(requests)
   local html = "<!doctype html><html><head><title>Hello Word</title></head><body><h1>eai parsa</h1></body></html>"
   return serjao.send_html(html, 200)
end

local i = 3000
serjao.initserver(i, main_server)
