# Lines configured by zsh-newuser-install
HISTFILE=~/.histfile
HISTSIZE=100000
SAVEHIST=100000
# End of lines configured by zsh-newuser-install
# The following lines were added by compinstall
zstyle :compinstall filename '/home/lmn/.zshrc'
setopt autocd		# Automatically cd into typed directory.
stty stop undef		# Disable ctrl-s to freeze terminal.
setopt interactive_comments
autoload -Uz compinit
compinit
# End of lines added by compinstall

autoload -U colors && colors	# Load colors
PS1="%{$fg[red]%}[%{$fg[yellow]%}%n%{$fg[green]%}@%{$fg[blue]%}%M %{$fg[magenta]%}%~%{$fg[red]%}]%{$reset_color%}$ "

# Set bat as my manpager
# export BAT_THEME="base16"
export MANPAGER="sh -c 'col -bx | bat --theme=\"Monokai Extended\" -l man -p'"

# Adding Doom Emacs to my PATH
export PATH="$HOME/.emacs.d/bin:$PATH"

# Setting vim as my text editor
export EDITOR="nvim"
export VISUAL="nvim"

# Setting Ungoogled-Chromium as my default web browser
export BROWSER="chromium"

# Get proper colors
export TERM="st-256color"         

# aliases
alias svelte="npx degit sveltejs/template --force" # +target
alias ls="exa -a --icons --group-directories-first"
alias grep="rg"
alias c="clear"
alias arch="cowsay \"i use arch btw\""
alias cowsay="fortune | cowsay"
alias icat="kitty +kitten icat"
alias pga="source pgadmin4/bin/activate"
alias fn="echo 0 | sudo tee /sys/module/hid_apple/parameters/fnmode"
alias spt="launchspt"
alias i3con="$EDITOR $HOME/.config/i3/config"
alias alacon="$EDITOR $HOME/.config/alacritty/alacritty.yml"
alias picon="$EDITOR $HOME/.config/picom.conf"
alias zshcon="$EDITOR $HOME/.zshrc"
alias r="ranger"
alias bc="eva"
alias fetch="neofetch"
alias laptop="xrandr --output eDP-1 --auto --scale 0.55 --brightness 1"
alias vim="nvim"
alias wall="nitrogen --set-zoom-fill Pictures/wallpapers/0095.jpg"
alias walls="nitrogen --set-auto --head=0 Pictures/wallpapers/0095.jpg && nitrogen --set-auto --head=1 Pictures/wallpapers/0095.jpg"
alias lcfg="laptop && wall && clear"
alias emacs="emacsclient -c -a 'emacs'"
alias kemacs="killall \emacs && \emacs --daemon &"
alias kspt="killall spotifyd"
alias mkdir="mkdir -pv"
alias ytfzf="ytfzf -t --thumbnail-quality=medium --keep-cache"
alias nofetch="nofetch.sh"
alias zat="zathura"

# pacman and yay
alias ss="yay -Ss"
alias S="sudo pacman -S"

# Replacing ls with exa
alias ls='exa -al --color=always --group-directories-first' # my preferred listing
alias la='exa -a --color=always --group-directories-first'  # all files and dirs
alias ll='exa -l --color=always --group-directories-first'  # long format
alias lt='exa -aT --color=always --group-directories-first' # tree listing

# memo git filter-branch -f --index-filter 'git rm --cached --ignore-unmatch"

eval "$(starship init zsh)"
eval "$(zoxide init zsh)"

# Script allowing to use arrows for navigation between words in the terminal

function zle-line-init {
    marking=0
}
zle -N zle-line-init

function select-char-right {
    if (( $marking != 1 ))
    then
        marking=1
        zle set-mark-command
    fi
    zle .forward-char
}
zle -N select-char-right

function select-char-left {
    if (( $marking != 1 ))
    then
        marking=1
        zle set-mark-command
    fi
    zle .backward-char
}
zle -N select-char-left

function forward-char {
    if (( $marking == 1 ))
    then
        marking=0
        NUMERIC=-1 zle set-mark-command
    fi
    zle .forward-char
}
zle -N forward-char

function backward-char {
    if (( $marking == 1 ))
    then
        marking=0
        NUMERIC=-1 zle set-mark-command
    fi
    zle .backward-char
}
zle -N backward-char

function delete-char {
    if (( $marking == 1 ))
    then
        zle kill-region
        marking=0
    else
        zle .delete-char
    fi
}
zle -N delete-char

# bindkey ';6D' select-word-left ## not working yet                                                                                                                                                                                                                               
# bindkey ';6C' select-word-right ## not working yet                                                                                                                                                                                                                              
# bindkey ';2D' select-char-left   # assuming xterm                                                                                                                                                                                                                               
# bindkey ';2C' select-char-right  # assuming xterm                                                                                                                                                                                                                               
bindkey ';5D' backward-word
bindkey ';5C' forward-word
bindkey '^[[3~' delete-char

### ZOXIDE environment variables ###
_ZO_DATA_DIR=$HOME/.local/share
_ZO_ECHO=1
_ZO_MAXAGE=50000
_ZO_RESOLVE_SYMLINKS=1
####################################

# =============================================================================
#
# Utility functions for zoxide.
#

# pwd based on the value of _ZO_RESOLVE_SYMLINKS.
function __zoxide_pwd() {
    \builtin pwd -L
}

# cd + custom logic based on the value of _ZO_ECHO.
function __zoxide_cd() {
    # shellcheck disable=SC2164
    \builtin cd "$@"
}

# =============================================================================
#
# Hook configuration for zoxide.
#

# Hook to add new entries to the database.
function __zoxide_hook() {
    zoxide add -- "$(__zoxide_pwd)"
}

# Initialize hook.
if [ "${__zoxide_hooked}" != '1' ]; then
    __zoxide_hooked='1'
    chpwd_functions=("${chpwd_functions[@]}" "__zoxide_hook")
fi

# =============================================================================
#
# When using zoxide with --no-aliases, alias these internal functions as
# desired.
#

# Jump to a directory using only keywords.
function __zoxide_z() {
    if [ "$#" -eq 0 ]; then
        __zoxide_cd ~
    elif [ "$#" -eq 1 ] && [ "$1" = '-' ]; then
        if [ -n "${OLDPWD}" ]; then
            __zoxide_cd "${OLDPWD}"
        else
            # shellcheck disable=SC2016
            \builtin printf 'zoxide: $OLDPWD is not set'
            return 1
        fi
    elif [ "$#" -eq 1 ] &&  [ -d "$1" ]; then
        __zoxide_cd "$1"
    else
        \builtin local __zoxide_result
        __zoxide_result="$(zoxide query --exclude "$(__zoxide_pwd)" -- "$@")" \
            && __zoxide_cd "${__zoxide_result}"
    fi
}

# Jump to a directory using interactive search.
function __zoxide_zi() {
    \builtin local __zoxide_result
    __zoxide_result="$(zoxide query -i -- "$@")" && __zoxide_cd "${__zoxide_result}"
}

# =============================================================================
#
# Convenient aliases for zoxide. Disable these using --no-aliases.
#

# Remove definitions.
function __zoxide_unset() {
    \builtin unalias "$@" &>/dev/null
    \builtin unfunction "$@" &>/dev/null
    \builtin unset "$@" &>/dev/null
}

__zoxide_unset 'z'
function z() {
    __zoxide_z "$@"
}

__zoxide_unset 'zi'
function zi() {
    __zoxide_zi "$@"
}

# if [[ $TERM == xterm  ]]; then TERM=xterm-256color; fi

# =============================================================================

# 1Password-cli #
export OP_SESSION_my="VihLKot44DRocfSKpNatVdRstxWqhe5JFL7fOxfX_VI"

# General exports
export PGUSER=postgres
export NODE_ENV=dev

# ZSH plugins
source $HOME/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh
source $HOME/zsh-autosuggestions/zsh-autosuggestions.zsh
