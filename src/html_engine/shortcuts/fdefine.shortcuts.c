//silver_chain_scope_start
//mannaged by silver chain: https://github.com/OUIsolutions/SilverChain
#include "../../imports/imports.fdeclare.h"
//silver_chain_scope_end


LuaCEmbedResponse *create_html(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "html");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_head(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "head");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_a(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "a");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_abbr(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "abbr");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_address(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "address");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_article(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "article");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_aside(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "aside");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_audio(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "audio");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_b(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "b");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_bdi(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "bdi");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_bdo(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "bdo");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_blockquote(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "blockquote");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_body(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "body");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_button(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "button");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_canvas(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "canvas");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_caption(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "caption");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_cite(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "cite");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_code(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "code");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_colgroup(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "colgroup");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_data(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "data");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_datalist(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "datalist");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_dd(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "dd");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_del(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "del");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_details(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "details");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_dfn(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "dfn");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_dialog(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "dialog");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_div(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "div");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_dl(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "dl");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_dt(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "dt");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_em(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "em");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_fieldset(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "fieldset");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_figcaption(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "figcaption");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_figure(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "figure");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_footer(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "footer");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_form(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "form");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_h1(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "h1");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_h2(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "h2");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_h3(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "h3");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_h4(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "h4");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_h5(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "h5");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_h6(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "h6");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_header(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "header");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_i(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "i");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_iframe(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "iframe");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_ins(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "ins");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_kbd(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "kbd");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_label(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "label");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_legend(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "legend");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_li(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "li");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_main(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "main");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_map(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "map");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_mark(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "mark");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_menu(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "menu");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_meter(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "meter");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_nav(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "nav");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_noscript(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "noscript");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_object(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "object");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_ol(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "ol");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_optgroup(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "optgroup");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_option(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "option");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_output(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "output");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_p(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "p");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_picture(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "picture");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_pre(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "pre");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_progress(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "progress");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_q(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "q");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_rp(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "rp");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_rt(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "rt");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_ruby(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "ruby");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_s(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "s");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_samp(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "samp");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_script(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "script");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_section(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "section");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_select(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "select");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_small(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "small");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_span(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "span");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_strong(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "strong");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_style(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "style");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_sub(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "sub");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_summary(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "summary");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_sup(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "sup");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_table(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "table");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_tbody(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "tbody");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_td(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "td");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_template(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "template");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_textarea(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "textarea");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_tfoot(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "tfoot");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_th(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "th");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_thead(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "thead");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_time(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "time");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_title(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "title");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_tr(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "tr");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_u(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "u");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_ul(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "ul");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_var(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "var");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_video(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "video");
  return LuaCEmbed_send_table(fragment);
}

LuaCEmbedResponse *create_wbr(LuaCEmbed *args) {
  LuaCEmbedTable *fragment = raw_create_fragment(args);
  LuaCEmbedTable_set_string_prop(fragment, "tag", "wbr");
  return LuaCEmbed_send_table(fragment);
}
