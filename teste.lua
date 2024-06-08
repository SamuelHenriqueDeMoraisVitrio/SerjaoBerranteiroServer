local serjao = require("serjao_berranteiro/serjao_berranteiro")

set_server.single_process = true

local function teste(request)

        if request.route == "/kill" then
                serjao.kill()
        end

        return serjao.send_text("html", 200)

end

serjao.initserver(3000,4000,teste)