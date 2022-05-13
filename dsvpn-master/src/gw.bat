@echo off
for /f "tokens=2,3 delims={,}" %%a in ('"WMIC NICConfig where IPEnabled="True" get DefaultIPGateway /value | find "I" "') do echo IPv4 %%~a IPV6 %%~b
pause

@echo on
set "ip="
for /f "tokens=1-2 delims=:" %%a in ('ipconfig^|find "Default"') do if not defined ip set ip=%%b 
pause

echo "check also https://superuser.com/questions/524822/awk-equivalent-functionality-on-windows"