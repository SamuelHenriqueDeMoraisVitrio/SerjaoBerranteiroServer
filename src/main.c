#include "main.h"

CwebHttpResponse *main_sever(CwebHttpRequest *request) {
  cbrq = request;
  create_request(l);
  lw.evaluate(l, "serverresponse = server_callback(request_main_server)");

  if (lw.has_errors(l)) {
    char *error = lw.get_error_message(l);
    printf("%s\n", error);
    return cb.response.send_text("Interno server error", 500);
  }

  int response_type = lw.globals.get_type(l, "serverresponse");

  if (response_type == lw.types.STRING) {
    char *value = lw.globals.get_string(l, "serverresponse");
    return cb.response.send_text(value, 200);
  }

  if (response_type == lw.types.TABLE) {
    LuaCEmbedTable *table = lw.globals.get_table(l, "serverresponse");

    CwebHttpResponse *response_cb =
        (CwebHttpResponse *)lw.tables.get_long_prop(table, "response_pointer");
    if (lw.has_errors(l)) {
      char *error = lw.get_error_message(l);
      printf("%s\n", error);
      return cb.response.send_text("Interno server error", 500);
    }

    return response_cb;
  }

  return NULL;
}

LuaCEmbedResponse *initserver(LuaCEmbed *arg) {
  unsigned short initport = (unsigned short)lw.args.get_long(arg, 0);
  const unsigned short lastport = (unsigned short)lw.args.get_long(arg, 1);
  unsigned short port = initport;

  if (lw.has_errors(arg)) {
    return lw.response.send_error("Uninformed arguments");
  }

  const char *functionvalue = "function(value) server_callback = value end";
  lw.args.generate_arg_clojure_evalation(arg, 2, functionvalue);

  if (lw.has_errors(arg)) {
    return lw.response.send_error("Uninformed arguments");
  }

  bool errorInit = true;
  for(unsigned short i = initport; i <= lastport; i++){
      struct CwebServer serverTEMP = newCwebSever(i, main_sever);
      serverTEMP.function_timeout = 100;
      get_params_for_server_config(&serverTEMP);

      errorInit = cb.server.start(&serverTEMP);
      if(errorInit){
          continue;
      }
      if(!errorInit){
          port = i;
          break;
      }
  }
  if(errorInit){
      return lw.response.send_error("Não foi possivel usar das portas %hd a %hd.", initport, lastport);
  }
  return NULL;
}


LuaCEmbedResponse *initdesktop(LuaCEmbed *arg) {

    unsigned short initport = (unsigned short)lw.args.get_long(arg, 0);
    unsigned short lastport = (unsigned short)lw.args.get_long(arg, 1);
    char * starter = lw.args.get_str(arg,2);


    if (lw.has_errors(arg)) {
    return lw.response.send_error("Uninformed arguments");
    }

    const char *functionvalue = "function(value) server_callback = value end";
    lw.args.generate_arg_clojure_evalation(arg, 3, functionvalue);


    if (lw.has_errors(arg)) {
    return lw.response.send_error("Uninformed arguments");
    }

    pid_t pid_server =0;
    int port = 0;
    for(int i = initport; i <= lastport; i++){
    pid_server = fork();
    if(pid_server==0){
      struct CwebServer serverTEMP = newCwebSever(i, main_sever);
      serverTEMP.function_timeout = 100;
      get_params_for_server_config(&serverTEMP);
      serverTEMP.single_process = true;
      serverTEMP.allow_cors = false;
      cb.server.start(&serverTEMP);
      exit(0);
    }

    sleep(1);
    int status;
    pid_t result = waitpid(pid_server, &status, WNOHANG);
    if (result == 0) {
      // Processo filho ainda está vivo
      port = i;
      break;
    }

    }
    printf("server rodando em %d\n",port);
    pid_t pid_browser = fork();
    if(pid_browser == 0){
    char comand[200] = {'\0'};
    sprintf(comand,"%s --app=http://localhost:%d/",starter,port);
    system(comand);
    exit(0);
    }

    while (true){
    int status;
    pid_t brwoser_result = waitpid(pid_browser, &status, WNOHANG);
    if(brwoser_result != 0){
      kill(pid_server, SIGKILL) ;
      kill(pid_browser,SIGKILL);
      break;
    }
    pid_t server_seult = waitpid(pid_server,&status,WNOHANG);
    if(server_seult != 0){
      kill(pid_server, SIGKILL) ;
      kill(pid_browser,SIGKILL);
      break;
    }
    }
    printf("application terminated\n");
    return NULL;
}

int serjao_berranteiro_start_point(lua_State *state) {
    cb = newCwebNamespace();
    lw = newLuaCEmbedNamespace();
    l = lw.newLuaLib(state);
    lw.add_callback(l, "initserver", initserver);
    lw.add_callback(l, "initdesktop", initdesktop);
    lw.add_callback(l, "send_file", send_file);
    lw.add_callback(l, "send_html", send_HTML);
    lw.add_callback(l, "send_json_string", send_json_string);
    lw.add_callback(l, "send_json", send_json);
    lw.add_callback(l , "send_raw", send_raw);
    lw.add_callback(l, "send_text", send_text);
    lw.add_callback(l,"component",create_component);
    lw.add_callback(l,"fragment",create_fragment);

    lw.add_global_callback(l,"html",create_html);
    lw.add_global_callback(l,"body",create_body);
    lw.add_global_callback(l, "head", create_head);
    lw.add_global_callback(l, "a", create_a);
    lw.add_global_callback(l, "abbr", create_abbr);
    lw.add_global_callback(l, "address", create_address);
    lw.add_global_callback(l, "article", create_article);
    lw.add_global_callback(l, "aside", create_aside);
    lw.add_global_callback(l, "audio", create_audio);
    lw.add_global_callback(l, "b", create_b);
    lw.add_global_callback(l, "bdi", create_bdi);
    lw.add_global_callback(l, "bdo", create_bdo);
    lw.add_global_callback(l, "blockquote", create_blockquote);
    lw.add_global_callback(l, "button", create_button);
    lw.add_global_callback(l, "canvas", create_canvas);
    lw.add_global_callback(l, "caption", create_caption);
    lw.add_global_callback(l, "cite", create_cite);
    lw.add_global_callback(l, "code", create_code);
    lw.add_global_callback(l, "colgroup", create_colgroup);
    lw.add_global_callback(l, "data", create_data);
    lw.add_global_callback(l, "datalist", create_datalist);
    lw.add_global_callback(l, "dd", create_dd);
    lw.add_global_callback(l, "del", create_del);
    lw.add_global_callback(l, "details", create_details);
    lw.add_global_callback(l, "dfn", create_dfn);
    lw.add_global_callback(l, "dialog", create_dialog);
    lw.add_global_callback(l, "div", create_div);
    lw.add_global_callback(l, "dl", create_dl);
    lw.add_global_callback(l, "dt", create_dt);
    lw.add_global_callback(l, "em", create_em);
    lw.add_global_callback(l, "fieldset", create_fieldset);
    lw.add_global_callback(l, "figcaption", create_figcaption);
    lw.add_global_callback(l, "figure", create_figure);
    lw.add_global_callback(l, "footer", create_footer);
    lw.add_global_callback(l, "form", create_form);
    lw.add_global_callback(l, "h1", create_h1);
    lw.add_global_callback(l, "h2", create_h2);
    lw.add_global_callback(l, "h3", create_h3);
    lw.add_global_callback(l, "h4", create_h4);
    lw.add_global_callback(l, "h5", create_h5);
    lw.add_global_callback(l, "h6", create_h6);
    lw.add_global_callback(l, "header", create_header);
    lw.add_global_callback(l, "i", create_i);
    lw.add_global_callback(l, "iframe", create_iframe);
    lw.add_global_callback(l, "ins", create_ins);
    lw.add_global_callback(l, "kbd", create_kbd);
    lw.add_global_callback(l, "label", create_label);
    lw.add_global_callback(l, "legend", create_legend);
    lw.add_global_callback(l, "li", create_li);
    lw.add_global_callback(l, "main", create_main);
    lw.add_global_callback(l, "map", create_map);
    lw.add_global_callback(l, "mark", create_mark);
    lw.add_global_callback(l, "menu", create_menu);
    lw.add_global_callback(l, "meter", create_meter);
    lw.add_global_callback(l, "nav", create_nav);
    lw.add_global_callback(l, "noscript", create_noscript);
    lw.add_global_callback(l, "object", create_object);
    lw.add_global_callback(l, "ol", create_ol);
    lw.add_global_callback(l, "optgroup", create_optgroup);
    lw.add_global_callback(l, "option", create_option);
    lw.add_global_callback(l, "output", create_output);
    lw.add_global_callback(l, "p", create_p);
    lw.add_global_callback(l, "picture", create_picture);
    lw.add_global_callback(l, "pre", create_pre);
    lw.add_global_callback(l, "progress", create_progress);
    lw.add_global_callback(l, "q", create_q);
    lw.add_global_callback(l, "rp", create_rp);
    lw.add_global_callback(l, "rt", create_rt);
    lw.add_global_callback(l, "ruby", create_ruby);
    lw.add_global_callback(l, "s", create_s);
    lw.add_global_callback(l, "samp", create_samp);
    lw.add_global_callback(l, "script", create_script);
    lw.add_global_callback(l, "section", create_section);
    lw.add_global_callback(l, "select", create_select);
    lw.add_global_callback(l, "small", create_small);
    lw.add_global_callback(l, "span", create_span);
    lw.add_global_callback(l, "strong", create_strong);
    lw.add_global_callback(l, "style", create_style);
    lw.add_global_callback(l, "sub", create_sub);
    lw.add_global_callback(l, "summary", create_summary);
    lw.add_global_callback(l, "sup", create_sup);
    lw.add_global_callback(l, "table", create_table);
    lw.add_global_callback(l, "tbody", create_tbody);
    lw.add_global_callback(l, "td", create_td);
    lw.add_global_callback(l, "template", create_template);
    lw.add_global_callback(l, "textarea", create_textarea);
    lw.add_global_callback(l, "tfoot", create_tfoot);
    lw.add_global_callback(l, "th", create_th);
    lw.add_global_callback(l, "thead", create_thead);
    lw.add_global_callback(l, "time", create_time);
    lw.add_global_callback(l, "title", create_title);
    lw.add_global_callback(l, "tr", create_tr);
    lw.add_global_callback(l, "u", create_u);
    lw.add_global_callback(l, "ul", create_ul);
    lw.add_global_callback(l, "var", create_var);
    lw.add_global_callback(l, "video", create_video);
    lw.add_global_callback(l, "wbr", create_wbr);






    creat_table_for_config_server();

  return lw.perform(l);
}
