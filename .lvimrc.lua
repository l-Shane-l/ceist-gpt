local project_path = vim.fn.getcwd()

require("lspconfig").clangd.setup({
    cmd = {"clangd", "--background-index", "--compile-commands-dir="..project_path.."/build"},
    filetypes = {"c", "cpp", "objc", "objcpp"},
    root_dir = function() return project_path end,
    settings = {
        ["clangd"] = {
            ["compileFlags"] = {"-I"..project_path.."/include", "-I/usr/local/include", "-I/usr/include/x86_64-linux-gnu", "-I/usr/include", "-I/usr/include/c++/11", "-I/home/shane/anaconda3/bin/", "-I/usr/include/x86_64-linux-gnu/c++/11", "-I"..project_path.."/**"}
        }
    }
})
