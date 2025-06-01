
function main()
    darwin.dtw.remove_any("release")

    install_dependencies()


    darwin.silverchain.generate({
        src = "src",
        project_short_cut="serjao_berranteiro",
        tags = { "dependencie", "consts", "macros","ftypes", "globals", "fdeclare", "fdefine" },
        implement_main = false
    })
   local no_dep_amalgamation = darwin.camalgamator.generate_amalgamation_with_callback("src/one.c",
        function(import, path)
            if import == "src/imports/../dependencie.dependencies.h"  then
                return "dont-include"
            end
            return "include-once"

        end,
        MAX_CONNTENT,
        MAX_RECURSION
    )
    darwin.dtw.write_file("release/serjao_no_dep.c", no_dep_amalgamation)
    local full_amalgamation = darwin.camalgamator.generate_amalgamation("src/one.c", MAX_CONNTENT, MAX_RECURSION)
    darwin.dtw.write_file("release/serjao_full.c", full_amalgamation)

    darwin.dtw.copy_any_overwriting("extra/starter.lua","release/serjao_berranteiro/serjao_berranteiro.lua")
    os.execute("gcc src/one.c  -shared  -fpic -o release/serjao_berranteiro/serjao_berranteiro.so")

    --zip the folder 
    os.execute("cd release && zip -r serjao_berranteiro.zip serjao_berranteiro")
end