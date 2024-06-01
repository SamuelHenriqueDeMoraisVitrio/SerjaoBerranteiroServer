#include "../src/dependencies/CWebStudio.h"

CwebNamespace cb;

CwebHttpResponse *main_sever(CwebHttpRequest *request ){
    unsigned char *read_content_cb = cb.request.read_content(request, 20000);
    CwebDict *query_paramns = request->params;
    for(int i = 0; i < query_paramns->size; i++){
        CwebKeyVal *key_val = query_paramns->keys_vals[i];
        char *key = key_val->key;
        char *value = key_val->value;
        printf("%s : %s\n", key, value);
    }
    printf("-----------------------------------------------\n");
    printf("\t%s\n\n", read_content_cb);
    return cb.response.send_text("Url readed", 200);

}

int main(int argc, char *argv[]){
    cb = newCwebNamespace();
    CwebServer server = newCwebSever(5000, main_sever);
    cb.server.start(&server);
    return 0;
}