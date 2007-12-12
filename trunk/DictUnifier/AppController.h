#import <Cocoa/Cocoa.h>
#import "SDDictionary.h"

@interface AppController : NSObject {
    IBOutlet NSWindow    *window;
    IBOutlet NSTextField *dictionaryIDField;
    IBOutlet NSTextField *dictionaryNameField;
    IBOutlet NSTextField *dictionaryWordCountField;
    IBOutlet NSTextView  *progressTextView;
    IBOutlet NSPathControl *dictionaryPath;
    IBOutlet NSProgressIndicator *progressIndicator;
    IBOutlet NSButton    *convertButton;
    BOOL                  windowExpanded;
    SDDictionary         *dictionary;
    NSFileHandle         *readHandle;
}
- (IBAction)chooseFile:(id)sender;
- (IBAction)convertDictionary:(id)sender;
- (IBAction)expandWindow:(id)sender;
@end
