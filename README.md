# notchutil
Easily "remove" the notch on any MacBook with notchutil!



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


