local serjao = require("serjao_berranteiro/serjao_berranteiro")

set_server.single_process = true
set_server.nullterminator="casa"

local tabelaCriadaAgora = {
    mame="aaaaaa",
    age=27,
    valor="casa",
    lista = {1,2,3,"null"},
    maried=false
}

local function main_server(request)

    if request.route == "/copy" then
        local outra = request.read_json_body(1000000)
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