local serjao = require("serjao_berranteiro/serjao_berranteiro")
set_server.single_process = true

local function main_server(requests)
   return "<html><body><h1>Hello Word</h1></body></html>"
end

local i = 3000
serjao.initserver(i, main_server)
