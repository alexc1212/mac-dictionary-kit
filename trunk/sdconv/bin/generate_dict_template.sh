#!/bin/sh
#
#
# generate_dict_template.sh
#

compress_body=1
encrypt_body=0

COMPRESS_OPT=
ENCRYPT_OPT=

while getopts c:e: opt
do
	case $opt in
	c)
		COMPRESS_OPT=$OPTARG
		if [ $COMPRESS_OPT -eq 0 ]
		then
			compress_body=0
		fi
		;;
	e)
		ENCRYPT_OPT=$OPTARG
		if [ $ENCRYPT_OPT -gt 0 ]
		then
			encrypt_body=1
		fi
		;;
	esac
done

shift $((OPTIND - 1))


PROP_LIST_FILE=$1


cat << END_OF_FILE
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>CFBundleInfoDictionaryVersion</key>
	<string>6.0</string>
END_OF_FILE

cat $PROP_LIST_FILE

cat << END_OF_FILE
	<key>DCSDictionaryCSS</key>
	<string>DefaultStyle.css</string>
	<key>IDXDictionaryIndexes</key>
	<array>
		<dict>
			<key>IDXIndexAccessMethod</key>
			<string>com.apple.TrieAccessMethod</string>
			<key>IDXIndexBigEndian</key>
			<false/>
			<key>IDXIndexDataFields</key>
			<dict>
				<key>IDXExternalDataFields</key>
				<array>
					<dict>
						<key>IDXDataFieldName</key>
						<string>DCSExternalBodyID</string>
						<key>IDXDataSize</key>
						<integer>4</integer>
						<key>IDXIndexName</key>
						<string>DCSBodyDataIndex</string>
					</dict>
				</array>
				<key>IDXFixedDataFields</key>
				<array>
					<dict>
						<key>IDXDataFieldName</key>
						<string>DCSPrivateFlag</string>
						<key>IDXDataSize</key>
						<integer>2</integer>
					</dict>
				</array>
				<key>IDXVariableDataFields</key>
				<array>
					<dict>
						<key>IDXDataFieldName</key>
						<string>DCSKeyword</string>
						<key>IDXDataSizeLength</key>
						<integer>2</integer>
					</dict>
					<dict>
						<key>IDXDataFieldName</key>
						<string>DCSHeadword</string>
						<key>IDXDataSizeLength</key>
						<integer>2</integer>
					</dict>
					<dict>
						<key>IDXDataFieldName</key>
						<string>DCSAnchor</string>
						<key>IDXDataSizeLength</key>
						<integer>2</integer>
					</dict>
					<dict>
						<key>IDXDataFieldName</key>
						<string>DCSYomiWord</string>
						<key>IDXDataSizeLength</key>
						<integer>2</integer>
					</dict>
				</array>
			</dict>
			<key>IDXIndexDataSizeLength</key>
			<integer>2</integer>
			<key>IDXIndexKeyMatchingMethods</key>
			<array>
				<string>IDXExactMatch</string>
				<string>IDXPrefixMatch</string>
				<string>IDXCommonPrefixMatch</string>
				<string>IDXWildcardMatch</string>
			</array>
			<key>IDXIndexName</key>
			<string>DCSKeywordIndex</string>
			<key>IDXIndexPath</key>
			<string>KeyText.index</string>
			<key>IDXIndexSupportDataID</key>
			<false/>
			<key>IDXIndexWritable</key>
			<true/>
			<key>TrieAuxiliaryDataFile</key>
			<string>KeyText.data</string>
		</dict>
		<dict>
			<key>IDXIndexAccessMethod</key>
			<string>com.apple.TrieAccessMethod</string>
			<key>IDXIndexBigEndian</key>
			<false/>
			<key>IDXIndexDataFields</key>
			<dict>
				<key>IDXExternalDataFields</key>
				<array>
					<dict>
						<key>IDXDataFieldName</key>
						<string>DCSExternalBodyID</string>
						<key>IDXDataSize</key>
						<integer>4</integer>
						<key>IDXIndexName</key>
						<string>DCSBodyDataIndex</string>
					</dict>
				</array>
			</dict>
			<key>IDXIndexDataSizeLength</key>
			<integer>2</integer>
			<key>IDXIndexKeyMatchingMethods</key>
			<array>
				<string>IDXExactMatch</string>
			</array>
			<key>IDXIndexName</key>
			<string>DCSReferenceIndex</string>
			<key>IDXIndexPath</key>
			<string>EntryID.index</string>
			<key>IDXIndexSupportDataID</key>
			<false/>
			<key>IDXIndexWritable</key>
			<true/>
			<key>TrieAuxiliaryDataFile</key>
			<string>EntryID.data</string>
		</dict>
		<dict>
			<key>IDXIndexAccessMethod</key>
			<string>com.apple.HeapAccessMethod</string>
			<key>IDXIndexBigEndian</key>
			<false/>
			<key>IDXIndexDataFields</key>
			<dict>
				<key>IDXVariableDataFields</key>
				<array>
					<dict>
						<key>IDXDataFieldName</key>
						<string>DCSBodyData</string>
						<key>IDXDataSizeLength</key>
						<integer>4</integer>
					</dict>
				</array>
			</dict>
			<key>IDXIndexName</key>
			<string>DCSBodyDataIndex</string>
END_OF_FILE

if [ $compress_body -gt 0 ]
then
cat << END_OF_FILE
			<key>HeapDataCompressionType</key>
			<integer>1</integer>
END_OF_FILE
fi

if [ $encrypt_body -gt 0 ]
then
cat << END_OF_FILE
			<key>HeapDataEncrypted</key>
			<true/>
END_OF_FILE
fi

cat << END_OF_FILE
			<key>IDXIndexPath</key>
			<string>Body.data</string>
			<key>IDXIndexSupportDataID</key>
			<true/>
			<key>IDXIndexWritable</key>
			<true/>
		</dict>
	</array>
	<key>IDXDictionaryVersion</key>
	<integer>1</integer>
	<key>DCSBuildToolVersion</key>
	<integer>1</integer>
</dict>
</plist>
END_OF_FILE
