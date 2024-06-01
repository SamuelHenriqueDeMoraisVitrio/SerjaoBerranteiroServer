
LuaCEmbedResponse *setHP_index(LuaCEmbed *args, CwebDict *hp, char *(*get_unified_param)(CwebHttpRequest *, const char *));

LuaCEmbedResponse *setHeaders(LuaCEmbedTable *self, LuaCEmbed *args);

LuaCEmbedResponse *setParams(LuaCEmbedTable *self, LuaCEmbed *args);

void create_request(LuaCEmbed *args);
