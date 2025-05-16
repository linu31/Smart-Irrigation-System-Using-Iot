@echo off
chcp 65001 >nul
cd /d %~dp0

:main_menu
cls
echo 🔧 Welcome To Smart Irrigation System (Testing)
echo.
echo 📌 Choose a testing mode:
echo   1. Manual Testing (no hardware)
echo   2. Real-time Testing (with Arduino + sensors)
echo.
set /p choice=Enter your choice (1 or 2): 

if "%choice%"=="1" goto manual_test
if "%choice%"=="2" goto realtime_test

echo ❌ Invalid choice. Please enter 1 or 2.
timeout /t 2 >nul
goto main_menu

:manual_test
echo ✅ Opening the website for Manual Testing...
start "" /B chrome "%cd%\public\home.html"

echo 🕒 Website will run for 2 minutes. Please test your frontend manually.
timeout /t 120 >nul

echo ⛔ Closing browser...
taskkill /IM chrome.exe /F >nul 2>&1

goto ask_retry

:realtime_test
echo.
echo ✅ Preparing for Real-time Testing...
echo.

rem Compile Arduino code
cli\arduino-cli.exe compile --fqbn esp8266:esp8266:nodemcuv2 arduino
if %errorlevel% neq 0 (
    echo ❌ Compilation Failed! Please check your Arduino code.
    pause
    goto ask_retry
)

rem Upload Arduino code
cli\arduino-cli.exe upload -p COM6 --fqbn esp8266:esp8266:nodemcuv2 arduino
if %errorlevel% neq 0 (
    echo ❌ Upload Failed! Check your COM port or connection.
    pause
    goto ask_retry
)

echo ✅ Data uploaded successfully.
echo 🔁 Waiting for device reboot...
timeout /t 3 >nul

echo 🚀 Starting Node.js server...
start "" /B cmd /c "node server.js"

echo ⏳ Waiting for server to fully start...
timeout /t 5 >nul

start chrome "%cd%\public\home.html"


:ask_retry
echo.
set /p retry=🔁 Do you want to test again? (yes/no): 
if /i "%retry%"=="yes" goto main_menu
if /i "%retry%"=="no" goto end

echo ❌ Invalid input. Please type yes or no.
timeout /t 2 >nul
goto ask_retry

:end
echo 🙏🏻 Exiting. Thank you Visit Again!
exit
