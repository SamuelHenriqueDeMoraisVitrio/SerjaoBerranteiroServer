local serjao = require("serjao_berranteiro/serjao_berranteiro")

---@param request Request
local function main(request)

  if request.route == "/enviarmsg" then

    local user = request.header["usuario"]
    local senha = request.header["senha"]

    if user == nil then
      return "user not defined", 404
    end

    if user ~= "samuel" then
      return "User invalid", 404
    end

    if senha == nil then
      return "senha not defined", 404
    end

    if senha ~= "1234" then
      return "senha invalid", 405
    end

    io.open("teste.txt", "w"):write("Usuario conseguiu")

    return "Escrita realizada"

  end

  return "Rota invalid", 404
end

serjao.server(3001, main)