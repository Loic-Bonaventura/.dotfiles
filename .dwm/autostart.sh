#!/bin/sh

# Checking monitor setup
mon=$(xrandr)
actual_mon=$(echo "$mon" | rg -e " connected" | awk ' { print $1 } ' | tr -d '[:space:]')
echo "$actual_mon"
if [ "$actual_mon" = "eDP-1" ]; then
    xrandr --output eDP-1 --auto --scale 0.55 --brightness 1
    nitrogen --set-zoom-fill "$WALLPAPER"
else
    xrandr --output eDP-1 --off --output DP-1 --primary --mode 1920x1080 --pos 0x0 --rotate normal --output DP-2 --mode 1920x1080 --pos 1920x0 --rotate normal
    nitrogen --set-auto --head=0 "$WALLPAPER"
    nitrogen --set-auto --head=1 "$WALLPAPER"
fi

# Changing CPU frequency for more performance
cpupower frequency-set -g performance

# Make the function keys default instead of media keys (for my Drevo Calibur 2 keyboard)
tee /sys/module/hid_apple/parameters/fnmode

# Launch the picom compositor, mainly to prevent screen tearing
picom -b --experimental-backends &

# I use slstatus on my dwm bar
slstatus &

# Launch Emacs' daemon, to spawn emacsclient afterwards, thus reducing its launch time
/usr/bin/emacs --daemon &

# I use flameshot for screenshots
flameshot &
