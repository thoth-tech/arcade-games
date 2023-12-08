# Guide To Contribute

This document will serve as an overview of the documentation for the Single Combat Game - the first point of call for questions about how to contribute. It contains information about the project, standards to follow, and where to look for further documentation.

## Workflow for Contributing - MUST READ

1. Make sure you have read any relevant documentation to the classes or part of the game that you are working on.

2. Keep in communication with the team about what you are working on and don't be afraid to ask for help if stuck with code or with documentation

3. Check the BugsAndIdeasLog.md document to see if there's anything that may affect you current work.

4. Write your code. Leave comments and remember to make commits.

5. If you make any design decisions, document them in the documentation folder when they happen so that you don't forget. The ClassDescriptionsAndDecisions.md file is particularly useful for information about the development of each class.

## Standards to follow - MUST READ

- Use camelCase for created methods and attributes. The only exception to this is Class names which use PascalCase.

- Always use `#pragma once` at the beginning of .h header files to ensure that #includes are only included once (this functions similar to #ifndef).

- Always use [shared pointers](https://en.cppreference.com/w/cpp/memory/shared_ptr) rather than regular C++ pointers. This will take care of garbage collection.

- Use [Vectors](https://cplusplus.com/reference/vector/vector/) rather than regular C++ arrays.

- **DO NOT** use `using namespace std;` by itself. This is to avoid namespace pollution. If required, include the specific names you require. E.g. `using std::cout;`

- Each Class should have it's own .h header file where methods and attributes are declared (but generally not defined, with the exception of constants). Most Classes should also have a .cpp where methods and attributes are then defined (the actual code).

- Please do your best to leave descriptive comments in your code, commit regularly with descriptive messages, and document important design decisions (refer to the 'ClassDescriptionsAnDecisions' markdown file).

- A lot of the game relies on specific dimensions for the board, blocks and window size. Please keep this in mind if any changes need to be made to any sizes, especially when expanding to 2-player.

## Compiling and Debugging

For debugging code, especially that which may remain in the game files after initial testing, use #ifdef and #endif directives. e.g:

```C++
#ifdef DEBUGSC
//your debug code here
#endif
```

You can then include the debug code when compiling by running: `skm clang++ -DDEBUGSC *.cpp -o SingleCombat`

You can still compile the game normally without the debug code by running the regular compile command: `skm clang++ *.cpp -o SingleCombat`

## Game Design

There is a folder for Game Design documentation in the Documentation folder for this game. Please use the files there to document decisions, suggestions, notes and anything else related to the game design and features. **DO NOT** edit the original game proposal document.

## Object Oriented Design

We are taking an Object Oriented approach to this game. The aim is to encapsulate the code into classes that can be reused as objects.

We want to keep functions as simple as possible to make the code as easy to read and reuse as possible. If you find you have a large function, it may be a sign that it needs to be broken up into other functions or worth considering if it can be handled in another class.

Knowledge of attributes should be kept as local as possible. Please always consider the necessity of getter and setter functions. Obviously there will be times when they are required, but there are also instances where it might be that the attributes or the methods using them are being handled by the wrong class.

## UML Diagrams

The nature of this project is Agile and as such, the UML diagram will change as the game develops. There is a UML Diagrams folder in the Documentation folder for this game. If a new version is made, please include a date in the file name.

## SplashKit

Part of the purpose of this game is to showcase what SplashKit can do. In saying that, it is useful to limit which classes need to access SplashKit methods directly. This can save time in future if there are updates to SplashKit, as it will only affect some files.

There are times where SplashKit methods may not function exactly as needed and it may be necessary to write a new function. In cases where there has been an intentional decision to create a new function rather than SplashKit's one, it would be good to note the reasoning in a comment, or otherwise document it. This may be useful in future to the SplashKit Expansion team.
