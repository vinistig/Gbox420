echo "This script is for developement only"
echo "Protects shared Gbox420 and Gbox420 Nano Components from going out of sync by hard linking the source files"
echo "------------------------------------------"

echo "Creating folders"
mkdir ./src
mkdir ./src/Components
mkdir ./src/Modules

echo "Creating hard links"
ln ../src/Modules/HempyModule.cpp ./src/Modules
ln ../src/Modules/HempyModule.h ./src/Modules

ln ../src/Helpers.cpp ./src
ln ../src/Helpers.h ./src
ln ../src/RollingAverage.cpp ./src
ln ../src/RollingAverage.h ./src
ln ../src/WirelessCommands_Hempy.h ./src

ln ../src/Components/420Common.h ./src/Components
ln ../src/Components/420Common.cpp ./src/Components
ln ../src/Components/420Module.h ./src/Components
ln ../src/Components/420Module.cpp ./src/Components
ln ../src/Components/HempyBucket.h ./src/Components
ln ../src/Components/HempyBucket.cpp ./src/Components
ln ../src/Components/Sound.h ./src/Components
ln ../src/Components/Sound.cpp ./src/Components
ln ../src/Components/WaterPump.h ./src/Components
ln ../src/Components/WaterPump.cpp ./src/Components
ln ../src/Components/WeightSensor.h ./src/Components
ln ../src/Components/WeightSensor.cpp ./src/Components


echo "Done, press enter to exit"
read fakeinput 

