---@class set_server
---@field single_process boolean
---@field allow_cors boolean
---@field use_folder boolean
---@field static_folder string
---@field function_timeout number
---@field client_timeout number
---@field max_queue number
---@field max_request number

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
---@field params table<number, param> | table<string, string>
---@field header table<number, header> | table<string, string>
---@field read_json_body fun(max_size: number) : table
---@field read_body fun(max_size: number) : file

---@class serjaoResponse

---@class serjaoBerranteiro
---@field initserver fun(port:number, main_server:fun(request: request))
---@field send_file fun(route_file: string, status_code: number, content_type: string|nil) : serjaoResponse
---@field send_json fun(data: table, status_code: number): serjaoResponse
---@field send_raw fun(content: any, content_type: string, status_code: number) : serjaoResponse

function strcmp(str1, str2)
    local len1 = #str1
    local len2 = #str2
    local minLen = math.min(len1, len2)

    for i = 1, minLen do
        local byte1 = string.byte(str1, i)
        local byte2 = string.byte(str2, i)

        if byte1 < byte2 then
            return -1
        elseif byte1 > byte2 then
            return 1
        end
    end

    if len1 < len2 then
        return -1
    elseif len1 > len2 then
        return 1
    else
        return 0
    end
end

local info = debug.getinfo(1, "S")
local path = info.source:match("@(.*/)") or ""


local lib_path = ''

if os.getenv("HOME") then
    lib_path = path .. "serjao_berranteiro.so"
else
    perror("undefined os")
end

local load_lua = package.loadlib(lib_path, "serjao_berranteiro_start_point")

---@type SerjaoBerranteiro
local lib = load_lua()
return lib
