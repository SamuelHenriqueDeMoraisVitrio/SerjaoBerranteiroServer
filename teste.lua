local serjao = require("serjao_berranteiro/serjao_berranteiro")
set_server.single_process = true

local function main_server(requests)

   return serjao.send_file("serjao_server.zip",200,"application/zip")

end

local i = 3000
serjao.initserver(i, main_server)
