/*
 *
 * notchtutil 0.8
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



const char* programVersion = "0.8";
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



const char* helpText =
            "usage: notchutil [-hvlt]\n\
            \r\t-h, --help\t\tprints this help text\n\
            \r\t-v, --version\t\tshows information about this program\n\
            \r\t-l, --list\t\tlists all currently available macbooks\n\
            \r\t-t, --temporary\t\tonly configure for session\n\
            \r\texample: notchutil --temporary\n\n";



const std::string versionText =
             "notchutil version " + (std::string)programVersion + "\n\
             \rauthor: lunaress\n\
             \rwebsite: https://lunaress.moe\n\n";



const char* validMacBooksText =
            "valid models:\t\tname:\n\
            \rMacBook Pro 2021 14\"\tMacBookPro18,3\n\
            \rMacBook Pro 2021 16\"\tMacBookPro18,4\n\
            \rMacBook Air 2022\tMac14,2\n";



std::tuple<int, int, int> getResolution(char* model) {
    // default height, default width, modified height for fitting the resolution below the notch
    if (strcmp(model, macTypes.mbp2021a) == 0) return std::make_tuple(1512, 982, 945);
    if (strcmp(model, macTypes.mbp2021b) == 0) return std::make_tuple(1728, 1117, 1080);
    if (strcmp(model, macTypes.mba2022) == 0) return std::make_tuple(1280, 832, 800);
    
    printf("notchutil: invalid model: %s\n", model);
    printf("%s", validMacBooksText);
    exit(1);
}



static struct option long_options[] = {
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'v'},
    {"list", no_argument, NULL, 'l'},
    {"temporary", no_argument, NULL, 't'},
    {NULL, 0, NULL, 0}
};




int main(int argc, const char* argv[]) {
    size_t len = 0;
    
    sysctlbyname("hw.model", nullptr, &len, NULL, 0);
    std::string model(len, '\0');
    sysctlbyname("hw.model", const_cast<char*>(model.data()), &len, NULL, 0);

    thisMac.macModel = model;
    
    if (thisMac.macModel.find("MacBook") != std::string::npos)
        thisMac.isMacBook = true;
    
    
    int ch;
    opterr = 0;
    
    while ((ch = getopt_long(argc, (char* const*)argv, (char*)"hvlt", long_options, NULL)) != -1) {
        switch (ch) {
            case 'h':
                printf("%s", helpText);
                return 0;
            case 'v':
                printf("%s", versionText.c_str());
                return 0;
            case 'l':
                printf("%s\nyour mac: %s\n\n", validMacBooksText, thisMac.macModel.c_str());
                return 0;
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
    
    
    
    int currentWidth = NULL;
    int currentHeight = NULL;
    
    int newWidth = NULL;
    int newHeight = NULL;
        
    CGDirectDisplayID display = CGMainDisplayID();
    CGDisplayModeRef currentMode = CGDisplayCopyDisplayMode(display);
    
    currentWidth = (int)CGDisplayModeGetWidth(currentMode);
    currentHeight = (int)CGDisplayModeGetHeight(currentMode);
    
    CGDisplayModeRelease(currentMode);

    

    auto resolution = getResolution(thisMac.macModel.data());
    
    newWidth = (int)std::get<0>(resolution);
    newHeight = (int)std::get<2>(resolution);

    if (currentHeight == (int)std::get<2>(resolution))
        newHeight = (int)std::get<1>(resolution);
    
    
    
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
