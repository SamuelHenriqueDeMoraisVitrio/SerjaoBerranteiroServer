local serjao = require("serjao_berranteiro/serjao_berranteiro")

local component =html(
    head(
            serjao.fragment("meta"),
            title("Não sei")
    ),

    body(
        h1({id="teste",class="aaaaaa",style={color="blue"}},"dddd"),
        serjao.component("h3","aaa")
    )


)

print(component.render())