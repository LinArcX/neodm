"-------------- Key-mapping(F2-F11 are free) ------------""
"Switch between .cpp/.hpp files
nnoremap <F2> :FSHere<CR>

function! FormatCurrentFile()
    silent execute "!clang-format -i ". " " . expand('%:p')
    :e
endfunction
au BufEnter,BufWrite *.cpp call FormatCurrentFile()
au BufEnter,BufWrite *.hpp call FormatCurrentFile()

"------------- vim-quickui settings  ------------------"
let s:cd_top = "cd ..;"
let s:update = "update | w |"
let s:cmake = "cd build; cmake"
let s:make = "cd build/; make; \n"
let s:ccc = "clear; cd build; cmake"

let s:make_debug   = s:cmake ." ". $CMAKE_ARGS ." " . $C_COMPILER ." ". $CXX_COMPILER ." ". $C_VERSION ." ". $CMAKE_DEBUG . s:cd_top . s:make
let s:make_release = s:cmake ." ". $CMAKE_ARGS ." " . $C_COMPILER ." ". $CXX_COMPILER ." ". $C_VERSION ." ". $CMAKE_RELEASE . s:cd_top . s:make
let s:make_test    = s:cmake ." ". $CMAKE_ARGS ." " . $C_BUILD_TEST ." ". $C_COMPILER ." ". $CXX_COMPILER ." ". $C_VERSION ." ". $CMAKE_DEBUG . s:cd_top . s:make
let s:project_clear = "clear; rm -rfv build/**; rm -rfv debug/**; rm -rfv release/**; \n"

let s:cmake_show_help = "cmake --help-full | less && exit \n"
let s:cmake_configure = s:cmake ." ". " -L .. | less && exit \n"
let s:cmake_show_vars = s:cmake ." ". $C_COMPILER ." ". "..  -LAH | less && exit \n"

call quickui#menu#clear('P&roject')
" mrautsc--pvfe--io--b
call quickui#menu#install('P&roject', [
            \ [ '&make(debug)', s:update.'call HTerminal(0.4, 300.0, "'. s:make_debug .'")' ],
            \ [ '&run(debug)', s:update.'call FTerminal("clear; debug/'. $NAME .' \n")' ],
            \ [ 'm&ake(release)', s:update.'call HTerminal(0.4, 300.0, "'. s:make_release .'")' ],
            \ [ 'r&un(release)', s:update.'call FTerminal("clear; release/'. $NAME .' \n")' ],
            \ [ 'make(&test)', s:update.'call HTerminal(0.4, 300.0, "'. s:make_test .'")' ],
            \ [ 'run(te&st)', s:update.'call HTerminal(0.4, 300.0, "clear; build/tests/tests \n")' ],
            \ [ '&clean-project', s:update.'call HTerminal(0.4, 300.0, "'. s:project_clear .'")' ],
            \ [ "--", '' ],
            \ [ "cmake-hel&p", 'call FTerminal("'. s:cmake_show_help .'")' ],
            \ [ "cmake-&vars", 'call FTerminal("'. s:cmake_show_vars .'")' ],
            \ [ "cmake-con&figure", 'call FTerminal("'. s:cmake_configure .'")' ],
            \ [ "cmake-help-s&earch", 'call CmakeSH()' ],
            \ [ "--", '' ],
            \ [ "l&ibc-help", 'call LibcSH()' ],
            \ [ "libc-help-under-curs&or", 'call LibcSHUC()' ],
            \ [ "--", '' ],
            \ [ "nix-collect-gar&bage", 'call HTerminal(0.4, 300.0, "clear; nix-store --gc \n")' ],
            \ ], 5000)

"coiustrpfs
call quickui#menu#install('&Debugging', [
            \ [ "&continue\tF(5)", 'call feedkeys("\<Plug>VimspectorContinue")' ],
            \ [ "step-&over\tF(6)", 'call feedkeys("\<Plug>VimspectorStepOver")' ],
            \ [ "step-&into\tF(7)", 'call feedkeys("\<Plug>VimspectorStepInto")' ],
            \ [ "step-o&ut\tF(8)", 'call feedkeys("\<Plug>VimspectorStepOut")' ],
            \ [ "&stop\tF(9)", 'call feedkeys("\<Plug>VimspectorStop")' ],
            \ [ "&toggle-breakpoint\tF(10)", 'call feedkeys("\<Plug>VimspectorToggleBreakpoint")' ],
            \ [ "&pause\tShift-p", 'call feedkeys("\<Plug>VimspectorPause")' ],
            \ [ "&restart\tShift-r", 'call feedkeys("\<Plug>VimspectorRestart")' ],
            \ [ "clo&se-debugger\tShift-s", ':call vimspector#Reset()' ],
            \ ], 5001)
