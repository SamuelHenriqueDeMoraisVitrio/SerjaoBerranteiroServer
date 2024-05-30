local serjao = require("serjao_berranteiro/serjao_berranteiro")
set_server.single_process = true

local function main_server(requests)
   local json = "{\"name\":\"CWebStudio\",\"version\":\"1.0.0\"}"
   return serjao.send_json_string(json, 200);
end

local i = 3000
serjao.initserver(i, main_server)
