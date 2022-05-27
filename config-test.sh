#!/bin/bash
set -e
# Stores the error messages
errorList=()
# The file to be tested
file=$1

# Checks if the config file exists
if [ ! -f "$file" ]; then
	echo "File '$file' does not exist"
	exit 1
fi

# Retreive base path from the first argument
basePath=$(dirname "$1")/

# Removing CRLF characters
sed -i "s/\r//g" "${file}"

# Stores all the expected properties
declare -A propertyDict
propertyDict=( ["repository"]=0 ["language"]=0 ["image"]=0 ["title"]=0 ["genre"]=0 ["rating"]=0 ["author"]=0 ["executable"]=0 ["description"]=0 )

# Reading the file
while read line || [ -n "$line" ]; do
	# Ignoring lines starting with # or empty lines
	if [[ ${line:0:1} == *"#"* ]] || [[ -z $line ]]; then
		continue
	fi
	# Split string with delimiter "="
	IFS='=' read -ra array <<< $line
	# Checking if key is in propertyDictionary
	if [[ -v propertyDict[${array[0]}] ]]; then
		# If key is in propertyDictionary, then increment value
		propertyDict[${array[0]}]=$((++propertyDict[${array[0]}]))
	else
		errorList+=("Key: Invalid, ${array[0]}")
	fi

	# Log the imagePath and ExecutableName
	if [ ${array[0]} == "image" ]; then
		imagePath=${array[1]}
	elif [ ${array[0]} == "executable" ]; then
		executableName=${array[1]}
	fi
# Closing the file
done < "$file"

#print propertyDictionary keys and values
echo "Occurrences of keys in config.txt file:"
for key in "${!propertyDict[@]}"; do
	echo "$key: ${propertyDict[$key]}"
	# If value is equal to one
	if [ ${propertyDict[$key]} != 1 ]; then
		errorList+=("Key: ${key}, invalid number of occurences, ${propertyDict[$key]}")
	fi
done

echo ""

echo "Checking if image and executable paths are valid:"
# Checking if image and executable paths are valid
if [ -f "$basePath$imagePath" ]; then
	echo "Image file exists"
else
	echo "Image file does not exist"
	errorList+=("Config: Image file does not exist")
fi

if [ -f "$basePath$executableName" ]; then
	echo "Executable file exists"
else
	echo "Executable file does not exist"
	errorList+=("Config: Executable path doesn't match")
fi

# Get the length of the errorList
 if [[ ${#errorList[@]} > 0 ]]; then
	echo ""
 	echo "Errors encountered: ${#errorList[@]}"
	for error in "${errorList[@]}"; do
		echo $error
	done
	exit 1
 fi
