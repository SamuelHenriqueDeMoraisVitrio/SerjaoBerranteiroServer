

typedef  struct  HtmlorError{
    char *html;
    LuaCEmbedResponse *error;
}HtmlorError;


HtmlorError render_component_raw(LuaCEmbedTable *self);

LuaCEmbedResponse * render_component(LuaCEmbedTable *self,LuaCEmbed *args);

LuaCEmbedResponse * create_fragment(LuaCEmbed *args);

LuaCEmbedResponse * create_component(LuaCEmbed *args);

