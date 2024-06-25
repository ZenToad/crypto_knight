-- Function to execute the batch script with the current buffer's filename
function RunProtoBuild()
    -- Get the full path of the current buffer
    local current_file = vim.fn.expand("%:p")
    -- Ensure the path uses backslashes for Windows compatibility
    current_file = current_file:gsub("/", "\\")
    -- Construct the command
    local cmd = string.format('build\\test.bat "%s"', current_file)
    -- Execute the command in a floating terminal
    require('toggleterm.terminal').Terminal:new({ cmd = cmd, hidden=false, direction = "float" }):toggle()
end

-- Keymapping to call the function
-- vim.api.nvim_set_keymap('n', '<F5>', ':lua RunProtoBuild()<CR>', { noremap = true, silent = true })
vim.api.nvim_set_keymap('n', '<F5>', ':TermExec cmd="echo %" go_back=0 direction=float<CR>', { noremap = true, silent = true })
