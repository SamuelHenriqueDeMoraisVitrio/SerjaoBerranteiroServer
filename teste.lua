local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function main_server()

  return "sla"
end

serjao.initserver(3001, main_server, true);
