local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function teste()

        local pagina  =html(
       
                body(
                  h2({id="teste",style={color="red"}},"aaa"),      
                  h1({style={color="blue"}},"eai parsa")
                )
        )
        return serjao.send_html(pagina,200)

end

serjao.initserver(3000,3001,teste)