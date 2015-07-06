# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# User specific aliases and functions
export PS1='[ISY@\h \W]$'

bind '"\eOC":forward-word'
bind '"\eOD":backward-word'
