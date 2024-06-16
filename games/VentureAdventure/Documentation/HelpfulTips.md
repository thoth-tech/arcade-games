# Helpful Tips

## Debugging Output

There is a variable called **debugging_output_enabled** in the **program.cpp** file. If you set this to *true*, debugging messages will appear in console. If you'd like to adjust these messages to better suit your needs, the code can be located in the **game.cpp** file in the **get_verbose_debugging_message** method.

## Easier Level Testing

- In **game.cpp** there is a variable called **debugeasymode**. This can be set to *true* to make levels only generate one gem, making them easier to complete. Remember to change this back before committing anything.

- In **program.cpp** there is a variable called **startinglevel**. You can change this to a different number to make that level the first level. Remember to change this back before committing anything.

## Change Enemy Speed

To change the enemy speed, refer to the **/Resources/animations/spider.txt** file. Spider movement is controlled by the 'vectors' section. The 2nd and 3rd numbers indicate the x and y change per frame for the different animations.

## Adding New Resources

Make sure to edit the **/Resources/bundles/icon.txt** file if you are adding new assets such as images, sound effects or animations to the game. This file also includes art credits.
