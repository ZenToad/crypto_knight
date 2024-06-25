

-- vim.api.nvim_set_keymap('n', '<F5>', ':TermExec cmd="build\\\\proto_build.bat ../prototypes/proto3/guitest.c" go_back=0 direction=float<CR>', { noremap = true, silent = true })

-- Function to execute the batch script with the current buffer's filename
function RunProtoBuild()
    local current_file = vim.fn.expand("%:p") -- Get the full path of the current buffer
    local cmd = string.format('build\\\\proto_build.bat %s', current_file)
    require('toggleterm.terminal').Terminal:new({ cmd = cmd, direction = "float" }):toggle()
end

-- Keymapping to call the function
vim.api.nvim_set_keymap('n', '<F5>', ':lua RunProtoBuild()<CR>', { noremap = true, silent = true })

