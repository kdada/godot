#!/usr/bin/env sh

# Minimum NDK API level required to build the export template.
MINIMUM_API_LEVEL=18
# The API level of the currently installed NDK.
installed_api_level=0
# Download URL for command line tools. The NDK is not currently distributed separately.
URL="https://repo.huaweicloud.com/harmonyos/ohpm/5.1.0/commandline-tools-linux-x64-5.1.0.840.zip"

echo "Check currently installed OpenHarmony NDK API level."

if command -v jq 2>&1 >/dev/null; then
	version_file=$HOME/OpenHarmony/command-line-tools/sdk/default/openharmony/native/oh-uni-package.json
	if [ -f "$version_file" ]; then
		installed_api_level=`jq -r '.apiVersion' $version_file`
	fi
else
	echo 'Error: Could not find 'jq' command. Is `jq` installed?'
fi

if [ $installed_api_level -ge $MINIMUM_API_LEVEL ]; then
	echo "The NDK that meets the required API level is already installed. Skipping installation."
	exit 0
else
	echo "The NDK is not installed or the currently installed NDK's API level does not meet the required API level."
	rm -rf $HOME/OpenHarmony/command-line-tools/
fi

echo "::group::Download OpenHarmony Command Line Tools."

curl -L "$URL" -o /tmp/commandline-tools.zip
curl -L "$URL.sha256" -o /tmp/commandline-tools.zip.sha256

echo "::endgroup::"

sha256sum /tmp/commandline-tools.zip | grep -f /tmp/commandline-tools.zip.sha256 - 2>&1 >/dev/null

if [ $? -eq 0 ]; then
	echo "Download successful."
else
	echo "Error: Download failed."
	exit 1
fi

echo "::group::Extract OpenHarmony NDK."

unzip /tmp/commandline-tools.zip "command-line-tools/sdk/default/openharmony/native/*" -d $HOME/OpenHarmony/

if [ $? -eq 0 ]; then
	echo "::endgroup::"
	echo "Extract successful."
else
	echo "::endgroup::"
	echo "Error: Extract failed."
	exit 1
fi

rm /tmp/commandline-tools.zip
