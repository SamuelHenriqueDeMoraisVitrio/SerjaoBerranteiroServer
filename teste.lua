local serjao = require("serjao_berranteiro/serjao_berranteiro")

local component =html(

    body(
        h1("dddd"),
        serjao.component("h3","aaa")
    )




)

print(component.render())