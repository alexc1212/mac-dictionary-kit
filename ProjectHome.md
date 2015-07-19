Mac Dictionary Kit is a collection of tools to help convert and build dictionaries on Mac OS X platform. The source dictionary could be in any format, but we only support [stardict format](https://stardict.svn.sourceforge.net/svnroot/stardict/trunk/doc/StarDictFileFormat) at present. The target dictionary must be Dictionary 2.0 format defined in Mac OS X 10.5.

## News ##

DictUnifier 2.1 released!

  * Fixed a quite a few bugs
  * Support Mac OS X 10.7 "Lion"

DictUnifier 2.0 beta 3 released!

  * Rewrote all python code (again!) with Objective-C to reduce memory footprint and improve performance
  * Tuned interface for displaying conversion progress
  * Experimental support of Lua scripting with LuaJIT 2.0.0 beta 3 (for sdconv only)
  * Only support Intel Macs

DictUnifier 2.0 beta 2 released!

  * Rewrote the interface
  * Update to leverage Mac OS X 10.6 compression options

sdconv 0.3 and DictUnifier 1.1 released!

Update highlights:

  * Fixed a lot of directory naming problems.
  * Support using python as dictionary entry transformation script.
  * You can change converted dictionary name and id in DictUnifier.

## Installation ##

Download command line utility [sdconv](sdconv.md) or GUI utility DictUnifier from links in the right pane. Follow instructions in contained README.txt.