#include "../src/dependencies/CWebStudio.h"
#include "CxpathJson.h"

CwebNamespace cb;
CxpathJsonNamespace xpath;
CxpathJsonErrorNamespace errors;

CwebHttpResponse *main_server(CwebHttpRequest *request) {

  cJSON *custom = cJSON_CreateObject();
  cJSON_AddStringToObject(custom, "a", "value of a");
  cJSON_AddStringToObject(custom, "b", "value of b");
  CwebHttpResponse *response = cb.response.send_cJSON(custom, 200);
  return response;
}

int main() {
  xpath = newCxpathJsonNamespace();
  errors = xpath.errors;
  cb = newCwebNamespace();

  struct CwebServer server = newCwebSever(3000, main_server);

  cb.server.start(&server);
  return 0;
}
