#ifndef __MAIN_H__
#define __MAIN_H__

#include "dependencies/CWebStudio.h"
#include "dependencies/LuaCEmbed.h"

#ifndef cJSON__h
#include "dependencies/JSON/cJSON.c"
#include "dependencies/JSON/cJSON.h"
#endif

CwebNamespace cb;
LuaCEmbedNamespace lw;
LuaCEmbed *l;
CwebHttpRequest *cbrq;
LuaCEmbedTable *set_server;
#include "configuring_the_server/set_server.h"
#include "request/request.h"
#include "response/cJSON_response/json_response.h"
#include "response/response.h"

#include "configuring_the_server/set_server.c"
#include "request/request.c"
#include "response/cJSON_response/json_response.c"
#include "response/response.c"

int serjao_berranteiro_start_point(lua_State *state);
CwebHttpResponse *main_sever(CwebHttpRequest *request);
LuaCEmbedResponse *initserver(LuaCEmbed *arg);

#endif
