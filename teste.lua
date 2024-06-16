local serjao = require("serjao_berranteiro/serjao_berranteiro")

function printTable(t, indent)
  indent = indent or 0
  local indentStr = string.rep(" ", indent)

  for key, value in pairs(t) do
    if type(value) == "table" then
      print(indentStr .. tostring(key) .. ":")
      printTable(value, indent + 4)
    else
      print(indentStr .. tostring(key) .. ": " .. tostring(value))
    end
  end
end

local num = 0
set_server.single_process = true;


---@param request Request
local function teste(request)

  print(request.header.size)

  if request.route == "/teste" then
    return html(body(h1("aaaaa")))
  end
  if request.route == "/a" then
    local v = nil .. 10
  end
  return {
    nome="mateus"
  },404
end

serjao.server(3000, 5000, teste)
--serjao.desktop("chromium", teste, 800, 400)