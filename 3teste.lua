local serjao = require("serjao_berranteiro/serjao_berranteiro")

set_server.single_process = true

local function main(request)

    local param = request.param["ns"]

    local header = request.header[1]

    print("Teste do send_text", header.key)

    return serjao.send_text("O send_text está funcionando", 200)
end

serjao.initserver(3000, main)