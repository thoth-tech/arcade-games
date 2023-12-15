#!/bin/bash

GAME_DIR=$1
BINARY_NAME=$2             

cd $GAME_DIR

file=config.txt
# Removing CRLF characters
sed -i "s/\r//g" "${file}"

# Read the file
while read line || [ -n "$line" ]; do
	# Ignoring lines starting with # or empty lines
	if [[ ${line:0:1} == *"#"* ]] || [[ -z $line ]]; then
		continue
	fi
	# Split string with delimiter "="
	IFS='=' read -ra item <<< $line
	# If key is language
	if [[ ${item[0]} == "language" ]]; then
		# Store the value in language
		language="${item[1]}"
		continue
	fi
	# If key is compile-command
	if [[ ${item[0]} == "compile-command" ]]; then
		# Store the value in command
		command="${item[1]}"
		continue
	fi
done < $file

# Get the game name
game_name=$(basename $GAME_DIR)
echo Compiling game $game_name...


#Check for C# Language
if [[ $language == "C#" ]]; then
	echo "C# language detected, using skm dotnet publish"
	# If command is empty
	if [[ -z "$command" ]]; then
		echo "No compile command found, using default"
		if [[ $BINARY_NAME == "linux-arm" ]]; then
			skm dotnet publish --runtime linux-arm --no-self-contained -o ./compiled/
		else
			skm dotnet publish -o ./compiled/
		fi
	else
		echo Appending output flag and name/loc 
		command+=" -o ./compiled/"
	fi
else
	#Assume C++ Language or Makefile
	# create compiled folder
	mkdir compiled
	# If command is empty

	if [[ -z "$command" ]]; then
		echo "No compile command found, using default"
		skm g++ program.cpp -o ./compiled/$game_name
	else
		# If command starts with skm
		
		if [[ $command == "skm"* ]]; then
		echo Appending output flag and name/loc 
		command+=" -o ./compiled/$game_name"
		else
		echo Assuming usage of makefile, appending output name/loc 
		command+=" ./compiled/$game_name"
		fi
		echo "Running compile command: $command"
		eval $command
	fi  
fi

#Create tar Archvie of the compiled folder
mkdir published
tar -czvf published/$game_name-$BINARY_NAME.tar.gz -C compiled/ .

#Assets tar
AssetsTar=published/$game_name-assets.tar

# Create an empty Assets tar archive
tar -cvf "$AssetsTar" --files-from /dev/null

# Function to add a directory to the archive if it exists
add_directory_to_archive() {
    local directory="$1"
	if [ -d "$directory" ]; then
        tar -rf "$AssetsTar" -C "$(dirname "$directory")" "$(basename "$directory")"
    fi
}

if [ $BINARY_NAME = "win-x86" ]; then
	#List of Directories that may have assets (non case senstive)
	AssetsDirectories=(
		"resources"
		"animations"
		"bundles"
		"databases"
		"fonts"
		"images"
		"json"
		"sounds"
	)
else
	#List of Directories that may have assets (case senstive)
	AssetsDirectories=(
		"Resources"
		"resources"
		"Animations"
		"animations"
		"Bundles"
		"bundles"
		"Databases"
		"databases"
		"Fonts"
		"fonts"
		"Images"
		"images"
		"Json"
		"json"
		"Sounds"
		"sounds"
	)
fi

for dir in "${AssetsDirectories[@]}"; do
    add_directory_to_archive "$dir"
done


# List contents of the archive
archive_contents=$(tar -tf "$AssetsTar")

# Check if the archive is empty
if [ -z "$archive_contents" ]; then
    echo "No Assets Found"
	rm "$AssetsTar"
    else
	#compress the assets tar
	gzip "$AssetsTar"
	echo "Archive '$AssetsTar' created with specified directories."
fi
