import json
from posixpath import dirname
import sys
from packaging import version
import platform 
from collections import defaultdict
from pathlib import Path
import shutil
import tarfile
import requests 
import os
import stat

# Path to the file where updater stores the info on last checked release etc.
# The file MUST be writable
# Overwritten with --updater-info-file
updater_info_path = "./updater.json"
# Where to put games, can be overridden via --destination-path
dest_path = '/tmp/.games'
# Where to put launch scripts, overwritten via --launch-scripts-directory
launch_scripts_dir = '/tmp/.arcade-games-launch-scripts'
# Github repository to check for updates. Format: "owner/repo"
GITHUB_REPO = "thoth-tech/arcade-games"
LAUNCHSCRIPT_SUFFIX = '-launchscript.sh'

def clear_file(fd):
    fd.truncate(0)
    fd.seek(0)
    

# Downgrading may be allowed with --allow-downgrade.
allow_downgrading = False

# This may be allowed with --allow-removing-destination-path
# Allows to remove the destination path if it is a file.
allow_removing_destination_path = False

def profile_platform(platform_title: str):
    if platform_title == 'linux':
        return "linux"
    elif platform_title == 'win32':
        return "win"
    elif platform_title == 'macos':
        return "macos"
    else:
        return None

# We are accepting everything compiled for 32-bit from repo
def profile_architecture(architecture_title: str):
    arch = platform.machine().lower()

    if "aarch64" in arch or "arm64" in arch:
        return "aarch64"
    elif "arm" in arch:
        return "arm"
    elif "x86_64" in arch or "amd64" in arch or "x64" in arch:
        return "amd64"
    elif "i386" in arch or "i686" in arch or "x86" in arch:
        return "x86"
    else:
        return None

def file_readable_to_updater(file_name: str):
    # TODO: for Win32, this should be changed to bat
    return file_name.endswith('.tar.gz') or file_name.endswith('.sh')

# downloads the given archive and puts it under the given name into destination_path
def download_and_unpack(file_name: str, url: str, destination_path: str|Path):
    if file_name.endswith('.tar.gz'):
        try:
            response = requests.get(url, stream=True)
            if response.status_code == 200:
                    with tarfile.open(fileobj=response.raw, mode='r:gz') as tar:
                        tar.extractall(path=destination_path)
            else:
                sys.stderr.write("Error: failed to download from the url " + url + ". Status code: " + str(response.status_code) + "\n")

        except ConnectionError:
            sys.stderr.write("Failed to connect to the internet. No updates have been pulled. The old game might have been deleted.")
            exit(14)
    else:
        sys.stderr.write(f"Error: {file_name} is not a .tar.gz file and hence may not be unpacked here")

# downloads the given file into the destination path, changes ${GAME_DIR} to the given one, gives the file the run privilege
def download_launch_script(file_name: str, url: str, destination_path: str|Path, game_dir: str|Path):
    try:
        response = requests.get(url, stream=True)
        if response.status_code == 200:
            update_script_txt = response.text.replace("${GAME_DIR}", str(game_dir))
            update_script_dest_path = destination_path / file_name 
            # tries to delete the file, ignores if it never existed anyway
            try: 
                update_script_dest_path.unlink()
            except FileNotFoundError:
                pass 
            with open(update_script_dest_path, "w+") as script_fd:
                script_fd.write(update_script_txt)
            # these are chmod +x
            new_mode = update_script_dest_path.stat().st_mode | stat.S_IXUSR
            update_script_dest_path.chmod(new_mode)
        else:
            sys.stderr.write("Error: failed to download from the url " + url + ". Status code: " + str(response.status_code) + "\n")

    except ConnectionError:
        sys.stderr.write("Failed to connect to the internet. No updates have been pulled. The old game might have been deleted.")
        exit(15)


# unpacks the .tar.gz's and puts the scripts in the respective folder
def unpack_games(available_games, file_contents, destination_path, launch_scripts_dir):
    for game_title, available_files in available_games.items():
        launch_script = available_files.get('launchscript')
        if launch_script == None:
            # no launch script - don't touch the whole game, as the machine can't work with it
            pass

        # at this point, the launchscript is definitely there
        game_directory = destination_path / game_title
        download_launch_script(launch_script["filename"],launch_script["url"], launch_scripts_dir, game_directory)

        # deal with assets & executable
        dir_recreated = [False]
        def recreate_dir(dir_recreated): # recreates the dir for the current game_title
            dir_recreated[0] = True
            if game_directory.exists():
                shutil.rmtree(game_directory)
            game_directory.mkdir(parents=True)

        # check if there are assets and unpack
        assets_file = available_files.get('assets')
        if assets_file != None:
            if not dir_recreated[0]: recreate_dir(dir_recreated)
            # untar the respective file and push into the game directory
            download_and_unpack(assets_file['filename'], assets_file['url'], game_directory)
            pass

        # check if there are executables and unpack
        executable_file = available_files.get('executable')
        if executable_file != None:
            if not dir_recreated[0]: recreate_dir(dir_recreated)
            # untar the respective file and push into the game directory
            download_and_unpack(executable_file['filename'], executable_file['url'], game_directory)
            pass

        
        for file_type, file_name in available_files.items():
            pass

# the main function here - checks the repo. Downloads the new files, if required
def check_updates(fd, destination_system, destination_architecture, destination_path, allow_downgrading):
    file_contents = {}
    try:
        json_contents = json.load(fd)
        if type(json_contents) != dict: 
            raise json.JSONDecodeError("Invalid content in the updater info file. Expected a JSON object.", doc=str(json_contents), pos=0)
        file_contents = json_contents
    except json.JSONDecodeError as e:
        # Invalid content, treat it as if it isn't there 
        sys.stderr.write(f"Warning: invalid content in the updater info file. Treating it as if it isn't there. File path: {updater_info_path.__str__()}\n")
        # Clearing and initializing the file
        clear_file(fd)
        fd.write("{}")

    try:
        response = requests.get("https://api.github.com/repos/" + GITHUB_REPO + "/releases/latest");
        release_info = response.json()
    except ConnectionError:
        sys.stderr.write("Failed to connect to the internet. No updates have been pulled.")
        exit(13)


    # destination path is a file. We don't want to remove it unless required.
    if os.path.isfile(destination_path):
        if allow_removing_destination_path:
            try:
                os.unlink(destination_path)
            except PermissionError as e:
                sys.stderr.write("Attempted to remove the file, but did not have permissions. Refusing to continue.\n")
                exit(6)
        else:
            sys.stderr.write("The destination path is a file. Refusing to continue.\n")
            exit(5)

    try:
        # Compare the version of the last release with the version of currently installed app. 
        remote_version = version.parse(release_info["tag_name"])
        # get the current version, default to "v0"
        local_version = version.parse(file_contents.get("last_checked_release_id", "v0"))
        if local_version > remote_version:
            if allow_downgrading:
                sys.stderr.write("Warning: the version of currently installed app is higher than the version of last release.\n")
            else:
                sys.stderr.write("Error: the version of currently installed app is higher than the version of last release. No downgrading allowed. Local version: " + str(local_version) + ", remote version: " + str(remote_version) + "\n")
                sys.stderr.write("Error: downgrading may be allowed with --allow-downgrade\n")
                exit(3)
        elif local_version == remote_version:
            sys.stderr.write("The version of currently installed app is the same as the version of last release. No update needed.\n")
            exit(0)

        available_games = defaultdict(dict)

        file_contents["last_checked_release_id"] = release_info["tag_name"]
        if file_contents.get("files") is None:
            file_contents["files"] = {}

        # We got here means we take the new release
        for asset in release_info["assets"]:
            if not file_readable_to_updater(asset["name"]):
                sys.stderr.write(f"Warning: updater does not work with file type: {asset["name"]}.\n")
                continue

            # unloading the filename into variables, so we can work with it
            first_dot = asset["name"].find(".") # never -1, because it's either .tar.gz or .sh
            file_name = asset["name"][:first_dot] or ""
            file_name_ext = asset["name"][first_dot:] or ""
            filename_segments = file_name.split("-") or [None]

            game_title = filename_segments[0]

            if filename_segments[1] == 'assets' and file_name_ext == '.tar.gz':
                # enqueue this file for unpacking always
                if available_games.get(game_title) == None:
                    available_games[game_title] = {}
                available_games[game_title]["assets"] = {"filename": asset["name"], "url": asset["browser_download_url"]}
                continue
            elif filename_segments[1] == dest_platform and filename_segments[2] == dest_arch and file_name_ext == '.tar.gz':
                if available_games.get(game_title) == None:
                    available_games[game_title] = {}
                available_games[game_title]["executable"] = {"filename": asset["name"], "url": asset["browser_download_url"]}
                continue 
            # TODO: change extension to .bat for win32
            elif filename_segments[1] == 'launchscript' and file_name_ext == '.sh':
                if available_games.get(game_title) == None:
                    available_games[game_title] = {}
                available_games[game_title]["launchscript"] = {"filename": asset["name"], "url": asset["browser_download_url"]}
                continue 
            else:
                sys.stderr.write(f"Warning: file is not eligible for this machine: {asset["name"]}.\n")
                continue


        # we have collected all available games by this point
        pass
            

        # Create the directory for games if it does not exist yet
        if not os.path.isdir(destination_path):
            try:
                Path(destination_path).mkdir(parents=True)
            except PermissionError:
                sys.stderr.write(f"Could not proceed with creating directory in {destination_path}. Refusing to proceed.\n")
                exit(6)

        if launch_scripts_dir.exists():
            if not launch_scripts_dir.is_dir():
                # exists, but not a directory

                sys.stderr.write(f"{launch_scripts_dir} is not a directory. Refusing to proceed.\n")
                exit(16)

            # exists, is a dir
        else:
            # does not exist, so we create one
            launch_scripts_dir.mkdir(parents=True)

        # Unpack available games and assets for the given platform
        unpack_games(available_games, file_contents, destination_path, launch_scripts_dir)

        clear_file(fd)
        json.dump(file_contents, fd)
    except KeyError as e:
        sys.stderr.write(f"Error: could not get last release from the GH Releases. Please ensure that repository https://github.com/{GITHUB_REPO}/ exists and has at least one current release.\n")
        exit(17)





try:
    # TODO: read argument from the input
    dest_platform = profile_platform(sys.platform)
    if dest_platform is None:
        sys.stderr.write("Error: unsupported platform. Please use one of linux, win, macos\n")
        exit(18)

    dest_arch = profile_architecture(platform.machine())
    if dest_arch == None:
        sys.stderr.write("Error: unsupported platform. Please use one of arm, arm64, x86, x64\n")
        exit(8)

    for arg in sys.argv:
        if arg == "--allow-downgrade":
            allow_downgrading = True
            sys.stderr.write("Warning: Downgrading allowed by the command.\n")
            continue

        dest_path_prefix = '--destination-path='
        if arg.startswith(dest_path_prefix):
            maybe_dest_path = arg[len(dest_path_prefix):]
            if len(maybe_dest_path) == 0:
                sys.stderr.write("Error: destination path has not been entered\n")
                exit(9)
            dest_path = maybe_dest_path

        updater_info_file_prefix = '--updater-info-file='
        if arg.startswith(updater_info_file_prefix):
            maybe_updater_info_file = arg[len(updater_info_file_prefix):]
            if len(maybe_updater_info_file) == 0:
                sys.stderr.write("Error: updater info file path has not been entered\n")
                exit(10)
            updater_info_path = maybe_updater_info_file

        launch_scripts_prefix = '--launch-scripts-directory='
        if arg.startswith(launch_scripts_prefix):
            maybe_launch_scripts_dir = arg[len(launch_scripts_prefix):]
            if len(maybe_launch_scripts_dir) == 0:
                sys.stderr.write("Error: updater info file path has not been entered\n")
                exit(12)
            launch_scripts_dir = maybe_launch_scripts_dir

    updater_info_path = Path(dirname(__file__)).joinpath(updater_info_path).resolve()
    dest_path = Path(dirname(__file__)).joinpath(dest_path).resolve()
    launch_scripts_dir = Path(dirname(__file__)).joinpath(launch_scripts_dir).resolve()

    f = None
    try:
        if not os.path.isfile(updater_info_path):
            Path(updater_info_path).touch()
        with open(updater_info_path, "r+") as f:
            check_updates(f, destination_system=dest_platform, destination_architecture=dest_arch, destination_path=dest_path, allow_downgrading=allow_downgrading)
    except PermissionError as e:
        sys.stderr.write("Error: no permission to write to the updater info file. File path: " + updater_info_path + "\n")
        exit(2)
    except KeyboardInterrupt as e:
        sys.stderr.write('Interrupting... ')
        try:
            # close the file in case of being here
            f.__exit()
        except Exception:
            pass
        exit(99)

    exit(0)
except KeyboardInterrupt as e:
    sys.stderr.write('Interrupting... Some files may not have been closed properly')
    exit(99)