/* vim:set ft=objc 
 * stardict.m: stardict dictionary parsing module */

#import <Foundation/Foundation.h>

enum SDDictionaryType { SDTypeTreeDict, SDTypeDict };

@interface SDDictionary : NSObject
{
    NSString *ifoFile;
    NSString *dictFile;
    NSString *idxFile;

    enum SDDictionaryType type;
    NSString *version;
    NSString *name;
    NSString *identifier;
    
    NSInteger wordCount;
    NSInteger indexFileSize;
}

@property (copy) NSString *ifoFile;
@property (copy) NSString *dictFile;
@property (copy) NSString *idxFile;

@property (copy) NSString *version;
@property (copy) NSString *name;
@property (copy) NSString *identifier;
@property (assign) NSInteger wordCount;
@property (assign) NSInteger indexFileSize;

- (id) initWithIfoFile:(NSString *)ifoFile;
- (id) initWithArchive:(NSString *)archive;
- (NSInteger) getIntegerFromLine:(NSString *)line 
                      withPrefix:(NSString *)prefix;

@end
