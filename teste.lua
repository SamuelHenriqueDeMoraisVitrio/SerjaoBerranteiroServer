local serjao = require("serjao_berranteiro/serjao_berranteiro")

set_server.single_process = true
set_server.nullterminator="casa"

local teste = io.open("static/veado.jpg", rb)

local function main_server(request)


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

    print("\n\t", type(teste), '\n')
    print('\t', serjao.read_content(teste), '\n')
    return serjao.send_file("static/veado.jpg", 200)
end

serjao.initserver(3000, main_server)