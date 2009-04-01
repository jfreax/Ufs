#!/bin/sh

echo " STATS: $(cat $(ls -w 1 *.hpp *.cpp */*.hpp */*.cpp) | wc -l) Zeilen ($(cat $(ls -w 1 *.hpp *.cpp */*.hpp */*.cpp) | wc -w) Wörter) in $(ls -w 1 *.hpp *.cpp */*.hpp */*.cpp | wc -l) Dateien"
echo " STATS: $(echo $(cat $(ls -w 1 *.cpp *.hpp gui/*.hpp gui/*.cpp) | wc -w) / $(cat $(ls -w 1 *.cpp *.hpp gui/*.hpp gui/*.cpp) | wc -l) | bc --mathlib | cut -b 1-4) Wörter/Zeile; $(echo $(cat $(ls -w 1 *.cpp *.hpp gui/*.hpp gui/*.cpp) | wc -l) / $(ls -w 1 *.cpp *.hpp gui/*.hpp gui/*.cpp | wc -l) | bc --mathlib | cut -b 1-6) Zeilen/Datei"
