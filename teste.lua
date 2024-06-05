local serjao = require("serjao_berranteiro/serjao_berranteiro")

set_server.single_process = true
set_server.nullterminator="casa"

local function main_server(request)

    if "/copy" == request.route then
        return serjao
    end

    return serjao.send_file("teste.html", 200)
end

serjao.initdesktop("chromium-browser",main_server)