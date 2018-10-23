# !/bin/bash
echo "hello!"
#whiptail --title "<dialog box title>" --yesno "<text to show>" <height> <width>
##ok按钮继续下一步
if (whiptail --title "Test Yes/No Box" --yesno "Choose between Yes and No." 10 60) then
	echo "You chose Yes. Exit status was $?."
else
	echo "You chose No. Exit status was $?."
fi 