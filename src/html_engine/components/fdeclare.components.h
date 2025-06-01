//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.globals.h"
//silver_chain_scope_end

bool its_a_component(LuaCEmbedTable *element);

TextOrError render_component_raw(LuaCEmbedTable *self);

LuaCEmbedResponse *render_component(LuaCEmbedTable *self, LuaCEmbed *args);

LuaCEmbedTable *raw_create_fragment(LuaCEmbed *args);

LuaCEmbedResponse *create_fragment(LuaCEmbed *args);

LuaCEmbedResponse *create_component(LuaCEmbed *args);
