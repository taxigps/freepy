@echo off
echo 即将安装自由拼音输入法，桌面（explorer.exe)将重启，勿大惊小怪O(∩_∩)O~
pause
@echo off
taskkill /f /im explorer.exe
start explorer.exe
copy Debug\freepy.dll c:\windows\system32\freepy.ime
echo 安装成功！
pause