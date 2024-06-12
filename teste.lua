local serjao = require("serjao_berranteiro/serjao_berranteiro")


set_server.single_process = true

---@param request Request
local function teste(request)

  if "/kill" == request.route then
    serjao.kill()
  end

  local html = serjao.fragment("<!DOCTYPE html>",
          html(
                  head(
                          title("Hello Word"),
                          script()
                  ),
                  body(
                          h1("Hello Word"),
                          button("Press here"),
                          serjao.component("h2", "hello")
                  )
          )
  )

--  local html = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Kill Server Button</title><style>.custom-button{background-color:#ff0000;border:none;color:white;padding:15px 32px;text-align:center;text-decoration:none;display:inline-block;font-size:16px;margin:4px 2px;cursor:pointer;border-radius:8px;}</style></head><body><button class=\"custom-button\" id=\"killButton\">Kill Server</button><script>document.getElementById('killButton').addEventListener('click',function(){fetch('/kill',{method:'GET'});});</script></body></html>"

  print(html.render())
end

serjao.desktop("chromium", teste, nil, 1000)