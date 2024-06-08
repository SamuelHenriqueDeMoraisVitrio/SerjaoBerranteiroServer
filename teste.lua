local serjao = require("serjao_berranteiro/serjao_berranteiro")


set_server.single_process = true


---@param request Request
local function teste(request)

      local html = html(
              head(),
              body(h1("hello Word"))
      )

      print(type(html))

      return serjao.send_html(html, 200)
end

serjao.initserver(3000,4000,teste);