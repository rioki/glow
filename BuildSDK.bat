rem @echo off

set VERSION=0.1.0

call "%VS150COMNTOOLS%\..\..\vc\vcvarsall.bat" x86

msbuild glow.sln /t:Build /p:Configuration=Debug /p:Platform="x86" /m:3
msbuild glow.sln /t:Build /p:Configuration=Release /p:Platform="x86" /m:3

call "%VS150COMNTOOLS%\..\..\vc\vcvarsall.bat" x64

msbuild glow.sln /t:Build /p:Configuration=Debug /p:Platform="x64" /m:3
msbuild glow.sln /t:Build /p:Configuration=Release /p:Platform="x64" /m:3

rmdir /s /q glow-%VERSION%
mkdir glow-%VERSION%

copy README.md glow-%VERSION%

mkdir glow-%VERSION%\include
mkdir glow-%VERSION%\include\glow
xcopy /s glow\*.h glow-%VERSION%\include\glow

mkdir glow-%VERSION%\x86\Debug
copy x86\Debug\*.dll glow-%VERSION%\x86\Debug
copy x86\Debug\*.lib glow-%VERSION%\x86\Debug
copy x86\Debug\*.pdb glow-%VERSION%\x86\Debug

mkdir glow-%VERSION%\x86\Release
copy x86\Release\*.dll glow-%VERSION%\x86\Release
copy x86\Release\*.lib glow-%VERSION%\x86\Release
copy x86\Release\*.pdb glow-%VERSION%\x86\Release

mkdir glow-%VERSION%\x64\Debug
copy x64\Debug\*.dll glow-%VERSION%\x64\Debug
copy x64\Debug\*.lib glow-%VERSION%\x64\Debug
copy x64\Debug\*.pdb glow-%VERSION%\x64\Debug

mkdir glow-%VERSION%\x64\Release
copy x64\Release\*.dll glow-%VERSION%\x64\Release
copy x64\Release\*.lib glow-%VERSION%\x64\Release
copy x64\Release\*.pdb glow-%VERSION%\x64\Release
