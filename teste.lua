local serjao = require("serjao_berranteiro/serjao_berranteiro")


set_server.single_process = true


---@param request Request
local function teste(request)
      return serjao.send_text("aaa",200)
end

serjao.initserver(3000,4000,teste);