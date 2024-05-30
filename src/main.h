#ifndef __MAIN_H__
#define __MAIN_H__

#include "dependencies/CWebStudio.h"
#include "dependencies/LuaCEmbed.h"

CwebNamespace cb;
LuaCEmbedNamespace lw;
LuaCEmbed *l;
CwebHttpRequest *cbrq;
LuaCEmbedTable *set_server;
#include "configuring_the_server/set_server.h"
#include "request/request.h"
#include "response/response.h"

#include "configuring_the_server/set_server.c"
#include "request/request.c"
#include "response/response.c"

int serjao_berranteiro_start_point(lua_State *state);
CwebHttpResponse *main_sever(CwebHttpRequest *request);
LuaCEmbedResponse *initserver(LuaCEmbed *arg);

#endif
