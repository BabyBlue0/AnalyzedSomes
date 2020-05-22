" setting
" coding: utf-8
set fenc=utf-8
" no create backup
set nobackup
" no create swap
set noswapfile
" auto reload
set autoread
" 編集中でも他のファイルが開ける
set hidden
" 入力中のコマンドをステータスに表示する
set showcmd

" 見た目
" 行番号
set number
" 現在の行を強調
"set cursorline
" 現在の行を強調（縦）
"set cursorcolumn
" 行末の最後の文字までカーソルを移動できるようにする
set virtualedit=onemore
" スマートインデント
set smartindent
" ビープ音を可視化
set visualbell
" カッコ入力時に対応するカッコを表示
set showmatch
" ステータスラインを常に表示
set laststatus=2
" コマンドラインの補完
set wildmode=list:longest
" 折り返し時に表示業単位での移動できるようになる
nnoremap j gj
nnoremap k gk
" シンタックスハイライト
syntax enable

" Tab系
" 不可視文字を可視化(タブが「▸-」と表示される)
set list listchars=tab:\▸\-
" Tab文字を半角スペースにする
set expandtab
" 行頭以外のTab文字の表示幅（スペースいくつ分）
set tabstop=2
" 行頭でのTab文字の表示幅
set shiftwidth=2

" 検索系
" 検索文字列が小文字の場合は大文字小文字を区別なく検索する
set ignorecase
" 検索文字列に大文字が含まれている場合は区別して検索する
set smartcase
" 検索文字列入力時に順次対象文字列にヒットさせる
set incsearch
" 検索時に最後まで行ったら最初に戻る
set wrapscan
" 検索語をハイライト表示
set hlsearch
" ESC連打でハイライト解除
nmap <Esc><Esc> :nohlsearch<CR><Esc>
