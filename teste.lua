

local serjao = require("serjao_berranteiro/serjao_berranteiro")


local function main_server(request)

  local teste = request.params["teste"]

  return "slaaa"
end


local i = 3000

while true do

  local erro = serjao.initserver(i, main_server)

  i = i + 1

  if erro == false then
    break
  end
end
