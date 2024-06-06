local serjao = require("serjao_berranteiro/serjao_berranteiro")

local component = serjao.component(
    "html",

    function ()
    	return "aaa"
    end,

    serjao.component("body",
            serjao.component(
                "h1",
                "aaaaaa"
            )

    )

)

print(component.render())