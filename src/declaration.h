#ifndef SERJAO_BERRANTEIRO_DECLARATION__H
#define SERJAO_BERRANTEIRO_DECLARATION__H

#include "dependencies/all.h"
#include "types/all.h"

CwebHttpResponse *main_sever(CwebHttpRequest *request);
int serjao_berranteiro_start_point(lua_State *state);
LuaCEmbedResponse *initserver(LuaCEmbed *arg);

#include "functions/declarations.h"

#endif
