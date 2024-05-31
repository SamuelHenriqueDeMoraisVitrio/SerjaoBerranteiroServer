local serjao = require("serjao_berranteiro/serjao_berranteiro")
set_server.single_process = true



local function main_server(requests)

    if requests.route == "/teste" then 
            local tabelaCriadaAgora = {
                mame="aaaaaa",
                age=27,
                valor="null",
                lista = {1,2,3,"null"},
                maried=false
            }

            return serjao.send_json(tabelaCriadaAgora, 200)
    end 
    return "aaaaaaa"
end

local i = 3000
serjao.initserver(i, main_server)
