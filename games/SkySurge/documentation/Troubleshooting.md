# Troubleshooting Sky Surge

## Errors launching the game

Some students have had difficulties with either getting Sky Surge to run, or getting Sky Surge to run without errors.

The below steps may be helpful in resolving these problems.

### General

* Unlike most of the arcade games, Sky Surge is written in C# rather than C++, so instead of using a command like `skm clang++ *.cpp -o game`, you'll instead need to use `skm dotnet run`.
* Sky Surge contains both a `.sln` and `.csproj` file. In some system configurations, you may find that you need to explicitly target one of these instead of the (implicit) automatic choice. Remember that you can surround file names in quotes as needed if they contain spaces, e.g., `"Sky Surge.sln"`. However, normally you shouldn't need to do this, and if you do it may be a sign of a problem somewhere else.

### MinGW / MSYS2 users on Windows

* Many MSYS2 terminal options are available. It's typically recommended to use the **MINGW64** option for 64-bit operating systems, and the **MINGW32** Option for 32-bit operating systems.
* Using the terminal built into your IDE (e,g., Visual Studio or Visual Studio Code) to build/run the game can sometimes cause problems that can be solved by using the appropriate MSYS2 terminal instead.
* If you find that you're able to build/run the game with less errors when using PowerShell or Command Prompt (as compared to using the appropriate MSYS2 terminal option for your system), it's possible that your MinGW installation is corrupted somehow. Reinstalling MinGW might resolve this.
