# Serjão Berranteiro Server
## Um framework para Desenvolver APIs e APPs Desktops

É uma biblioteca para a linguagem Lua visando ser facil e intuitiva, mas sem perder a liberdade de montar o seu servidor da maneira que achar melhor.

Trazendo muitas funcionalidades como a escolha de um processo ou varios processos, uma engine de html, trabalhar com json, entre outros.

## Avisos

- Essa é a versão beta ainda, então pode conter varios “bugs”.

***

## Versões

- É recomendado usar sempre a última versão publicada.

***

## Como usar a lib?

### A forma mais simples de usar a lib no seu codigo é:

Vai para o repositorio [SerjaoBerranteiroServer](https://github.com/SamuelHenriqueDeMoraisVitrio/SerjaoBerranteiroServer) e baixe o arquivo.zip mais recente postado em [Realeses/Lançamentos](https://github.com/SamuelHenriqueDeMoraisVitrio/SerjaoBerranteiroServer/releases).

Coloque o arquivo descompactado dentro do diretorio onde está seu arquivo.lua. Após isso finalize chamando um require dentro de seu codigo Lua.

```
local serjao = require("Caminho do diretorio serjao_berranteiro/serjao_berranteiro")
```

Pronto agora pode usar todas as funcionalidades do SerjaoBerranteiroServer.

***

## Iniciando um servidor

### A função initserver permite iniciar o seu servidor

#### Parametros:
| Numero porta | funcionalidade  |
|--------------|-----------------|
| parametro 1  | Porta de início |
| parametro 2  | Porta final     |
| parametro 3  | Callback        |

##### Portas
Antes de iniciar o processo, é feito de forma automatica uma verificação da porta inicial e se não estiver a funcionar, a lib vai incrementar um número e vai testar de novo até que funcione ou até a porta seja maior que a porta final.

##### Callback
O terceiro parametro vai chamar a função que você criou que vai ser trabalhado o seu servidor.

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function nome_Que_quiser()

  return serjao.send_text("Hello Word", 200)

end

serjao.initserver(3000, 5000, nome_Que_quiser)
```

***

## Iniciando um ‘desktop’

### A função initdesktop permite iniciar o appServer

#### Parametros:

| Numero porta | funcionalidade  |
|--------------|-----------------|
| parametro 1  | Porta de início |
| parametro 2  | Porta final     |
| parametro 3  | Navegador       |
| parametro 3  | Callback        |

##### Portas:

Os dois primeiro parametros funciona da mesma forma que o initserver.

##### Navegador:

Esse parametro você vai colocar uma “string” do seu navegador que preferir.  
PS:. Recomendado instalar o chromium-browser

###### APT
```
sudo apt install chromium-browser
```
###### DNF
```
sudo dnf install chromium-browser
```

##### Calback:

Esse parametro funciona da mesma forma que o initserver.

```
local srj = require("serjao_berranteiro/serjao_berranteiro")

local function nome_Que_quiser()

  return "Hello Word"

end

srj.initdesktop(3000, 3000, "chromium", nome_Que_quiser)
```

***

## requisição

Na função que você criou, pode ser colocado um parametro que será feito as requisições no seu codigo.

As requisições são:  

| Requisições     | retorno          |
|-----------------|------------------|
| URL             | string           |
| METHOD          | string           |
| ROUTE           | string           |
| PARAMS          | valor ou tabela  |
| HEADER          | numero ou string |
| read_json_body  | tabela json      |
| read_body       | arquivo/binaro   |

### URL, METHOD, ROUTE
Essas três requisições são simples:
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

Essas requisições têm duas formas de ser usadas:  
O primeiro é pela key, pode procurar um header ou param pelo nome da key.  
A segunda forma é pelo index tendo como retorno uma tabela podendo ascessar o key e o value.
PS:. Lembre que se for trabalhar com index, o Lua começa o index pelo número 1.
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

O read_json_body é uma função que vai ler o body e vai retornar uma tabela desse json. Essa função tem como parametro somente o tamanho maximo em bytes para segurança.  
O json que vou usar como exemplo é esse:  

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

O read_body é uma função para ler o body também.

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

## Resposta

As respostas são funções que retornam uma mensagem para body do seu servidor.

### responses:

| Nome             | retorno para o body   |
|------------------|-----------------------|
| send_text        | texto                 |
| send_file        | arquivo               |
| send_json        | json                  |
| send_raw         | qualquer coisa        |
| send_json_string | json                  |
| send_html        | html                  |
| ""               | texto sem status code |

### send_text

Essa função serve para você mandar uma mensagem com o status code desse retorno.

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function main_server(request)


  return serjao.send_text("Hello Word", 200)

end

serjao.initserver(3000, 3003, main_server)
```

### send_json_string

Essa função serve para você mandar um json para o body por meio de uma “string”.

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function main_server(request)


  local json = "{\"name\" = \"serjão\",\"idade\" = 20}"

  return serjao.send_json_string(json, 200)

end

serjao.initserver(3000, 3003, main_server)
```

### send_html

Essa função serve para você mandar um html para o body por meio de uma string.

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function main_server(request)


  local html = "<html><body><h1>Hello</h1></body></html>"

  return serjao.send_html(html, 200)

end

serjao.initserver(3000, 3003, main_server)
```

### send_file

Essa função serve para você mandar um arquivo para o body.
O primeiro parametro fica o caminho do arquivo e o terceiro parametro não é obrigatorio, mas é recomendado que é o content_type do seu arquivo.

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function main_server(request)
  return serjao.send_file("Captura de tela de 2024-06-01 14-38-38.png", 200, "image/png")
end

serjao.initserver(3000, 3003, main_server)
```


### send_json

Essa função manda um json para o body recebendo uma tabela de json.

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

Essa função além de tabelas tambem aceita strings, bools e numbers.

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function main_server(request)

  return serjao.send_json("Hello Word", 200)
end

serjao.initserver(3000, 3003, main_server)
```

### send_raw

Essa função manda qualquer coisa, como um png, binario, ou um zip.
O segundo parametro é o content_type.

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

Esse set_server é para fazer pré-configurações no seu servidor.

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

Essa config serve para configurar a string de null do json

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

Essa config determina se o servidor vai ser single ou multi process.

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

set_server.single_process = true

local function main_server(request)
  return "sla sla sla"
end

serjao.initserver(3000, 3003, main_server)
```

### use_folder

Configura um folder padrão.

```
local serjao = require("serjao_berranteiro/serjao_berranteiro")

set_server.use_folder = false

local function main_server(request)
  return "sla sla sla"
end

serjao.initserver(3000, 3003, main_server)
```

### static_folder

Configura o nome do folder padrão.

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

## Engine HTML

Tem uma engine para trabalhar com HTMLs dentro do proprio codigo.  
Para colocar algum comando não reconhecido pela lib, use o **component** e caso queira fazer na mão o comando ou queira usar um comando que termine com '</...>' use **fragment**

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


