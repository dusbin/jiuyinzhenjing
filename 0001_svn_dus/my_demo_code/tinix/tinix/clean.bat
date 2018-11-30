rm oskernel
rm system.map
rm system.idc
cd kernel
make clean
cd ../boot
make clean
pause