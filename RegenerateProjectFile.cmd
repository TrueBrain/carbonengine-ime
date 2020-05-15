@echo off
echo Checking out project and filters file
p4 edit IME_v141.vcxproj
p4 edit IME_v141.vcxproj.filters
echo Regenerating
..\..\..\..\..\..\shared_tools\python\27\python.exe ..\..\tools\ProjectFileGenerator\ProjectFileGenerator.py -i IME.ccpproj --outfile=IME_v141.vcxproj --toolset=v141
pause
