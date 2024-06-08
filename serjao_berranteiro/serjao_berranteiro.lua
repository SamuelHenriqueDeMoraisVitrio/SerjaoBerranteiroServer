

---@class SetServer
---@field single_process boolean
---@field allow_cors boolean
---@field use_folder boolean
---@field static_folder string
---@field function_timeout number
---@field client_timeout number
---@field max_queue number
---@field max_request number
---@field nullterminator string

---@class param
---@field key string
---@field value string

---@class header
---@field key string
---@field value string

---@class request
---@field url string
---@field method string
---@field route string
---@field params table<number, param>|table<string, string>
---@field header table<number, header>|table<string, string>
---@field read_json_body fun(max_size:number):table
---@field read_body fun(max_size:number):string

---@class serjaoResponse


---@class serjaoBerranteiro
---@field initserver fun(initport:number, lastport:number, main_server:fun(request:request))
---@field initdesktop fun(initport:number, lastport:number, server:string, main_server:fun(request:request))
---@field send_text fun(text:string, status_code:number):serjaoResponse
---@field send_file fun(route_file:string, status_code:number, content_type:string|nil):serjaoResponse
---@field send_json fun(data:any, status_code:number):serjaoResponse
---@field send_raw fun(content:any, content_type:string, status_code:number):serjaoResponse
---@field send_json_string fun(string:string, status_code:number):serjaoResponse
---@field send_html fun(html_code:string, status_code:number):serjaoResponse
---@field kill fun()



local info = debug.getinfo(1, "S")
local path = info.source:match("@(.*/)") or ""


local lib_path = ''

if os.getenv("HOME") then
    lib_path = path .. "serjao_berranteiro.so"
else
    perror("undefined os")
end

local load_lua = package.loadlib(lib_path, "serjao_berranteiro_start_point")

---@type serjaoBerranteiro
local lib = load_lua()

---@type SetServer
set_server = set_server
return lib
