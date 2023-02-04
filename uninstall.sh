echo ">>> Uninstalling rmdecis lib ..."
xargs rm < ./build/install_manifest.txt
echo ">>> Update ldconfig ..."
sudo ldconfig
echo ">>> Lib rmdecis removed."