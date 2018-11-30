#! /bin/bash
echo "begin..."
umount none
rm -rf /opt/aufs/
mkdir -p /opt/aufs
cd /opt/aufs/
mkdir container-layer
cd container-layer/
touch container-layer.txt
#vim container-layer.txt "I am container-layer"
echo "I am container-layer" >container-layer.txt
cd ..
mkdir image-layer{1..4}
cd image-layer1
#vim image-layer1.txt "I am image-layer1"
echo "I am image-layer1" >image-layer1.txt
cd ..
cd image-layer2
cat > image-layer2.txt <<EOF
I am image-layer2
EOF
cd ..
cd image-layer3
#vim image-layer3.txt "I am image-layer3"
echo "I am image-layer3" >image-layer3.txt
cd ..
cd image-layer4
#vim image-layer4.txt "I am image-layer4"
echo "I am image-layer4" >image-layer4.txt
cd ..
mkdir mnt
mount -t aufs -o dirs=./container-layer:./image-layer4:./image-layer3:./image-layer2:./image-layer1 none ./mnt
tree mnt/
cd mnt/
cd ..
echo "write to mnt's image-layer1.txt"
echo -e "write to mnt's image-layer1.txt" >> ./mnt/image-layer1.txt 
echo "cat mnt/image-layer1.txt"
cat mnt/image-layer1.txt 
echo "cat image-layer1/image-layer1.txt"
cat image-layer1/image-layer1.txt 
echo "cat container-layer/image-layer1.txt"
cat container-layer/image-layer1.txt 
echo "write to mnt's image-layer4.txt"
echo -e "write to mnt's image-layer4.txt" >> ./mnt/image-layer4.txt 
echo "cat ./mnt/image-layer4.txt"
cat ./mnt/image-layer4.txt 
echo "cat ./image-layer4/image-layer4.txt"
cat ./image-layer4/image-layer4.txt 
echo "cat ./container-layer/image-layer1.txt"
cat ./container-layer/image-layer1.txt 
echo "cat ./container-layer/image-layer4.txt"
cat ./container-layer/image-layer4.txt 
echo "end..."
