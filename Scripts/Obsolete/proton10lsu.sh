#!/bin/sh

# <<Important>> You first need to add Proton 10 to the known versions list in `/opt/steam-utils/tools/LSU_FreeBSD_Wine/run.rb`. It should look like this:

# KNOWN_VERSIONS = {
#	'7.0' => {appId: 1887720},
#	'8.0' => {appId: 2348590},
#	'9.0' => {appId: 2805730},
#  '10.0' => {appId: 3658110}
# }


# Prepare 'Linuxulator-Steam-Utils' for FreeBSD Proton 10 environment.

mkdir $HOME/.steam/tmp/FreeBSD_Proton/proton_10.0/files.tmp/lib64
ln -sf -h $HOME/.i386-wine-pkg/usr/local/lib/gstreamer-1.0 $HOME/.steam/tmp/FreeBSD_Proton/proton_10.0/files.tmp/lib
ln -sf -h /usr/local/lib/gstreamer-1.0 $HOME/.steam/tmp/FreeBSD_Proton/proton_10.0/files.tmp/lib64
ln -sf -h $HOME/.steam/steam/steamapps/common/Proton\ 10.0/files/lib/vkd3d $HOME/.steam/tmp/FreeBSD_Proton/proton_10.0/files.tmp/lib64
cp -r $HOME/.steam/tmp/FreeBSD_Proton/proton_10.0/files.tmp/lib/wine $HOME/.steam/tmp/FreeBSD_Proton/proton_10.0/files.tmp/lib64
ln -sf -h /usr/local/wine-proton/lib/wine/x86_64-windows/* $HOME/.steam/tmp/FreeBSD_Proton/proton_10.0/files.tmp/lib/wine/x86_64-windows
sed -i '' 's/if os.environ.get(\"PROTON_USE_WOW64\", None) == \"1\" or not file_exists(self.wine_bin, follow_symlinks=True)://g' $HOME/.steam/tmp/FreeBSD_Proton/proton_10.0/proton
sed -i '' 's/self.bin_dir = self.path(\"files\/bin-wow64\/\")//g' $HOME/.steam/tmp/FreeBSD_Proton/proton_10.0/proton
sed -i '' 'H;$!d;x;s/.//;s/self.wine_bin = \"wine\"//2' $HOME/.steam/tmp/FreeBSD_Proton/proton_10.0/proton
sed -i '' 's/self.wine64_bin = self.bin_dir + \"wine\"//g' $HOME/.steam/tmp/FreeBSD_Proton/proton_10.0/proton
sed -i '' 'H;$!d;x;s/.//;s/self.wineserver_bin = \"wineserver\"//2' $HOME/.steam/tmp/FreeBSD_Proton/proton_10.0/proton