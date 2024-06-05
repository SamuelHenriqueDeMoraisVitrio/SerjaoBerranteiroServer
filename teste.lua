local serjao = require("serjao_berranteiro/serjao_berranteiro")

set_server.single_process = true
set_server.nullterminator="casa"

local function main_server(request)

    local teste = request.read_json_body(2000)
    if teste == nil then
    	return "voce não passou o json"
    end

    print("nome:", teste.nome)
    print("idade:", teste.idade)

    return "conteudo lido"
end

serjao.initserver(3000, main_server)