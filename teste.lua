local serjao = require("serjao_berranteiro/serjao_berranteiro")

set_server.single_process = true
set_server.nullterminator="casa"

local function main_server(request)


    return "Deu certo"
end

serjao.initdesktop(3000, 3002, "chromium", main_server)