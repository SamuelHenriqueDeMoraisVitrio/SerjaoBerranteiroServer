local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function teste(request)

        local pagina  =html(
                body(
                        h2({id="teste",style={color="yellow"}},"aaa"),
                        h1({style={color="blue"}},"eai parsa")
                ),
                serjao.component("h4"),
                serjao.fragment("<area>")
        )
        return serjao.send_html(pagina,200)

end

serjao.initserver(3000,4000,teste)