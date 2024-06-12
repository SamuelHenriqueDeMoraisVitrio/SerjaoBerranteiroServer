# A fazer

- [x] Documentar kill
- [ ] Documentar todos set_servers
- [ ] Documentar em PT-BR
- [x] Documentar todos commands html
- [x] Documentar função para printar html
- [x] Documentar a opção de não escolher o status_code
- [x] Documentar multi e single quando não nescessario
- [x] Documentar como escolher a resolução
- [ ] Fazer um doc help

***

- [x] Separar o main em mais files
- [x] Portas padrão é 200 em responses
- [x] Colocar todos os comandos html no autocomplete
- [x] Colocar funções da engine html no autocomplete
- [x] Colocar opção de resolução em “desktop”
- [x] Opção de escolher o título em “desktop”
- [x] Vazamento no "desktop"
```
{
Compilação concluida. Iniciando...


Sever is running on port:3000
server rodando em 3000
Gtk-Message: 15:20:58.124: Not loading module "atk-bridge": The functionality is provided by GTK natively. Please try to not load it.

(chrome:18393): Gtk-WARNING **: 15:20:58.388: GTK+ module /snap/chromium/2873/gnome-platform/usr/lib/gtk-2.0/modules/libcanberra-gtk-module.so cannot be loaded.
GTK+ 2.x symbols detected. Using GTK+ 2.x and GTK+ 3 in the same process is not supported.
Gtk-Message: 15:20:58.388: Failed to load module "canberra-gtk-module"

(chrome:18393): Gtk-WARNING **: 15:20:58.395: GTK+ module /snap/chromium/2873/gnome-platform/usr/lib/gtk-2.0/modules/libcanberra-gtk-module.so cannot be loaded.
GTK+ 2.x symbols detected. Using GTK+ 2.x and GTK+ 3 in the same process is not supported.
Gtk-Message: 15:20:58.395: Failed to load module "canberra-gtk-module"
[18393:18393:0610/152100.911037:ERROR:object_proxy.cc(576)] Failed to call method: org.freedesktop.ScreenSaver.GetActive: object_path= /org/freedesktop/ScreenSaver: org.freedesktop.DBus.Error.NotSupported: This method is not implemented
[18531:18531:0610/152104.748239:ERROR:gl_surface_presentation_helper.cc(260)] GetVSyncParametersIfAvailable() failed for 1 times!
[18531:18531:0610/152109.026197:ERROR:gl_surface_presentation_helper.cc(260)] GetVSyncParametersIfAvailable() failed for 2 times!
[18531:18531:0610/152113.454041:ERROR:gl_surface_presentation_helper.cc(260)] GetVSyncParametersIfAvailable() failed for 3 times!
application terminated


==18381==
==18381== HEAP SUMMARY:
==18381==     in use at exit: 149,009 bytes in 1,361 blocks
==18381==   total heap usage: 2,006 allocs, 645 frees, 316,188 bytes allocated
==18381==
==18381== LEAK SUMMARY:
==18381==    definitely lost: 0 bytes in 0 blocks
==18381==    indirectly lost: 0 bytes in 0 blocks
==18381==      possibly lost: 0 bytes in 0 blocks
==18381==    still reachable: 149,009 bytes in 1,361 blocks
==18381==         suppressed: 0 bytes in 0 blocks
==18381== Rerun with --leak-check=full to see details of leaked memory
==18381==
==18381== For lists of detected and suppressed errors, rerun with: -s
==18381== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
}
```
- [ ] Icone do chromium, funcionalidade de trocar.
```
[Desktop Entry]
Version=1.0
Name=Meu Chromium App
Comment=Meu aplicativo baseado no Chromium
Exec=chromium --app=https://example.com
Icon=/caminho/para/o/icone/icon.png
Terminal=false
Type=Application
Categories=Utility;Application;
```
