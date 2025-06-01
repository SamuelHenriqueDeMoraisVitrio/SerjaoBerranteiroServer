

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Release: V6.0](https://img.shields.io/badge/Release-V6.0-blue.svg)](https://github.com/SamuelHenriqueDeMoraisVitrio/SerjaoBerranteiroServer/releases)
[![Language: Lua](https://img.shields.io/badge/Language-Lua-blue.svg)](https://www.lua.org/)
[![Platform: Linux/macOS/Windows](https://img.shields.io/badge/Platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey.svg)](https://github.com/SamuelHenriqueDeMoraisVitrio/SerjaoBerranteiroServer)
[![Status: Active](https://img.shields.io/badge/Status-Active-brightgreen.svg)](https://github.com/SamuelHenriqueDeMoraisVitrio/SerjaoBerranteiroServer)


# Serjão Berranteiro Server

![Logo by SerjaoBerranteiroServer](/extra/logo.png)

## Overview

Serjão Berranteiro Server is a powerful Lua framework for building both APIs and desktop applications with an emphasis on simplicity and flexibility. The framework provides robust features including server configuration options, process management, HTML generation, JSON handling, and more.

## Installation

### Method 1: Download from GitHub

1. Download the latest zip file from [Releases](https://github.com/SamuelHenriqueDeMoraisVitrio/SerjaoBerranteiroServer/releases)
2. Extract the file in your project directory
3. Require the library in your code:

```lua
local serjao = require("path_to_serjao_berranteiro/serjao_berranteiro")
```

### Method 2: Terminal Installation

In your project directory, run:

```bash
curl -L -o serjao_berranteiro.zip https://github.com/SamuelHenriqueDeMoraisVitrio/SerjaoBerranteiroServer/releases/download/V6/serjao_berranteiro.zip && unzip serjao_berranteiro.zip && rm serjao_berranteiro.zip 
```
### Method 3: Build from Source
1. Install [Darwin](https://github.com/OUIsolutions/Darwin) on version **0.3.0** or higher.
```bash
curl -L https://github.com/OUIsolutions/Darwin/releases/download/0.3.0/darwin.out -o darwin.out && sudo chmod +x darwin.out &&  sudo  mv darwin.out /usr/bin/darwin
```
2. Clone the repository:
```bash
git clone https://github.com/SamuelHenriqueDeMoraisVitrio/SerjaoBerranteiroServer.git
cd SerjaoBerranteiroServer
```
3. Build the library:
```bash
darwin run_blueprint --mode folder build/
```

## Core Features

### 1. Server Mode

Create HTTP servers for API development with automatic port selection and flexible configuration.

```lua
local serjao = require("serjao_berranteiro/serjao_berranteiro")

-- Define your request handler
local function api_handler(request)
  -- Process the request here
  return serjao.send_text("API is running", 200)
end

-- Start server with port range (will try ports 3000-5000 until one works)
serjao.server(3000, 5000, api_handler)

-- Or start with a specific port only
serjao.server(3000, api_handler)
```

### 2. Desktop Mode

Launch desktop applications using a browser engine with customizable window dimensions.

```lua
local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function app_handler(request)
  return serjao.html(
    serjao.head(
      serjao.title("My Desktop App")
    ),
    serjao.body(
      serjao.h1("Welcome to My Desktop App"),
      serjao.p("This is running in desktop mode!")
    )
  )
end

-- Start desktop app with specific dimensions (800x600)
serjao.desktop("chromium", app_handler, 800, 600)

-- Full screen
serjao.desktop("chromium", app_handler)

-- Custom height with default width
serjao.desktop("chromium", app_handler, nil, 600)
```

### 3. Request Handling

The framework provides comprehensive request object properties:

```lua
local serjao = require("serjao_berranteiro/serjao_berranteiro")

---@param request Request
local function handler(request)
  -- Basic request properties
  local url = request.url      -- Full URL of the request
  local method = request.method -- HTTP method (GET, POST, etc.)
  local route = request.route   -- Route path
  
  -- Headers access
  local content_type = request.header["Content-Type"]
  local all_headers = request.header[1] -- Access first header as a table
  local header_count = request.header.size
  
  -- URL parameters
  local user_id = request.params["user_id"]
  local all_params = request.params[1]
  local params_count = request.params.size
  
  -- Request body handling
  if method == "POST" or method == "PUT" then
    -- Parse JSON body (with 10KB limit)
    local json_data = request.read_json_body(10240)
    
    -- Or read raw body (with 1MB limit)
    local raw_data = request.read_body(1048576)
  end
  
  return {
    received_url = url,
    received_method = method,
    received_route = route
  }
end

serjao.server(3000, 5000, handler)
```

### 4. Response Types

Multiple response formats supported:

#### Text Response

```lua
local function handler(request)
  -- Method 1: Using send_text
  return serjao.send_text("Plain text response", 200)
  
  -- Method 2: Direct string return (status code 200 by default)
  return "Plain text response"
  
  -- Method 3: String with custom status code
  return "Plain text response", 201
end
```

#### JSON Responses

```lua
local function handler(request)
  local data = {
    id = 1,
    name = "Example",
    active = true,
    items = {
      "item1",
      "item2"
    }
  }
  
  -- Method 1: Using send_json
  return serjao.send_json(data, 200)
  
  -- Method 2: Direct table return (status code 200 by default)
  return data
  
  -- Method 3: Table with custom status code
  return data, 201
  
  -- Method 4: JSON string
  local json_str = '{"id":1,"name":"Example"}'
  return serjao.send_json_string(json_str, 200)
end
```

#### HTML Responses

```lua
local function handler(request)
  -- Method 1: Raw HTML string
  return serjao.send_html("<html><body><h1>Hello World</h1></body></html>", 200)
  
  -- Method 2: Using HTML builder API
  local html_content = serjao.html(
    serjao.head(
      serjao.title("Page Title")
    ),
    serjao.body(
      serjao.h1("Hello World"),
      serjao.p("This is a paragraph")
    )
  )
  
  return serjao.send_html(html_content, 200)
  
  -- Method 3: Direct HTML builder return
  return serjao.html(
    serjao.body(
      serjao.h1("Hello World")
    )
  ), 200
end
```

#### File Responses

```lua
local function handler(request)
  -- Send an image file
  return serjao.send_file("images/logo.png", 200, "image/png")
  
  -- Send a PDF document
  return serjao.send_file("documents/report.pdf", 200, "application/pdf")
end
```

#### Raw Binary Responses

```lua
local function handler(request)
  local binary_data = io.open("archive.zip", "rb"):read("*all")
  return serjao.send_raw(binary_data, "application/zip", 200)
end
```

### 5. Server Configuration

Customize server behavior with the `set_server` configuration object:

```lua
local serjao = require("serjao_berranteiro/serjao_berranteiro")

-- Configure null handling in JSON
set_server.nullterminator = "NULL"

-- Use single process mode (required for kill function)
set_server.single_process = true

-- Configure CORS
set_server.allow_cors = true

-- Static file serving
set_server.use_folder = true
set_server.static_folder = "public"

-- Timeouts and limits
set_server.function_timeout = 30  -- Function execution timeout in seconds
set_server.client_timeout = 5     -- Client connection timeout
set_server.max_queue = 100        -- Maximum request queue size
set_server.max_request = 1000     -- Maximum request size in bytes

-- Define your handler function
local function handler(request)
  return "Server configured successfully"
end

-- Start the server
serjao.server(3000, 5000, handler)
```

### 6. HTML Engine

The framework includes a powerful HTML builder API for creating complex HTML structures:

```lua
local serjao = require("serjao_berranteiro/serjao_berranteiro")

local function create_user_card(user)
  return serjao.div(
    { class = "user-card", id = "user-" .. user.id },
    serjao.h2(user.name),
    serjao.p(user.description),
    serjao.button(
      {
        class = "contact-btn",
        ["data-id"] = user.id,
        onclick = "contactUser(" .. user.id .. ")"
      },
      "Contact " .. user.name
    )
  )
end

local function handler(request)
  local users = {
    { id = 1, name = "John Doe", description = "Developer" },
    { id = 2, name = "Jane Smith", description = "Designer" }
  }
  
  local page = serjao.html(
    serjao.head(
      serjao.title("User Directory"),
      serjao.style("body { font-family: Arial; max-width: 800px; margin: 0 auto; }"),
      serjao.script({ src = "/js/main.js" })
    ),
    serjao.body(
      serjao.header(
        serjao.h1("User Directory")
      ),
      serjao.main(
        serjao.div(
          { class = "user-list" },
          create_user_card(users[1]),
          create_user_card(users[2])
        )
      ),
      serjao.footer(
        serjao.p("© 2025 User Directory")
      )
    )
  )
  
  return page
end

serjao.desktop("chromium", handler, 800, 600)
```

#### Adding Raw HTML

For adding HTML fragments directly, use the `fragment` function:

```lua
local header = serjao.fragment("<!DOCTYPE html>",
  "<meta charset='utf-8'>",
  "<meta name='viewport' content='width=device-width, initial-scale=1'>"
)

local html = serjao.html(
  serjao.head(
    header,
    serjao.title("My Page")
  ),
  serjao.body(
    serjao.h1("Hello World")
  )
)
```

#### Custom Components

Create custom components with the `component` function:

```lua
local custom_element = serjao.component("custom-element", 
  { class = "my-class" },
  "Content inside custom element"
)

local html = serjao.html(
  serjao.body(
    custom_element
  )
)
```

### 7. Process Management

The framework allows for both single-process and multi-process server modes:

```lua
local serjao = require("serjao_berranteiro/serjao_berranteiro")

-- Enable single process mode
set_server.single_process = true

-- Create a kill switch endpoint
local function handler(request)
  if request.route == "/kill" then
    serjao.kill()  -- Terminates the server
    return "Server shutting down"
  end
  
  return serjao.html(
    serjao.body(
      serjao.h1("Server Control Panel"),
      serjao.button(
        { onclick = "fetch('/kill')" },
        "Shutdown Server"
      )
    )
  )
end

serjao.server(3000, handler)
```

### 8. Interactive Application Example

Here's a complete example of an interactive counter application using HTMX:

```lua
local serjao = require("serjao_berranteiro/serjao_berranteiro")

local counter = 0

---@param request Request
local function app_handler(request)
  -- Handle increment endpoint
  if request.route == "/increment" then
    counter = counter + 1
    return serjao.send_html(serjao.span({ id = "counter" }, tostring(counter)))
  end
  
  -- Main page with HTMX integration
  return serjao.fragment("<!DOCTYPE html>",
    serjao.html(
      serjao.head(
        serjao.title("Counter App"),
        serjao.script({ src = "https://unpkg.com/htmx.org@1.9.12" })
      ),
      serjao.body(
        serjao.div(
          { class = "container", style = "text-align: center; margin-top: 50px;" },
          serjao.h1("HTMX Counter Example"),
          serjao.div(
            serjao.p("Current count: "),
            serjao.span({ id = "counter" }, tostring(counter))
          ),
          serjao.button(
            {
              class = "btn",
              style = "padding: 10px 20px; background-color: #4CAF50; color: white; border: none; cursor: pointer;",
              ["hx-post"] = "/increment",
              ["hx-target"] = "#counter",
              ["hx-swap"] = "innerHTML"
            },
            "Increment"
          )
        )
      )
    )
  )
end

serjao.desktop("chromium", app_handler, 800, 600)
```

## HTML Element Support

The framework provides built-in functions for all standard HTML elements:

| Common Elements | Form Elements | Text Elements | Container Elements | Media Elements |
|----------------|--------------|--------------|-------------------|----------------|
| `div` | `form` | `h1` to `h6` | `header` | `img` |
| `span` | `input` | `p` | `footer` | `audio` |
| `button` | `textarea` | `strong` | `nav` | `video` |
| `a` | `select` | `em` | `section` | `canvas` |
| `ul`, `ol`, `li` | `option` | `code` | `article` | `picture` |
| `table`, `tr`, `td` | `label` | `pre` | `aside` | `iframe` |
| `script` | `fieldset` | `blockquote` | `main` | `object` |
| `style` | `legend` | `q` | `details` | - |

## Additional Resources

- View examples in the `/examples` directory
- Read API documentation in the [wiki](https://github.com/SamuelHenriqueDeMoraisVitrio/SerjaoBerranteiroServer/wiki)
- Report issues on [GitHub Issues](https://github.com/SamuelHenriqueDeMoraisVitrio/SerjaoBerranteiroServer/issues)


## License

See the [LICENSE](LICENSE) file for license rights and limitations.

