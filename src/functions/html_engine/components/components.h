#include "../uniq.declarations_requirement.h"

bool its_a_component(LuaCEmbedTable *element);

TextOrError render_component_raw(LuaCEmbedTable *self);

LuaCEmbedResponse *render_component(LuaCEmbedTable *self, LuaCEmbed *args);

LuaCEmbedTable *raw_create_fragment(LuaCEmbed *args);

LuaCEmbedResponse *create_fragment(LuaCEmbed *args);

LuaCEmbedResponse *create_component(LuaCEmbed *args);
