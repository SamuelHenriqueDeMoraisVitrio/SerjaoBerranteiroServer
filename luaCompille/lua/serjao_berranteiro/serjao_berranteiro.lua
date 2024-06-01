---@class Request
---@field url string
---@field method string
---@field route string

---@class SerjaoResponse

---@class SerjaoBerranteiro
---@field initserver fun(port: number, main_server: fun(request: Request), singleprocesses: boolean|nil)
---@field send_file fun(file:string, status_code:number:content_type:str | nil) SerjaoResponse

local info = debug.getinfo(1, "S")
local path = info.source:match("@(.*/)") or ""


local lib_path = ''

if os.getenv("HOME") then
    lib_path = path.."serjao_berranteiro.so"
else
    perror("undefined os")
end

local load_lua = package.loadlib(lib_path, "serjao_berranteiro_start_point")

---@type SerjaoBerranteiro
local lib = load_lua()
return lib