local json = require("luaFluidJson/luaFluidJson")

local user  ={

    name='Nateus',
    age=27,
    maried=true,
    sons={
        {name='son1',maried=false }
    }
}
local ident = true
local value = json.dumps_to_string(user,ident);

print(value)
