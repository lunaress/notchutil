/*
 *
 * notchtutil
 * author: lunaress
 * website: https://lunaress.moe
 *
 * notchutil is a tool to quickly and easily "remove" the notch on any macbook that features one
 *
 */

#include <iostream>
#include <getopt.h>
#include <sys/sysctl.h>
#include <CoreGraphics/CoreGraphics.h>



const char* programVersion = "0.7";
bool isForcingMac = false;
char* forcedMac;
CGConfigureOption configurationLength = kCGConfigurePermanently;



struct {
    std::string macModel;
    bool isMacBook; // currently unused
}thisMac;



static struct {
    const char* mbp2021a = "MacBookPro18,3"; // 14"
    const char* mbp2021b = "MacBookPro18,4"; // 16"
    const char* mba2022 = "Mac14,2"; // Air
}macTypes;



static struct {
    const int mbp2021a_w_d = 1512; // 14"
    const int mbp2021a_h_d = 982;

    const int mbp2021a_h_m = 945; // Modified
    
    
    const int mbp2021b_w_d = 1728; // 16"
    const int mbp2021b_h_d = 1117;
    
    const int mbp2021b_h_m = 1080;
    
    
    const int mba2022_w_d = 1280; // Air
    const int mba2022_h_d = 832;
    
    const int mba2022_h_m = 800;
}macResolutions;



const char* helpText =
            "usage: notchutil [-hvlt] [-f]\n\
            \r\t-h, --help\t\tprints this help text\n\
            \r\t-v, --version\t\tshows information about this program\n\
            \r\t-l, --list\t\tlists all currently available macbooks\n\
            \r\t-t, --temporary\t\tonly configure for session\n\
            \r\t-f, --force <model>\tforces a specific notch macbook model\n\n\
            \r\texample: notchutil --temporary --force MacBookPro18,4\n\n";



const std::string versionText =
             "notchutil version " + (std::string)programVersion + "\n\
             \rauthor: lunaress\n\
             \rwebsite: https://lunaress.moe\n\n";



const char* validMacBooksText =
            "valid models:\t\tname:\n\
            \rMacBook Pro 2021 14\"\tMacBookPro18,3\n\
            \rMacBook Pro 2021 16\"\tMacBookPro18,4\n\
            \rMacBook Air 2022\tMac14,2\n\n";



bool isCompatibleMac(char* input) {
    if ((strcmp(input, macTypes.mbp2021a)) != 0 &&
        (strcmp(input, macTypes.mbp2021b)) != 0 &&
        (strcmp(input, macTypes.mba2022)) != 0)
        return false;
    
    return true;
}



static struct option long_options[] = {
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'v'},
    {"list", no_argument, NULL, 'l'},
    {"force", required_argument, NULL, 'f'},
    {"temporary", no_argument, NULL, 't'},
    {NULL, 0, NULL, 0}
};



int main(int argc, const char* argv[]) {
    int ch;
    opterr = 0;
    
    while ((ch = getopt_long(argc, (char* const*)argv, (char*)"hvltf:", long_options, NULL)) != -1) {
        switch (ch) {
            case 'h':
                printf("%s", helpText);
                return 0;
            case 'v':
                printf("%s", versionText.c_str());
                return 0;
            case 'l':
                printf("%s", validMacBooksText);
                return 0;
            case 'f':
                forcedMac = optarg;
                if (!isCompatibleMac(forcedMac)) {
                    printf("notchutil: invalid model: %s\n", forcedMac);
                    printf("%s", validMacBooksText);
                    return 1;
                }
                
                printf("notchutil: forcing model: %s\n", forcedMac);
                isForcingMac = true;
                break;
            case 't':
                printf("notchutil: configuring for session\n");
                configurationLength = kCGConfigureForSession;
                break;
            default:
                if (optind <= 1)
                    ++optind;
                printf("notchutil: invalid argument: %s\n", argv[optind-1]);
                printf("%s", helpText);
                return 1;
        }
    }
    
    
    
    size_t len = 0;
    
    sysctlbyname("hw.model", nullptr, &len, NULL, 0);
    std::string model(len, '\0');
    sysctlbyname("hw.model", const_cast<char*>(model.data()), &len, NULL, 0);

    thisMac.macModel = model;
    
    if (thisMac.macModel.find("MacBook") != std::string::npos)
        thisMac.isMacBook = true;
    
    
    if ((!isCompatibleMac(thisMac.macModel.data())) && (isForcingMac == false)) {
        printf("notchutil: this mac is unsupported. continue? [y/n]");
        
        char input = getchar();
        switch (toupper(input)) {
            case 'N':   return 0;
            case 'Y':   break;
            default:    return 0;
          }
    }


    
    int currentWidth = NULL;
    int currentHeight = NULL;
    
    int newWidth = NULL;
    int newHeight = NULL;
    
    char* currentGivenMac;
    
    CGDirectDisplayID display = CGMainDisplayID();
    CGDisplayModeRef currentMode = CGDisplayCopyDisplayMode(display);
    
    currentWidth = (int)CGDisplayModeGetWidth(currentMode);
    currentHeight = (int)CGDisplayModeGetHeight(currentMode);
    
    CGDisplayModeRelease(currentMode);

    
    // may god have mercy on my soul
    
    if (isForcingMac)
        currentGivenMac = forcedMac;
    else
        currentGivenMac = thisMac.macModel.data();
    
    
    if ((strcmp(currentGivenMac, macTypes.mbp2021a)) == 0) { // 14"
        newWidth = macResolutions.mbp2021a_w_d;
        newHeight = macResolutions.mbp2021a_h_d;
        
        if ((newHeight == currentHeight) && (newWidth == currentWidth))
            newHeight = macResolutions.mbp2021a_h_m;
    }
    else if ((strcmp(currentGivenMac, macTypes.mbp2021b)) == 0) { // 16"
            newWidth = macResolutions.mbp2021b_w_d;
            newHeight = macResolutions.mbp2021b_h_d;
        
            if ((newHeight == currentHeight) && (newWidth == currentWidth))
                newHeight = macResolutions.mbp2021b_h_m;
    }
    else if ((strcmp(currentGivenMac, macTypes.mba2022)) == 0) { // Air
            newWidth = macResolutions.mba2022_w_d;
            newHeight = macResolutions.mba2022_h_d;
        
            if ((newHeight == currentHeight) && (newWidth == currentWidth))
                newHeight = macResolutions.mba2022_h_m;
    }

    
    
    CFStringRef keys[1] = {kCGDisplayShowDuplicateLowResolutionModes};
    
    CFBooleanRef values[1] = {kCFBooleanTrue};
    
    CFDictionaryRef options = CFDictionaryCreate(kCFAllocatorDefault, (const void**)keys, (const void**)values, 1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    
    CFArrayRef modes = CGDisplayCopyAllDisplayModes(display, options);
    
    CGDisplayModeRef newMode = NULL;
    const CFIndex count = CFArrayGetCount(modes);
    
    for (CFIndex i = 0; i < count; ++i) {
        const CGDisplayModeRef mode = (CGDisplayModeRef) CFArrayGetValueAtIndex(modes, i);
        const size_t tWidth = CGDisplayModeGetWidth(mode);
        const size_t tHeight = CGDisplayModeGetHeight(mode);
        
        if (tWidth == newWidth && tHeight == newHeight) {
            newMode = CGDisplayModeRetain(mode);
            break;
        }
    }
    
    CFRelease(modes);

    CGDisplayConfigRef config;
    
    if (CGBeginDisplayConfiguration(&config) != kCGErrorSuccess) {
        printf("notchutil: failed to begin display configuration\n");
        return 1;
    }
    
    if (CGConfigureDisplayWithDisplayMode(config, display, newMode, NULL) != kCGErrorSuccess) {
        printf("notchutil: failed to configure display with new mode\n");
        return 1;
    }

    if (CGCompleteDisplayConfiguration(config, configurationLength) != kCGErrorSuccess) {
        printf("notchutil: failed to complete display configuration\n");
        return 1;
    }
    
    printf("notchutil: successfully changed resolution from %dx%d to %dx%d\n\n", currentWidth, currentHeight, newWidth, newHeight);

    return 0;
}
