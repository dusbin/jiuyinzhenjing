@echo off
color 2
reg delete HKCR\Inkfile /v IsShortcut /f
reg delete HKCR\piffile /v IsShortcut /f
reg delete HKCR\InternetShortcut /v IsShortcut /f
taskkill /f /im explorer.exe && explorer



