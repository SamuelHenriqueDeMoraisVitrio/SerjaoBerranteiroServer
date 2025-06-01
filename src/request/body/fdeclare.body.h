//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.globals.h"
//silver_chain_scope_end


LuaCEmbedTable *private_serjao_parse_array(LuaCEmbed *args,
                                              cJSON *json_array);
LuaCEmbedTable *Creat_table_from_json_object(LuaCEmbed *args,
                                             cJSON *json_object);
LuaCEmbedResponse *read_raw_body(LuaCEmbedTable *self, LuaCEmbed *args);
LuaCEmbedResponse *read_json_from_table(LuaCEmbedTable *self, LuaCEmbed *args);
