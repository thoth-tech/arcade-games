# Updater description

## Releases
Updater analyses the Releases section of the repository. Each game, to be downloaded by the updater, should have:

- ${GAME_NAME}-${OS_NAME}-script.sh (__required__, file type on the OS) - goes into LaunchScripts folder
- ${GAME_NAME}-${OS_NAME}-${ARCHITECTURE}.tar.gz (_optional_) - files to be put in the game's directory - OS and architecture specific
- ${GAME_NAME}-gamelists-entry.xml (_optional_) - entry for gamelists, must be an XML file - _this feature is not developed_
- ${GAME_NAME}-assets.tar.gz (_optional_) - files to be put into the game's directory, non-OS specific

### Packing of the releases
The game will be unpacking everything that is put in the release directly under the directory with the same name. In other words, this would be the script to pack exactly the contents of a given game titles `${GAME_NAME}`
```sh
cd ./${GAME_NAME}-linux-aarch64
tar -czvf ../${GAME_NAME}-linux-aarch64.tar.gz .
cd ..
```

### Run script
As the user may change any directory settings, denote game's directory as `${GAME_DIR}`. Run script will be put into the respective directory