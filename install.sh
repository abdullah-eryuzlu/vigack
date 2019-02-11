
GREEN="\033[0;32m"
RESET="\033[0m"
YELLOW="\033[0;33m"
RED="\033[0;31m"

cd ./src
gcc main.c unsigned_pair.c vigenere.c cracker.c -o vigack
echo -e "[ + ]$GREEN Compiled$RESET"
error=$(mv ./vigack /usr/bin/vigack 2>/dev/null)

if [ $? -eq 0 ]; then
	echo -e "[ + ]$GREEN Moved executable to /usr/bin/vigack$RESET"
else
	echo -e "[ - ]$RED Couldn't move the executable to /usr/bin/vigack$RESET \n[ ! ]$YELLOW Root privileges may needed$RESET"
	exit
fi


cd ..
cp ./vigackconfig ~/.vigackconfig
echo -e "[ + ]$GREEN Copied configure file to ~/.vigackconfig$RESET"
echo -e "[ + ]$GREEN Done"

