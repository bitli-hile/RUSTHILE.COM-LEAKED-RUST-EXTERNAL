#include <iostream>
#include <vector>
#include <thread>
#include <conio.h>
#include <Windows.h>
#include "driverloader.h"
#include "xorstr.hpp"

#define _CRT_SECURE_NO_WARNINGS

#define FG_RED _xor_("\033[31m").c_str()
#define FG_GREEN _xor_("\033[32m").c_str()
#define FG_YELLOW _xor_("\033[33m").c_str()
#define FG_WHITE _xor_("\033[0m").c_str()
#define FG_CYAN    _xor_("\033[36m").c_str()

void pencere()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(handle, &info);
    COORD new_size =
    {
        info.srWindow.Right - info.srWindow.Left + 1,
        info.srWindow.Bottom - info.srWindow.Top + 1
    };
    int count = 0;
     int horizontal = 0, vertical = 0;
    int x = 300, y = 400;
    HWND hwnd = GetConsoleWindow();
    MoveWindow(hwnd, 0, 0, x, y, FALSE);
    RECT desktop;
    GetWindowRect(GetDesktopWindow(), &desktop);
    SetWindowPos(hwnd, HWND_NOTOPMOST, desktop.right / 2 - x / 2, desktop.bottom / 2 - y / 2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
    lStyle &= ~(WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
    SetWindowLong(hwnd, GWL_STYLE, lStyle);
    GetConsoleScreenBufferInfo(handle, &info);
    COORD scrollbar = {
        info.srWindow.Right - info.srWindow.Left + 1,
        info.srWindow.Bottom - info.srWindow.Top + 1
    };
    if (handle == 0)
        throw 0;
    else
        SetConsoleScreenBufferSize(handle, scrollbar);
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, 0, (255 * 100) / 10, LWA_ALPHA);
    CONSOLE_CURSOR_INFO info2;
    info2.dwSize = 100;
    info2.bVisible = FALSE;
    SetConsoleCursorInfo(handle, &info2);
}

bool BOXESP;
bool NORECOIL;
bool ADMINFLAG;
bool SPIDERMAN;
bool ALWAYSDAY;




void CHEAT()
{
    bool BOXESPFEATURE = true;
    bool NORECOILFEATURE = true;
    bool ADMINFLAGFEATURE = true;
    bool SPIDERMANFEATURE = true;
    bool ALWAYSDAYFEATURE = true;
    while (true)
    {
        if (!FindWindowA(_xor_("UnityWndClass").c_str(), _xor_("Rust").c_str())) exit(0);
        if (BOXESP)
        {
            if (BOXESPFEATURE)
            {
                keybd_event(0xF9, 0xF9, KEYEVENTF_EXTENDEDKEY, 0);
                keybd_event(0xF9, 0xF9, KEYEVENTF_KEYUP, 0);
                BOXESPFEATURE = false;
            }
        }
        else
        {
            if (!BOXESPFEATURE)
            {
                keybd_event(0xF9, 0xF9, KEYEVENTF_EXTENDEDKEY, 0);
                keybd_event(0xF9, 0xF9, KEYEVENTF_KEYUP, 0);
                BOXESPFEATURE = true;
            }
        }
        if (NORECOIL)
        {
            if (NORECOILFEATURE)
            {
                keybd_event(0xF8, 0xF8, KEYEVENTF_EXTENDEDKEY, 0);
                keybd_event(0xF8, 0xF8, KEYEVENTF_KEYUP, 0);
                NORECOILFEATURE = false;
            }
        }
        else
        {
            if (!NORECOILFEATURE)
            {
                keybd_event(0xF8, 0xF8, KEYEVENTF_EXTENDEDKEY, 0);
                keybd_event(0xF8, 0xF8, KEYEVENTF_KEYUP, 0);
                NORECOILFEATURE = true;
            }
        }
        if (ADMINFLAG)
        {
            if (ADMINFLAGFEATURE)
            {
                keybd_event(0xF7, 0xF7, KEYEVENTF_EXTENDEDKEY, 0);
                keybd_event(0xF7, 0xF7, KEYEVENTF_KEYUP, 0);
                ADMINFLAGFEATURE = false;
            }
        }
        else
        {
            if (!ADMINFLAGFEATURE)
            {
                keybd_event(0xF7, 0xF7, KEYEVENTF_EXTENDEDKEY, 0);
                keybd_event(0xF7, 0xF7, KEYEVENTF_KEYUP, 0);
                ADMINFLAGFEATURE = true;
            }
        }
        if (SPIDERMAN)
        {
            if (SPIDERMANFEATURE)
            {
                keybd_event(0xF6, 0xF6, KEYEVENTF_EXTENDEDKEY, 0);
                keybd_event(0xF6, 0xF6, KEYEVENTF_KEYUP, 0);
                SPIDERMANFEATURE = false;
            }
        }
        else
        {
            if (!SPIDERMANFEATURE)
            {
                keybd_event(0xF6, 0xF6, KEYEVENTF_EXTENDEDKEY, 0);
                keybd_event(0xF6, 0xF6, KEYEVENTF_KEYUP, 0);
                SPIDERMANFEATURE = true;
            }
        }
        if (ALWAYSDAY)
        {
            if (ALWAYSDAYFEATURE)
            {
                keybd_event(0xE8, 0xE8, KEYEVENTF_EXTENDEDKEY, 0);
                keybd_event(0xE8, 0xE8, KEYEVENTF_KEYUP, 0);
                ALWAYSDAYFEATURE = false;
            }
        }
        else
        {
            if (!ALWAYSDAYFEATURE)
            {
                keybd_event(0xE8, 0xE8, KEYEVENTF_EXTENDEDKEY, 0);
                keybd_event(0xE8, 0xE8, KEYEVENTF_KEYUP, 0);
                ALWAYSDAYFEATURE = true;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

 

bool checkServiceStatus()
{
    SC_HANDLE scm_handle = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    SC_HANDLE service_handle = OpenServiceA(scm_handle, _xor_("wdfilter").c_str(), SERVICE_QUERY_STATUS);
    SERVICE_STATUS service_status;
    QueryServiceStatus(service_handle, &service_status);
    if (service_status.dwCurrentState == SERVICE_STOPPED)
    {
        return true;
    }
    else
    { 
        MessageBoxA(NULL, _xor_("WDF working please stop!\n\n https://www.sordum.org/9480/defender-control-v2-1/").c_str(), _xor_("RUST ARBIT LEGIT").c_str(), MB_OK | MB_ICONHAND);
        exit(0);
    }
    CloseServiceHandle(service_handle);
    CloseServiceHandle(scm_handle);
}
int main()
{
    SetConsoleTitle(_xor_(L"RUSTHILE.COM").c_str());
    if (!FindWindowA(_xor_("UnityWndClass").c_str(), _xor_("Rust").c_str()))
    {
        keybd_event(0xE7, 0xE7, KEYEVENTF_EXTENDEDKEY, 0);
        keybd_event(0xE7, 0xE7, KEYEVENTF_KEYUP, 0);
    }
    system("cls");
    pencere();
EnBasaDon:
   

    checkServiceStatus();
    if(FindWindowA(_xor_("UnityWndClass").c_str(), _xor_("Rust").c_str()))
    {
        MessageBoxA(NULL, _xor_("RUSTI KAPAT!").c_str(), _xor_("RUST ARBIT LEGIT").c_str(), MB_OK | MB_ICONASTERISK);
        exit(0);
    }
 
    Sleep(1500);
    //raw_image = KeyAuthApp.download(_xor_(" ").c_str()); // Drivera.sys keyautha koyulacak
    BypassLoader();
    raw_image.clear();

r5:
    system(_xor_("cls").c_str());
    Sleep(15000);
     std::cout << FG_YELLOW << _xor_("\n\n  [+] RUST BEKLENIYOR.").c_str() << std::endl;;
    system(_xor_("start steam://rungameid/252490").c_str());
    if (!FindWindowA(_xor_("UnityWndClass").c_str(), _xor_("Rust").c_str()))
        goto r5;
  
    std::cout << FG_GREEN << _xor_("\n\n  [+] RUST BULUNDU HILE ACILIYOR.").c_str() << std::endl;;
    Sleep(15000);

    system(_xor_("cls").c_str());
    std::thread(CHEAT).detach();
    const int numOptions = 5;
    std::vector<bool> options(numOptions, false); // Default: all options are false
    int choice = 0;
    while (true)
    {
        pencere();
        // Print menu options
        system("cls"); // Clear the screen
        std::cout << FG_CYAN << _xor_("\n         RUST ARBIT LEGIT\n\n").c_str() << std::endl;
        std::cout << FG_YELLOW << _xor_("  _______________________________\n\n\n").c_str() << std::endl;
        for (int i = 0; i < numOptions; i++)
        {
            if (i == choice)
                std::cout << FG_YELLOW << _xor_("    > ").c_str();
            else
                std::cout << FG_WHITE << _xor_("      ").c_str();

            if (options[i])
                std::cout << FG_WHITE << _xor_("[").c_str() << FG_GREEN << _xor_(" ACIK ").c_str() << FG_WHITE << _xor_("] ").c_str();
            else
                std::cout << FG_WHITE << _xor_("[").c_str() << FG_RED << _xor_("KAPALI").c_str() << FG_WHITE << _xor_("] ").c_str();

            switch (i)
            {
            case 0:
                std::cout << FG_WHITE << _xor_("KUTU ESP").c_str();
                break;
            case 1:
                std::cout << FG_WHITE << _xor_("SEKMEME").c_str();
                break;
            case 2:
                std::cout << FG_WHITE << _xor_("IZLEYICI MODU").c_str();
                break;
            case 3:
                std::cout << FG_WHITE << _xor_("ORUMCEK ADAM").c_str();
                break;
            case 4:
                std::cout << FG_WHITE << _xor_("HER ZAMAN SABAH").c_str();
                break;
            }

            std::cout << std::endl;
        }
        std::cout << FG_YELLOW << _xor_("\n\n\n  _______________________________\n").c_str() << std::endl;

        // Wait for user input
        int key = _getch();
        if (key == 224) // Special key (like arrow keys)
        {
            key = _getch();
            if (key == 72) // Up arrow
            {
                if (choice > 0)
                    choice--;
            }
            else if (key == 80) // Down arrow
            {
                if (choice < numOptions - 1)
                    choice++;
            }
            else if (key == 77) // Enter key
            {
                // Toggle selected option
                options[choice] = !options[choice];

                // Activate corresponding flag
                switch (choice)
                {
                case 0:
                    BOXESP = options[0];
                    break;
                case 1:
                    NORECOIL = options[1];
                    break;
                case 2:
                    ADMINFLAG = options[2];
                    break;
                case 3:
                    SPIDERMAN = options[3];
                    break;
                case 4:
                    ALWAYSDAY = options[4];
                    break;
                }
            }
        }
       
        
    }

    return 0;
}