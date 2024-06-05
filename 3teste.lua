local serjao = require("serjao_berranteiro/serjao_berranteiro")

set_server.single_process = true

local function main(request)


    if request.route == "/calcula" then

        local requestJson = request.read_json_body(2000)

        print(type(requestJson.num1))

        if requestJson == nil then
            return serjao.send_text("Esse json não é compativel", 404)
        end

        if requestJson.operacao == nil then
            return serjao.send_text("Essa operação não é compativel", 404)
        end

        if requestJson.num1 == nil or type(requestJson.num1) ~= "number" then
            return serjao.send_text("Esse num1 não é compativel", 404)
        end

        if requestJson.num2 == nil or type(requestJson.num2) ~= "number" then
            return serjao.send_text("Esse num2 não é compativel", 404)
        end

        if requestJson.operacao ~= "soma" then
            return "Essa operação não conheço."
        end

        local soma = requestJson.num1 + requestJson.num2
        return serjao.send_json({Resposta = soma }, 200)
    end -- calcula

    return serjao.send_file("teste.html", 200)

end

serjao.initdesktop("chromium", main)