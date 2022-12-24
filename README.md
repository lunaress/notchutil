# Synopsis
       notchutil [-hvlt] [-f] <model>

# Description
       notchutil is a simple tool to switch between the default and a modified
       resolution with the intent to hide the notch on Apple's  newer  MacBook
       models.

# Options
       notchutil does not require any options. Your MacBook should be detected
       and have its resolution configured accordingly. However,  there  are  a
       handful of optional arguments. It works by shaving down the height to a
       size right below the notch and sets that as the system resolution.

       -h, --help           Prints a Help text

       -v, --version        Shows information about notchutil

       -l, --list           List all MacBooks supported

       -t, --temporary      Only configure the mode for the session

       -f, --force <model>  Force a specific MacBook model from the list


# Bugs
       Bugs can occur when notchutil is invoked while an external  display  is
       connected.


# Example
<img width="1512" alt="example" src="https://user-images.githubusercontent.com/121362066/209452105-23ea083c-2187-4366-b61a-0d0841fa3cbe.png">

The new resolution is just snuggly below the notch, just like if you were to fullscreen into an app.
To revert this, simply run the program again or tick "Default for Display" resolution under Display in System Preferences.
