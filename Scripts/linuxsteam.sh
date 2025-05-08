#!/bin/sh

/opt/steam-utils/bin/steam -forcedesktopscaling=1.5 -console &

while ! [ -e $HOME/.steam/tmp/FreeBSD_Proton ]; do
	echo "Waiting for Steam to fully load."
	sleep 1
done

sleep 1
echo "Loading complete."
$HOME/Scripts/proton10lsu.sh