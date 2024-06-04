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
        if not outra then
            print("Error in copy to outra.\n")
        end
        print(fluid.dumps_to_string(outra, true))
        return "opa copy"
    end

--
--    if outra == nil then
--        return "Body não foi gravado em 'outra'"
--    end

--    print("valor de name: " .. outra.name)

    return "epa main"

end

serjao.initserver(3000, main_server)