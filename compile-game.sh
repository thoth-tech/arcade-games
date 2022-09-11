GAME_DIR=$1
BINARY_SUFFIX=$2             

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
	# If key is compile-command
	if [[ ${item[0]} == "compile-command" ]]; then
		# Store the value in command
		command="${item[1]}"
		break
	fi
done < $file

# Get the game name
game_name=$(basename $GAME_DIR)
echo Compiling game $game_name...

# If command is empty
if [[ -z "$command" ]]; then
	echo "No compile command found, using default"
	skm g++ program.cpp -o ${game_name}-$BINARY_SUFFIX
else
	# If command starts with skm
	if [[ $command == "skm"* ]]; then
	echo Appending output flag and name/loc 
	command+=" -o ${game_name}-$BINARY_SUFFIX"
	else
	echo Assuming usage of makefile, appending output name/loc 
	command+=" ${game_name}-$BINARY_SUFFIX"
	fi
	echo "Running compile command: $command"
	eval $command
fi  