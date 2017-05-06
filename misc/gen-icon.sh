#!/usr/bin/env bash

if hash convert 2>/dev/null; then	
	convert misc/icon.svg -resize 256x256 -quality 100 $1/icon.png
	convert icon.png -resize 128x128 -quality 100 $1/icon128.png
	convert icon.png -resize 64x64 -quality 100 $1/icon64.png
	convert icon.png -resize 48x48 -quality 100 $1/icon48.png
	convert icon.png -resize 32x32 -quality 100 $1/icon32.png
	mkdir -p $1
	convert $1/*.png $1/icon.ico
fi