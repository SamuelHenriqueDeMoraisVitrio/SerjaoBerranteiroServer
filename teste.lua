local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function main_server(requests)
  local i = 1
  while true do
    header = requests.header[i]
    i = i + 1
    if header[1] == nil then
      header = { "key12", "num1" }
      break
    end
  end

  print(header[1])

  -- serjao.response.send_text = "hello"
  -- serjao.response.sen_var_html()

  local sla = serjao.response.send_text("Texto")
  print(sla)
  return "sa"
end

local i = 3000
serjao.initserver(i, main_server)
