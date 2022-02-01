
#!/usr/bin/env bash

# Setting primary monitor
PRIMARY_MONITOR=$(xrandr | grep "primary" | awk '{print $1}')

# Terminate already running bar instances
killall -q polybar

# Wait until the processes have been shut down
while pgrep -u $UID -x polybar >/dev/null; do sleep 1; done

if type "xrandr" > /dev/null; then
  for m in $(xrandr --query | grep " connected" | cut -d" " -f1); do
	if [ "$MONITOR" = "$PRIMARY_MONITOR" ]; then
        export TRAY_POS="right"
    fi
    MONITOR=$m polybar --reload mainbar-i3 -c ~/.config/polybar/config &
	unset TRAY_POS
  done
else
	polybar --reload mainbar-i3 -c ~/.config/polybar/config &
fi

echo "Bars launched..."