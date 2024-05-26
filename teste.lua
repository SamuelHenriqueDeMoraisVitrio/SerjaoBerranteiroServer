local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function main_server(request)


  local v = request.header["num1"]

  print("v: ", v)
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
