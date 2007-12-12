#!/usr/bin/python
# build-dict.py

import sys, os

dict_name  = sys.argv[1]

# The DICT_BUILD_TOOL_DIR value is used also in "build_dict.sh" script.
# You need to set it when you invoke the script directly.

dict_build_tool_dir = sys.argv[2]
dict_build_tool_bin = "%s/bin" % dict_build_tool_dir

os.putenv("LANG", "en_US.UTF-8")
os.putenv("DICT_BUILD_TOOL_DIR", dict_build_tool_dir)

cmd = "\"%s/build_dict.sh\" %s Dictionary.xml Dictionary.css DictInfo.plist" % \
        (dict_build_tool_bin, dict_name)
print cmd
os.system(cmd)
print "Done."

dest_dir = "~/Library/Dictionaries"
print "Installing into %s." % dest_dir

os.system("mkdir -p %s" % dest_dir)
os.system("ditto --noextattr --norsrc ./objects/%s.dictionary %s/%s.dictionary" % \
        (dict_name, dest_dir, dict_name))
os.system("touch %s" % dest_dir)

print "Done."
print "To test the new dictionary, try Dictionary.app."

