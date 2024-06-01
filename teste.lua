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

    if  request.route == "/upload" then
        local content = request.read_body(1000000)
        local tipe = request.header["tipo"]
        if tipo == nill then
            return "Tipo de arquivo não informado."
        end
        if content == nil then
            return "Body não informado"
        end
        if tipo == "image/jpg" then
            io.open("QualquerNome.jpg", "wb"):write(content)
        elseif tipo == "image/png" then
            io.open("QualquerNome.png", "wb"):write(content)
        else
            return "Tipo invalido"
        end

        return "conteudo livre"

    end
    return serjao.send_raw(teste,"image/jpg",200);



end

serjao.initserver(3000, main_server)