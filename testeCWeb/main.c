#include "../src/dependencies/CWebStudio.h"

CwebNamespace cb;

CwebHttpResponse *main_sever(CwebHttpRequest *request ){


    if(strcmp(request->route, "/html") == 0){

    }

    if(strcmp(request->route,"/kill") == 0){
        cweb_kill_single_process_server();
    }
    return cb.response.send_text("Working", 200);

}

int main(int argc, char *argv[]){
    cb = newCwebNamespace();
    CwebServer server = newCwebSever(3001, main_sever);
    server.single_process = true;
    server.use_static = true;
    server.static_folder = "front";
    cb.server.start(&server);
    return 0;
}