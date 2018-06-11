" Vim syntax file
" Language: Vongsprache
" Maintainer: Arc676/Alessandro Vinciguerra
" Latest Revision: 11 June 2018

if exists("b:current_syntax")
  finish
endif

" Keywords
syn keyword langKeyword i bims vong her mit bis Funktionigkeit
syn keyword langKeyword am Wahrigkeit Sonstigkeit solange hab bidde

syn match langComment "#.*$"

" Literals
syn region langString start='"' end='"' contained
syn match langNumber "\d+\.?\d*"

hi def link langKeyword		Statement
hi def link langComment		Comment
hi def link langString		Constant
hi def link langNumber		Constant
