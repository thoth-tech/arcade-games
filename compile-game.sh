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
			# RID linux-arm64 = 64-bit AArch64 (Pi 3/4/5 with 64-bit OS).
			# RID linux-arm   = 32-bit armhf (legacy Pi); the Pi5 64-bit OS has no armhf loader.
			skm dotnet publish --runtime linux-arm64 --no-self-contained -o ./compiled/
		else
			skm dotnet publish -o ./compiled/
		fi
	else
		echo Appending output flag and name/loc
		command+=" -o ./compiled/"
		# If targeting ARM and the user's compile-command didn't pin a runtime,
		# force linux-arm64 so we don't fall back to the host runner's RID (x86-64).
		# Skipped when the user already specified --runtime, to respect their choice.
		if [[ $BINARY_NAME == "linux-arm" ]] && [[ $command != *"--runtime"* ]]; then
			echo "Injecting --runtime linux-arm64 --no-self-contained for ARM target"
			command+=" --runtime linux-arm64 --no-self-contained"
		fi
		echo "Running compile command: $command"
		eval $command
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

# Bundle asset directories into compiled/ BEFORE tarring, so the platform
# tarball is self-contained (binary + resources in one archive).
# Previously assets were packed into a separate <game>-assets.tar.gz that
# publish-release never picked up — end users got a binary with no resources.
if [ "$BINARY_NAME" = "win-x86" ]; then
	# Windows filesystem is case-insensitive, list lowercase only
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
	# Linux/macOS filesystem is case-sensitive, list both cases
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

bundled=()
for dir in "${AssetsDirectories[@]}"; do
	if [ -d "$dir" ]; then
		cp -r "$dir" compiled/
		bundled+=("$dir")
	fi
done
if [ ${#bundled[@]} -eq 0 ]; then
	echo "No asset directories found to bundle"
else
	echo "Bundled asset directories into compiled/: ${bundled[*]}"
fi

#Create tar archive of the compiled folder (binary + bundled assets)
mkdir published
tar -czvf published/$game_name-$BINARY_NAME.tar.gz -C compiled/ .
