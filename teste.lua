local serjao = require("serjao_berranteiro/serjao_berranteiro")


local num = 0
set_server.single_process = true;

---@param request Request
local function teste(request)



    for i=1,request.header.size do
        local current = request.header[i]
--        print(current.key," = ",current.value)
    end

    return serjao.send_json()
end

serjao.server(3000,4000,teste)