local serjao = require("serjao_berranteiro/serjao_berranteiro")


print(set_server.single_process)
print(set_server.use_folder)

set_server.single_process = true
set_server.use_folder = false

print(set_server.single_process)
print(set_server.use_folder)

local function main_server(requests)
  local param = requests.params[1]
  local header = requests.header["num1"]
  local ola = set_server.single_process
  print("\n\n\t", ola)
  print("\n\theader value: ", header)
  print("\n\tParams value: ", param.value, "\n\tParams key: ", param.key)
  return "sla"
end

local i = 3000
serjao.initserver(i, main_server)
