local serjao = require("serjao_berranteiro/serjao_berranteiro")


set_server.single_process = true

---@param request Request
local function teste(request)

      local h = request.header[1]

      local html = html(
              head(
                      title("youtube")
              ),
              body(h1("Hello Word")
              )
      )

      return serjao.send_html(html, 400)

end


serjao.desktop("chromium", teste)