/* vim:set ft=objc 
 * stardict.m: stardict dictionary parsing module */
 
#import "SDDictionary.h"
#import <archive.h>
#import <archive_entry.h>
#import <fnmatch.h>

NSString *SDDictMagicData     = @"StarDict's dict ifo file";
NSString *SDTreeDictMagicData = @"StarDict's treedict ifo file";
NSString *SDVersionPrefix     = @"version=";
NSString *SDNamePrefix        = @"bookname=";
NSString *SDWordCountPrefix   = @"wordcount=";
NSString *SDIdxFileSizePrefix = @"idxfilesize=";

@implementation SDDictionary 

// @synthesize ifoFile;
@synthesize dictFile;
@synthesize idxFile;

@synthesize version;
@synthesize name;
@synthesize identifier;
@synthesize wordCount;
@synthesize indexFileSize;

enum IFOParseState {
    IFO_EXPECT_MAGIC_DATA, 
    IFO_EXPECT_VERSION, 
    IFO_EXPECT_OTHER
};

- (void) setIfoFile:(NSString *)anIfoFile
{
    ifoFile = [anIfoFile copy];
    
    [self setIdentifier: [[anIfoFile lastPathComponent] stringByDeletingPathExtension]];
}

- (NSString *)ifoFile
{
    return ifoFile;
}

- (NSInteger) getIntegerFromLine:(NSString *)line 
                      withPrefix:(NSString *)prefix
{
    NSInteger integer;
    
    NSRange range = [line rangeOfString: prefix];
    NSString *intStr = [line substringFromIndex: range.location + range.length];
    
    NSScanner *scanner = [NSScanner scannerWithString: intStr];
    [scanner scanInteger: &integer];
    
    return integer;
}

- (NSString *) getStringFromLine:(NSString *)line
                      withPrefix:(NSString *)prefix
{
    NSRange range = [line rangeOfString: prefix];
    NSString *str = [line substringFromIndex: range.location + range.length];
    
    return str;
}

- (int) loadIfoContent:(NSString *)ifoContent
{
    NSArray *lines = [ifoContent componentsSeparatedByString: @"\n"];
    if (! lines)
        return 0;

    NSString *line;
    enum IFOParseState state = IFO_EXPECT_MAGIC_DATA;

    for (line in lines)
    {
        NSLog(line);

        switch (state)
        {
        case IFO_EXPECT_MAGIC_DATA:
            if ([line isEqualToString: SDDictMagicData])
            {
                type = SDTypeDict;
                state = IFO_EXPECT_VERSION;
            }
            else
            if ([line isEqualToString: SDTreeDictMagicData])
            {
                type = SDTypeTreeDict;
                state = IFO_EXPECT_VERSION;
            }
            else
                return 0;
            break;
            
        case IFO_EXPECT_VERSION:
            if ([line hasPrefix: SDVersionPrefix])
            {
                [self setVersion: [self getStringFromLine: line 
                                               withPrefix: SDVersionPrefix]];
                state = IFO_EXPECT_OTHER;
            }
            else
                return 0;
            break;

        case IFO_EXPECT_OTHER:
            if ([line hasPrefix: SDWordCountPrefix])
                wordCount = [self getIntegerFromLine: line 
                                          withPrefix: SDWordCountPrefix];
            
            else
            if ([line hasPrefix: SDNamePrefix])
                [self setName: [self getStringFromLine: line 
                                            withPrefix: SDNamePrefix]];
                                            
            else
            if ([line hasPrefix: SDIdxFileSizePrefix])
                [self setIndexFileSize: [self getIntegerFromLine: line
                                                      withPrefix: SDIdxFileSizePrefix]];
            break;
        }
    }
    
    return 1;
}

- (id) initWithIfoFile:(NSString *)anIfoFile
{
    if ((self = [super init])) {
        if (! [[NSFileManager defaultManager] fileExistsAtPath: anIfoFile])
            return nil;

        [self setIfoFile: anIfoFile];
        if (! [self loadIfoContent: [NSString stringWithContentsOfFile: anIfoFile
                                                              encoding: NSUTF8StringEncoding
                                                                 error: NULL]])
            return nil;

        NSString *base = [anIfoFile stringByDeletingPathExtension];
        [self setDictFile: [base stringByAppendingPathExtension: @"dict.dz"]];
        [self setIdxFile: [base stringByAppendingPathExtension: @"idx"]];
    }
    
    return self;
}

- (id) initWithArchive:(NSString *)archive
{
    if ((self = [super init])) {
        if (! [[NSFileManager defaultManager] fileExistsAtPath: archive])
            return nil;
        
        struct archive *ar;
        struct archive_entry *entry;
        
        ar = archive_read_new();
        archive_read_support_compression_all(ar);
        archive_read_support_format_all(ar);
        if (archive_read_open_filename(ar, [archive fileSystemRepresentation], 4096) != ARCHIVE_OK)
        {
            archive_read_finish(ar);
            return nil;
        }

        while (archive_read_next_header(ar, &entry) == ARCHIVE_OK)
        {
            if (fnmatch("*.ifo", archive_entry_pathname(entry), 0) == 0)
            {
                [self setIfoFile: [NSString stringWithUTF8String: archive_entry_pathname(entry)]];
                int64_t size = archive_entry_size(entry);
                char *buf = (char *) malloc(size + 1);
                archive_read_data(ar, buf, size);
                buf[size] = '\0';

                [self loadIfoContent: [NSString stringWithUTF8String: buf]];

                free(buf);
                break;
            }
            
            archive_read_data_skip(ar);
        }
        
        archive_read_finish(ar);        
    }
    return self;
}

@end
