<h2 align="center">
notchutil is a simple tool to switch between the default and a modified resolution with the intent to hide the notch on Apple's newer MacBook models.
</h2>

## Running the downloaded binary
<b>macOS 12.0 Monterey and later.</b><br>
Using your terminal emulator of choice, simply run the executable like usual:

       ./notchutil


If macOS does not recognize the file as a Unix binary, run

       chmod +x notchutil

to flag the file as executable.

<b>Note:</b> It is also possible to just click the file twice and have it run through Finder



## Compiling
Simply create a new Command Line Tool project in Xcode. Make sure to select C++ as the project language.<br>
It is important that you set the language dialect to ISO C++17 in the project settings.

You also want to tell the linker to include CoreFoundation and CoreGraphics under Build Phases.
<img width="1237" alt="linker" src="https://user-images.githubusercontent.com/121362066/209615301-9ec1b0b0-24d3-46b1-aca1-7fd5226f47e4.png">


Keep in mind that you must have some form of signature, at least to run locally. Otherwise it will not compile.

## Synopsis
       notchutil [-hvlt] [-f] <model>

## Options
       notchutil does not require any options. Your MacBook should be detected
       and have its resolution configured accordingly. However,  there  are  a
       handful of optional arguments. It works by shaving down the height to a
       size right below the notch and sets that as the system resolution.

       -h, --help           Prints a Help text

       -v, --version        Shows information about notchutil

       -l, --list           List all MacBooks supported

       -t, --temporary      Only configure the mode for the session

       -f, --force <model>  Force a specific MacBook model from the list


## Bugs
Bugs can occur when notchutil is invoked while an external display is connected.


## Example
<img width="1512" alt="example" src="https://user-images.githubusercontent.com/121362066/209452105-23ea083c-2187-4366-b61a-0d0841fa3cbe.png">

The new resolution is just snuggly below the notch, just like if you were to fullscreen into an app.
To revert this, simply run the program again or tick "Default for Display" resolution under Display in System Preferences.

## License
notchutil is released under the <a href="https://github.com/lunaress/notchutil/blob/main/LICENSE">GNU General Public License v3.0</a>.
