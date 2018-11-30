cd kernel
make
cd ../
cp kernel/kernelz boot/
cp kernel/system.map ./
cd boot
make
cd ../
cp boot/oskernel.elf.gz ./oskernel
perl map2idc.pl > system.idc
pause