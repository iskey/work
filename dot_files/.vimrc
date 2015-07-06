set nocompatible "关闭
syntax on "自动语法高亮
"colorscheme molokai "设定配色方案
colorscheme koehler
set number "显示行号
set ruler "打开状态标尺
set shiftwidth=4 "设定<<和>>命令移动的宽度
set softtabstop=4 "设定按退格键时可以一次删掉4个空格
set tabstop=4 "设定tab长度为4
set nobackup "覆盖文件时不备份
"set autochdir "自动切换当前目录为当前文件所在的目录
filetype plugin indent on "开启插件
set backupcopy=yes "设置备份时的行为为覆盖
set ignorecase smartcase "搜索时忽略大小写，但在一个或以上大写字母时仍保持对大小写敏感
set nowrapscan "禁止在搜索到文件两端时重新搜索
set incsearch "输入搜索内容时，显示搜索结果
set hlsearch " 搜索时高亮显示被找到的文本
set noerrorbells " 关闭错误信息响铃
set novisualbell " 关闭使用可视响铃代替呼叫
set t_vb= " 置空错误铃声的终端代码
"set showmatch " 插入括号时，短暂地跳转到匹配的对应括号
"set matchtime=2 " 短暂跳转到匹配括号的时间
set magic " 设置魔术
set hidden " 允许在有未保存的修改时切换缓冲区，此时的修改由 vim 负责保存
set guioptions-=T " 隐藏工具栏
set guioptions-=m " 隐藏菜单栏
set smartindent " 开启新行时使用智能自动缩进
set backspace=indent,eol,start
" 不设定在插入状态无法用退格键和 Delete 键删除回车符
set cmdheight=1 " 设定命令行的行数为 1
set laststatus=2 " 显示状态栏 (默认值为 1, 无法显示状态栏)
set statusline=\ %<%F[%1*%M%*%n%R%H]%=\ %y\ %0(%{&fileformat}\ %{&encoding}\ %c:%l/%L%)\ " 设置在状态行显示的信息
"set foldenable " 开始折叠
"set foldmethod=syntax " 设置语法折叠
"set foldcolumn=0 " 设置折叠区域的宽度
"setlocal foldlevel=1 " 设置折叠层数为
" set foldclose=all " 设置为自动关闭折叠
nnoremap <space> @=((foldclosed(line('.')) < 0) ? 'zc' : 'zo')<CR>
" 用空格键来开关折叠
set tags=tags;

"+++ taglist +++++++++++++++++++++++++++++++++++++++++
let Tlist_Auto_Open = 1    "自动打开taglist窗口
"let Tlist_Ctags_Cmd = ’ctags’   "设置ctags命令,当ctags命令没在PATH变量的路径中时使用
let Tlist_Use_Lift_Window = 1   "taglist窗口在屏幕左边
"let Tlist_Use_Right_Window = 1   "taglist窗口在屏幕右边
let Tlist_Use_SingleClick = 1   "鼠标单击即可跳到对应的tags处，缺省为双击
let Tlist_Exit_OnlyWindow = 1   "如果taglist窗口是最后一个窗口则退出VIM
"let Tlist_WinWidth = 25     "设置taglist窗口宽度
"let Tlist_Show_One_File=1    "只显示当前文件中的tags
let Tlist_File_Fold_Auto_Close = 1  "其他文件的tags都被折叠起来
"let Tlist_GainFocus_On_ToggleOpen = 1 "打开时默认焦点在taglist窗口中
""===tagslist================================>

"set mouse=a
