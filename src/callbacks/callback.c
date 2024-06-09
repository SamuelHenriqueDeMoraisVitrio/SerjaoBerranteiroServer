
void calbback_main(){
    lw.add_callback(l, "server", initserver);
    lw.add_callback(l, "desktop", initdesktop);
    lw.add_callback(l, "send_file", send_file);
    lw.add_callback(l, "send_html", send_HTML);
    lw.add_callback(l, "send_json_string", send_json_string);
    lw.add_callback(l, "send_json", send_json);
    lw.add_callback(l , "send_raw", send_raw);
    lw.add_callback(l, "send_text", send_text);
    lw.add_callback(l,"component",create_component);
    lw.add_callback(l,"fragment",create_fragment);
    lw.add_callback(l, "kill", kill_server);
}