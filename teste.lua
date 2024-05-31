local serjao = require("serjao_berranteiro/serjao_berranteiro")
set_server.single_process = true

local tabelaCriadaAgora = {
    name='Nateus',
    age=27,
    maried=false,
    sons={
        {name='son1',maried='null'}
    }
}

local function main_server(requests)

   return serjao.send_json(tabelaCriadaAgora, 200)
end

local i = 3000
serjao.initserver(i, main_server)
