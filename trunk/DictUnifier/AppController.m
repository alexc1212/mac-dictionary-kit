#import "AppController.h"

@implementation AppController

- (void)awakeFromNib
{
    windowExpanded = NO;

    [dictionaryNameField setEnabled: NO];
    [dictionaryNameField setStringValue: @"No dictionary loaded"];
    
    [dictionaryIDField setEnabled: NO];
    [dictionaryIDField setStringValue: @"No dictionary loaded"];
    
    [dictionaryWordCountField setEnabled: NO];
    [dictionaryWordCountField setStringValue: @"No dictionary loaded"];
    
    NSString *defaultDirectory = [NSHomeDirectory() stringByAppendingPathComponent:@"Downloads"];
    NSURL *url = [NSURL fileURLWithPath: defaultDirectory];

    [dictionaryPath setURL: url];
    
    NSFont *font = [NSFont fontWithName: @"Monaco" size: 14.0];
    [progressTextView setFont: font];
    
    [[NSNotificationCenter defaultCenter] addObserver: self
                                             selector: @selector(taskDidEnd:)
                                                 name: NSTaskDidTerminateNotification
                                               object: nil];    
}

- (IBAction)chooseFile:(id)sender
{
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    [panel setAllowsMultipleSelection: NO];
    [panel setCanChooseDirectories: NO];
    [panel setCanChooseFiles: YES];
    [panel setResolvesAliases: YES];
    [panel setTitle: @"Choose a dictionary file"];
    [panel setPrompt: @"Choose"];
    
    if ([panel runModalForTypes: [NSArray arrayWithObject: @"bz2"]] == NSOKButton)
    {
        [dictionaryPath setURL: [[panel URLs] objectAtIndex: 0]];
        
        // [progressIndicator setHidden: NO];
        [progressIndicator setIndeterminate: YES];
        [progressIndicator setStyle: NSProgressIndicatorSpinningStyle];
        [progressIndicator startAnimation: self];
        
        dictionary = [[SDDictionary alloc] initWithArchive: [[dictionaryPath URL] path]];

        [progressIndicator stopAnimation: self];
        [dictionaryNameField setStringValue: [dictionary name]];
        [dictionaryWordCountField setIntegerValue: [dictionary wordCount]];
        [dictionaryIDField setStringValue: [dictionary identifier]];
        
        [dictionaryNameField setEnabled: YES];
        [dictionaryIDField setEnabled: YES];
        [dictionaryNameField setEditable:YES];
        [dictionaryIDField setEditable: YES];
    } else
    {
        [dictionary release];
        dictionary = nil;
    }
}

- (void) dealloc
{
    [dictionary release];
    [super dealloc];
}

- (void)outputString:(NSString *)string
{
    if (string == nil)
        return;

    NSTextStorage *ts = [progressTextView textStorage];
    
    [ts beginEditing];
    [[ts mutableString] appendString: string];
    [ts endEditing];
    
    int size = [[progressTextView textStorage] length];
    [progressTextView scrollRangeToVisible: NSMakeRange(size - 1, 1)];
}

- (void)fileHandleRead:(NSNotification *)aNotification
{
	NSDictionary *userInfo = [aNotification userInfo];
	int error = [[userInfo objectForKey: @"NSFileHandleError"] intValue];
    
    if (error)
        NSLog(@"sdconv: error %d.", error);
    else {
		NSData *data = [userInfo objectForKey: NSFileHandleNotificationDataItem];
		NSUInteger length = [data length];
        if (length == 0)
            return;

        [self outputString: [NSString stringWithUTF8String: [data bytes]]];

        if (length > 0)
            [readHandle readInBackgroundAndNotify];
    }
}

- (void)taskDidEnd:(NSNotification *)aNotification
{
    [progressIndicator stopAnimation: self];
    [convertButton setEnabled: YES];
    
    system("osascript -e 'tell app \"Dictionary\" to quit'");
    system("osascript -e 'tell app \"Dictionary\" to launch'");
}

- (IBAction)convertDictionary:(id)sender
{
    if (! dictionary)
    {
        NSAlert *alert = [NSAlert alertWithMessageText: @"No dictionary loaded" 
                                         defaultButton: @"OK" 
                                       alternateButton: nil
                                           otherButton: nil
                             informativeTextWithFormat: @"Please choose a dictionary file first."];
        [alert runModal];
        // [alert release];
        
        return;
    }
    
    NSTask *task = [[NSTask alloc] init];
    
    NSString *scriptPath = [[[NSBundle mainBundle] resourcePath] stringByAppendingFormat: @"/sdconv/convert"];
    NSArray *arguments = [NSArray arrayWithObjects: scriptPath, [[dictionaryPath URL] path], nil];
    NSPipe *pipe = [NSPipe pipe];
    
    NSString *workingPath = [NSHomeDirectory() stringByAppendingFormat: @"/.sdconv"];
    [[NSFileManager defaultManager] createDirectoryAtPath: workingPath 
                              withIntermediateDirectories: YES 
                                               attributes: nil 
                                                    error: NULL];

    [task setCurrentDirectoryPath: workingPath];
    [task setLaunchPath: @"/usr/bin/python"];
    [task setArguments: arguments];
    [task setStandardOutput: pipe];
    [task setStandardError: pipe];

    readHandle = [pipe fileHandleForReading];

    [[NSNotificationCenter defaultCenter] addObserver: self 
                                             selector: @selector(fileHandleRead:) 
                                                 name: NSFileHandleReadCompletionNotification 
                                               object: readHandle];
        
    [readHandle readInBackgroundAndNotify];

    [progressIndicator startAnimation: self];
    [convertButton setEnabled: NO];
    [task launch];
    // [task waitUntilExit];
    // [progressIndicator stopAnimation: self];
    
    int status = [task terminationStatus];
    
    if (status == 0)
        NSLog(@"Task succeeded.");
    else
        NSLog(@"Task failed.");
    
    [task release];
}

- (IBAction)expandWindow:(id)sender
{
    NSRect frame = [window frame];
    NSRect newFrame;
    
    newFrame.size = frame.size;
    
    if (! windowExpanded)
    {
        newFrame.size.height += 260.0;
    } else
        newFrame.size.height -= 260.0;

    windowExpanded = (! windowExpanded);
    newFrame.origin = NSMakePoint(NSMinX([window frame]), 
                                  NSMaxY([window frame]) - NSHeight(newFrame));
    
    [window setFrame: newFrame 
             display: YES 
             animate: YES];
}

@end
