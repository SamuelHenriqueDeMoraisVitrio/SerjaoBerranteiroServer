function install_dependencies()
    local hasher = darwin.dtw.newHasher()
    hasher.digest_folder_by_content("dependencies")
    local EXPECTED_SHA = '4b76bcb751467beaab7d3856702e35ba75a5d60bd407f1ba1ae6c2a7c2a4ccf2'
    if hasher.get_value() == EXPECTED_SHA then
        return
    end
    
    darwin.dtw.remove_any("dependencies")
    os.execute("mkdir -p dependencies")

    
    os.execute("curl -L https://github.com/OUIsolutions/LuaCEmbed/releases/download/0.8.3/LuaCEmbedOne.c -o dependencies/LuaCEmbedOne.c")
    os.execute("curl -L https://github.com/OUIsolutions/CWebStudio/releases/download/4.0.0/CWebStudioOne.c -o dependencies/CWebStudioOne.c")


    local new_hasher = darwin.dtw.newHasher()
    new_hasher.digest_folder_by_content("dependencies")
    print("new hasher is: " .. new_hasher.get_value())
end