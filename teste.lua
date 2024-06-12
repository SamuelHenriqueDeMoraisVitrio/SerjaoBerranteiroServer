local serjao = require("serjao_berranteiro/serjao_berranteiro")


local num = 0
set_server.single_process = true;

---@param request Request
local function teste(request)
    if request.route == "/kill" then
    	serjao.kill()
    end

   if request.route == "/increment" then
        num = num +1
   	    return serjao.send_html(h1("o valor do numero é ",tostring(num),{id="num"}))

   end
  local html = serjao.fragment("<!DOCTYPE html>",

          html(
                  head(
                          title("Hello Word"),
                          script({src="https://unpkg.com/htmx.org@1.9.12"})
                  ),

                  body(
                          h1("o valor do numero é ",tostring(num),{id="num"}),
                          "<br>",
                          button("increment",{
                                ["hx-trigger"]="click",
                                ["hx-post"]="/increment",
                                ["hx-target"]="#num",
                                ["hx-swap"]="innerHTML"
                          }),
                          "<br>",
                          button("kill",{
                                   style={color="white",["background-color"]="red"},
                                  ["hx-trigger"]="click",
                                  ["hx-post"]="/kill",
                                  ["hx-swap"]="None"
                          })
                  )
          )
  )

--  local html = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Kill Server Button</title><style>.custom-button{background-color:#ff0000;border:none;color:white;padding:15px 32px;text-align:center;text-decoration:none;display:inline-block;font-size:16px;margin:4px 2px;cursor:pointer;border-radius:8px;}</style></head><body><button class=\"custom-button\" id=\"killButton\">Kill Server</button><script>document.getElementById('killButton').addEventListener('click',function(){fetch('/kill',{method:'GET'});});</script></body></html>"
    return serjao.send_html(html.render())
end

serjao.desktop("chromium-browser", teste, nil, 1000)