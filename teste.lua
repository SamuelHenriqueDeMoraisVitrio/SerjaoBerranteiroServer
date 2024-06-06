local serjao = require("serjao_berranteiro/serjao_berranteiro")

local component = serjao.component(
    "html",
    serjao.component("body",
            serjao.props({a=20,b=30}),
            serjao.fragment("aaa","bbbb","ccc")
    )



)

print(component.render())