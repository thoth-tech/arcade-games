import os
from requests import get
from zipfile import ZipFile
from io import BytesIO
from shutil import move, rmtree
from tarfile import open

#Get the zip
git_url = "https://github.com/thoth-tech/arcade-games/archive/refs/heads/main.zip"
r = get(git_url)
z = ZipFile(BytesIO(r.content))
z.extractall(r"/home/deakin/Games/LaunchScripts/")

#move to games folder
sourceDir = r"/home/deakin/Games/LaunchScripts/arcade-games-main/compiled-games/"
gameFolder = r"/home/deakin/Games/LaunchScripts/"
for file in os.listdir(sourceDir):
    src_path = os.path.join(sourceDir, file)
    dst_path = os.path.join(gameFolder, file)
    move(src_path, dst_path) 
    
#Remove the cloned repo file
rmtree(r"/home/deakin/Games/LaunchScripts/arcade-games-main/")
#Extract games
for folder in os.listdir(gameFolder):
    if os.path.isdir(gameFolder+folder):
        for file in os.listdir(gameFolder+folder):
            filePath = gameFolder+folder+"/"+file
            if os.path.isdir(filePath):
                continue 
            if ".tar.gz" in file:
                try:
                    print("Extracting: " + file)
                    tar = open(filePath)
                    tar.extractall(gameFolder+folder+"/") 
                    tar.close() 
                    #Remove the old tar file
                    os.remove(filePath)
                except:
                    print("Failed to decompress " + file)
