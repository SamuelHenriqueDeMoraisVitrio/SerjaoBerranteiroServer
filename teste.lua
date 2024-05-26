local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function main_server(request)


  local key_valueP = request.params[1]
  local key_valueH = request.header[1]

  print("\tHeader\tParam")
  print("key: ", key_valueH.key, "\t", key_valueP.key)
  print("value: " .. key_valueH.value .. "\t" .. key_valueP.value)
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
