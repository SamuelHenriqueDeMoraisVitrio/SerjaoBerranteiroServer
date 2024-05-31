local serjao = require("serjao_berranteiro/serjao_berranteiro")



local function main_server(requests)

    if requests.route == "/teste" then 
            local tabelaCriadaAgora = {
                name='Nateus',
                age=27,
                maried=false,
                sons={
                    {name='son1',maried='null'}
                }
            }

            return serjao.send_json(tabelaCriadaAgora, 200)
    end 
    return "aaaaaaa"
end

local i = 3000
serjao.initserver(i, main_server)
