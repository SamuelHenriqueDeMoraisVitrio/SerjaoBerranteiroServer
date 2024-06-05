local serjao = require("serjao_berranteiro/serjao_berranteiro")

set_server.single_process = true
set_server.nullterminator="casa"

local function main_server(request)


    return "conteudo lido"
end

serjao.initdesktop("chromium-browser",main_server)