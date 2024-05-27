local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function main_server()
  print("\n\tslaslasla  \n")
  return "sla"
end

local i = 3000
serjao.initserver(i, main_server)
