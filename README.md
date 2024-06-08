# Serjão Berranteiro Server

## Languages

[Português]()

## A framework to Develop APIs and Desktop Apps

It is a library for the Lua language aiming to be easy and intuitive, but without losing the freedom to set up your server the way you prefer.

Bringing many functionalities such as the choice of a single process or multiple processes, an HTML engine, working with JSON, among others.

## Notices

- This is still the beta version, so it may contain several "bugs".
- To make it easier to use, put this line "---@param request Request" in your code before the server loop function.
```
local serjao = require("serjao_berranteiro/serjao_berranteiro")


set_server.single_process = true


---@param request Request
local function teste(request)
      return serjao.send_text("aaa",200)
end

serjao.initserver(3000,4000,teste);
```

***

## Versions

- It is recommended to always use the latest published version.

***

## How to use the lib?

### The simplest way to use the lib in your code is:

Go to the repository [SerjaoBerranteiroServer](https://github.com/SamuelHenriqueDeMoraisVitrio/SerjaoBerranteiroServer) and download the latest .zip file posted in [Realeses/Lançamentos](https://github.com/SamuelHenriqueDeMoraisVitrio/SerjaoBerranteiroServer/releases).

Place the unzipped file inside the directory where your .lua file is located. After that, finish by calling a requirement within your Lua code.

```
local serjao = require("Path to serjao_berranteiro directory/serjao_berranteiro")
```

Now you can use all the functionalities of the SerjaoBerranteiroServer.

***

## Starting a server

### The initserver function allows you to start your server

#### Parameters:
| Port number | Functionality |
|-------------|---------------|
| Parameter 1 | Starting port |
| Parameter 2 | Ending port   |
| Parameter 3 | Callback      |

##### Ports
Before starting the process, an automatic check of the starting port is performed, and if it does not work, the lib will increment a number and test again until it works or until the port is greater than the ending port.

##### Callback
The third parameter will call the function you created that will handle your server.

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function whatever_name()

  return serjao.send_text("Hello Word", 200)

end

serjao.initserver(3000, 5000, whatever_name)
```

***

## Starting a 'desktop'

### The initdesktop function allows you to start the appServer

#### Parameters:

| Port number	 | Functionality |
|--------------|---------------|
| Parameter 1  | Starting port |
| Parameter 2  | Ending port   |
| Parameter 3  | Browser       |
| Parameter 3  | Callback      |

##### Ports:

The first two parameters work the same way as in initserver.

##### Navegador:

This parameter requires a string of your preferred browser.
PS: Recommended to install chromium-browser

###### APT
```
sudo apt install chromium-browser
```
###### DNF
```
sudo dnf install chromium-browser
```

##### Calback:

This parameter works the same way as in initserver.

```
local srj = require("serjao_berranteiro/serjao_berranteiro")

local function whatever_name()

  return "Hello Word"

end

srj.initdesktop(3000, 3000, "chromium", whatever_name)
```

***

## request

In the function you created, you can add a parameter that will handle the requests in your code.

The requests are:

| requests       | return           |
|----------------|------------------|
| URL            | string           |
| METHOD         | string           |
| ROUTE          | string           |
| PARAMS         | values or table  |
| HEADER         | number or string |
| read_json_body | table json       |
| read_body      | file/binary      |

### URL, METHOD, ROUTE
These three requests are simple:
```
local sla = require("serjao_berranteiro/serjao_berranteiro")

local function main_server(request)

  local url  = request.url
  local method = request.method
  local route = request.route

  print("url: ", url, "\nmethod: ", method, "\nroute: ", route, "\n\n")

  return "Hello Word"

end

sla.initdesktop(3000, 3000, "chromium", nome_Que_quiser)
```

### PARAMS, HEADERS

These requests can be used in two ways:
The first is by key, where you can search for a header or param by the key name.
The second way is by index, returning a table that can access the key and value.
PS:. Remember that if you work with an index, Lua starts the index at number 1.
```
local srj = require("serjao_berranteiro/serjao_berranteiro")

local function main_server(request)

  local tabela_do_header = request.header[1]
  local value_do_param = request.params["nsei"]

  print("\n\tHeader:\n\tkey: ", tabela_do_header.key, "\n\tvalue: ", tabela_do_header.value)

  print("\n\tvalue de param: ", value_do_param)

  return "Hello Word"

end

srj.initserver(3000, 3000, main_server)
```

### read_json_body

The read_json_body function reads the body and returns a table of this JSON. This function has only one parameter, which is the maximum size in bytes for security.
The JSON I will use as an example is this:

```
{
	"json": "jsonV4.0",
	"name": "samuel",
	"name2": "henrique"
}
```

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function main_server(request)

  local json = request.read_json_body(10000)

  print(json.name)

  return serjao.send_json(json, 200)

end

serjao.initserver(3000, 3000, main_server)
```

### read_body

The read_body function also reads the body.

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function main_server(request)

  local file = request.read_body(1000000)

  io.open("teste.png", "w"):write(file)

  return "Deu certo"

end

serjao.initserver(3000, 3003, main_server)
```

***

## Response

The responses are functions that return a message to the body of your server.

### responses:

| Name             | Return to the body       |
|------------------|--------------------------|
| send_text        | text                     |
| send_file        | file                     |
| send_json        | json                     |
| send_raw         | anyway                   |
| send_json_string | json                     |
| send_html        | html                     |
| ""               | text without status code |

### send_text

This function is used to send a message with the status code of this return.

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function main_server(request)


  return serjao.send_text("Hello Word", 200)

end

serjao.initserver(3000, 3003, main_server)
```

### send_json_string

This function is used to send a JSON to the body via a string.

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function main_server(request)


  local json = "{\"name\" = \"serjão\",\"idade\" = 20}"

  return serjao.send_json_string(json, 200)

end

serjao.initserver(3000, 3003, main_server)
```

### send_html

This function is used to send HTML to the body via a string.

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function main_server(request)


  local html = "<html><body><h1>Hello</h1></body></html>"

  return serjao.send_html(html, 200)

end

serjao.initserver(3000, 3003, main_server)
```

### send_file

This function is used to send a file to the body.
The first parameter is the file path, and the third parameter is optional but recommended, which is the content type of your file.

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function main_server(request)
  return serjao.send_file("Captura de tela de 2024-06-01 14-38-38.png", 200, "image/png")
end

serjao.initserver(3000, 3003, main_server)
```


### send_json

This function sends a JSON to the body by receiving a JSON table.

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function main_server(request)
  local json = {
    nome = "samuel",
    sla = "NULL",
    idade = 20
  }

  return serjao.send_json(json, 200)
end

serjao.initserver(3000, 3003, main_server)
```

This function also accepts strings, booleans, and numbers.

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function main_server(request)

  return serjao.send_json("Hello Word", 200)
end

serjao.initserver(3000, 3003, main_server)
```

### send_raw

This function sends anything, like a PNG, binary, or zip.
The second parameter is the content type.

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

local sla = io.open("Captura de tela de 2024-06-01 14-38-38.png", "rb")
local function main_server(request)

  return serjao.send_raw(sla, "image/png", 200)
end

serjao.initserver(3000, 3003, main_server)
```

***

## set_server

This set_server is used to make pre-configurations on your server.

### Todos set_servers

| name             | type   | default  |
|------------------|--------|----------|
| nullterminator   | string | "NULL"   |
| single_process   | bool   | false    |
| allow_cors       | bool   | true     |
| use_folder       | bool   | true     |
| static_folder    | string | "static" |
| function_timeout | number | 30       |
| client_timeout   | number | 5        |
| max_queue        | number | 100      |
| max_request      | number | 1000     |



### nullterminator

This config is used to set the null string of the JSON

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

set_server.nullterminator = "nil"

local function main_server(request)
  local json = {
    nome = "samuel",
    sla = "nil",
    idade = 20
  }

  return serjao.send_json(json, 200)
end

serjao.initserver(3000, 3003, main_server)
```

### single_process

This config determines whether the server will be single or multiprocess.

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

set_server.single_process = true

local function main_server(request)
  return "sla sla sla"
end

serjao.initserver(3000, 3003, main_server)
```

### use_folder

Sets a default folder.

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

set_server.use_folder = false

local function main_server(request)
  return "sla sla sla"
end

serjao.initserver(3000, 3003, main_server)
```

### static_folder

Sets the name of the default folder.

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

set_server.use_folder = true
set_server.static_folder = "sla"

local function main_server(request)
  return "sla sla sla"
end

serjao.initserver(3000, 3003, main_server)
```

***

## HTML engine

There is an engine to work with HTMLs within the code itself.
To add an unrecognized command by the lib, use the component and if you want to manually create the command or use a command that ends with '</...>' use fragment

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function teste()

        local pagina  =html(
                body(
                  h2({id="teste",style={color="yellow"}},"aaa"),
                  h1({style={color="blue"}},"eai parsa")
                ),
                serjao.component("h4"),
                serjao.fragment("<area>")
        )
        return serjao.send_html(pagina,200)

end

serjao.initserver(3000,4000,teste)


```