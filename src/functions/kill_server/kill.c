
#include "../uniq.definitions_requirements.h"

LuaCEmbedResponse *kill_server(LuaCEmbed *args) {

  cweb_kill_single_process_server();

  return NULL;
}
