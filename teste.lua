local serjao = require("serjao_berranteiro/serjao_berranteiro")

local component = serjao.component(
    "html",
    serjao.component("body",
            serjao.component(
                "h1",
                "aaaaaa"
            )
    )
)

print(component.render())