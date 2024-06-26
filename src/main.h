#ifndef __MAIN_H__
#define __MAIN_H__

#include "dependencies/CWebStudio.h"
#include "dependencies/LuaCEmbed.h"

CwebHttpResponse *main_sever(CwebHttpRequest *request);
int serjao_berranteiro_start_point(lua_State *state);

CwebNamespace cb;
LuaCEmbedNamespace lw;
LuaCEmbed *l;
CwebHttpRequest *cbrq;
LuaCEmbedTable *set_server;

#include "init/declaration.h"
#include "configuring_the_server/set_server.h"
#include "request/declaration.h"
#include "response/declaration.h"
#include "html_engine/declaration.h"
#include "kill_server/kill.h"
#include "callbacks/callback.h"
//******
#include "init/definitions.h"
#include "configuring_the_server/set_server.c"
#include "request/definitions.h"
#include "response/definition.h"
#include "html_engine/definition.h"
#include "kill_server/kill.c"
#include "callbacks/callback.c"

int serjao_berranteiro_start_point(lua_State *state);
CwebHttpResponse *main_sever(CwebHttpRequest *request);
LuaCEmbedResponse *initserver(LuaCEmbed *arg);

#endif
