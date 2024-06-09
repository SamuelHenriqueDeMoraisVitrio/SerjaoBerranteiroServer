local serjao = require("serjao_berranteiro/serjao_berranteiro")


set_server.single_process = true

---@param request Request
local function teste(request)

      local html = html(serjao.fragment("<!DOCTYPE html>"),
              head(serjao.fragment("<title>Hello Word</title>")),
              body(h1("hello Word"),
                head("Hello Word"),
                h2("Viz maria")
              )
      )

      print(type(html)) -- table
      print(html.render())

      return serjao.send_html(html, 200)
end

serjao.server(3000,4000,teste);