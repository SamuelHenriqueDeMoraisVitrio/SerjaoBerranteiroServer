//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.globals.h"
//silver_chain_scope_end


LuaCEmbedResponse *setHP_index(LuaCEmbed *args, CwebDict *hp,
                               char *(*get_unified_param)(CwebHttpRequest *,
                                                          const char *));

LuaCEmbedResponse *setHeaders(LuaCEmbedTable *self, LuaCEmbed *args);

LuaCEmbedResponse *setParams(LuaCEmbedTable *self, LuaCEmbed *args);

void create_request(LuaCEmbed *args);
