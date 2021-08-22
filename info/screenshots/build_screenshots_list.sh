#!/bin/bash

echo "# Screenshots"
echo "_Various screenshots, sorted from newer to older..._"
echo

for f in $(ls -1t | grep '\.png$'); do
	b="${f%%.png}"
	echo "* **${b}:**  "
	echo "![${b}](${f}?raw=true \"${b}\")"
	echo
done

