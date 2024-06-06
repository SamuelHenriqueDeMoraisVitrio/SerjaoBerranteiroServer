

void get_params_for_server_config(struct CwebServer *struct_server_TEMP) {
  struct_server_TEMP->single_process =
      lw.tables.get_bool_prop(set_server, "single_process");
  struct_server_TEMP->allow_cors =
      lw.tables.get_bool_prop(set_server, "allow_cors");
  struct_server_TEMP->use_static =
      lw.tables.get_bool_prop(set_server, "use_folder");
  struct_server_TEMP->static_folder =
      lw.tables.get_string_prop(set_server, "static_folder");
  struct_server_TEMP->function_timeout =
      lw.tables.get_long_prop(set_server, "function_timeout");
  struct_server_TEMP->client_timeout =
      lw.tables.get_long_prop(set_server, "client_timeout");
  struct_server_TEMP->max_queue =
      lw.tables.get_long_prop(set_server, "max_queue");
  struct_server_TEMP->max_requests =
      lw.tables.get_long_prop(set_server, "max_request");

}

void creat_table_for_config_server() {
  set_server = lw.globals.new_table(l, "set_server");
  lw.tables.set_bool_prop(set_server, "single_process", false);
  lw.tables.set_bool_prop(set_server, "allow_cors", true);
  lw.tables.set_bool_prop(set_server, "use_folder", true);
  lw.tables.set_string_prop(set_server, "static_folder", "static");
  lw.tables.set_long_prop(set_server, "function_timeout", 30);
  lw.tables.set_long_prop(set_server, "client_timeout", 5);
  lw.tables.set_long_prop(set_server, "max_queue", 100);
  lw.tables.set_long_prop(set_server, "max_request", 1000);
  lw.tables.set_string_prop(set_server,"nullterminator","NULL");
}
