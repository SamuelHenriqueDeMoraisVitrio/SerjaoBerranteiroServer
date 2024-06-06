local serjao = require("serjao_berranteiro/serjao_berranteiro")

local component =html(
    head(
            title("Não sei")
    ),

    body(
        h1("dddd"),
        serjao.component("h3","aaa"),
        serjao.component("h2", "aaaaaa")
    )




)

print(component.render())