local serjao = require("serjao_berranteiro/serjao_berranteiro")

set_server.single_process = true
set_server.nullterminator="casa"

local function main_server(request)
    local teste = io.open("static/veado.jpg", "rb"):read('a')

    if request.route == "/teste" then
            local tabelaCriadaAgora = {
                mame="aaaaaa",
                age=27,
                valor="casa",
                lista = {1,2,3,"null"},
                maried=false
            }

            return serjao.send_json(tabelaCriadaAgora, 200)
    end

    return serjao.send_raw(teste,"image/jpg",200);



end

serjao.initserver(3000, main_server)