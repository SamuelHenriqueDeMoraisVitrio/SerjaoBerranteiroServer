local serjao = require("serjao_berranteiro/serjao_berranteiro")
local fluid = require("testeFluidJson/luaFluidJson/luaFluidJson")

set_server.single_process = true
-- set_server.nullterminator="casa"

local tabelaCriadaAgora = {
    mame="aaaaaa",
    age=27,
    valor="casa",
    lista = {1,2,3,"null"},
    maried=false
}

local function main_server(request)

    if request.route == "/copy" then
        local outra = request.read_json_body(100000000)
        if outra == nil then
            print("Error in copy to outra.\n")
            return "Deu errado"
        end

        print(fluid.dumps_to_string(outra, true))
        return "opa copy Deu certo"
    end

    return "epa main Deu certo"
---- chromium-browser   --app=http://youtube.com --window-size=300x300
end

serjao.initserver(3000, main_server)