local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function main_server(request)


  local header = request.header[1]
  local param = request.params[1]

  print("v: " .. header)
  print("param: " .. param)
  return "slaaa"
end

local i = 3000

while true do

  local erro = serjao.initserver(i, main_server, true)

  i = i + 1

  if erro == false then
    break
  end
end
