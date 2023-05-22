import sys, os, subprocess, hashlib, vernam, asyncio

def getKey():

    handle = open('key.txt', 'r')
    key = handle.read()    
    handle.close()

    return key

def getPaths():

    handle = open('.paths.txt', 'r')
    paths = handle.read()    
    handle.close()
    paths = paths.split(',')

    return paths


def decryptKey(key):

    default = ".home/user/encryption/testing"
    key = vernam.encryptData(default,key)

    return key


def confirmSplaskit(key, paths):

    for i in range(dependenies):
        if os.path.exists(vernam.encryptData(key,paths[0])+os.getlogin()+vernam.encryptData(key,paths[1])) :
            file1.write(paths[1] + "True")
        else:
            file1.write(paths[1] + "False")



def confirmDotnet(key,paths):
    if os.path.exists(vernam.encryptData(key,paths[0])+os.getlogin()+vernam.encryptData(key,paths[2])) :
        return True
    return False

async def installSplashkit():

    print("Installing Splaskhit Manager...")
    os.system("bash installer.sh")
    os.system("skm")

    print("Installation completed...")

    return True

async def installSplashLibrary():

    print("")
    print("---------------")
    print("Installing Splashkit libraries...")
    os.system("skm linux install")
    print("Installation completed...")

    return True

async def installDotnet():

    print("")
    print("---------------")
    print("Installing Dotnet...")
    os.system("curl -sSL https://dot.net/v1/dotnet-install.sh | bash /dev/stdin --channel STS")
    os.system("echo 'export PATH=$PATH:$HOME/.dotnet' >> ~/.bashrc")
    os.system("")
    os.system("source ~/.bashrc")
    print("Installation completed...")

    return True





async def Main():

    key = getKey()
    key = decryptKey(key)
    paths = getPaths()
    splashkit = confirmSplaskit(key,paths)
    dotnet = confirmDotnet(key,paths)
    splashInstall = await installSplashkit()
    if not splashInstall:
        await installSplashLibrary()
    if not dotnet:

    
    print("")
    print("---------------")
    print("Shut down application now...")


    


if __name__=='__main__':
    asyncio.run(Main())


