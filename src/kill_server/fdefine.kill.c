//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end


LuaCEmbedResponse *kill_server(LuaCEmbed *args) {

  cweb_kill_single_process_server();

  return NULL;
}
