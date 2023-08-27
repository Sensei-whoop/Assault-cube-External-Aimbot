#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <string>
#include "proc.h"
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <string>
#include <math.h> 
#define PI 3.14159265






int main()
{
    
 
	DWORD pid;
    //X, Z and Y offsets in assult cube are 30, 50, 28 (all in hex)
    
	HWND hwnd = FindWindow(NULL, "AssaultCube");
	GetWindowThreadProcessId(hwnd, &pid);
    
	HANDLE phandle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    
    uintptr_t base = GetModuleBaseAddress(pid, L"ac_client.exe") + 0x17E0A8;
   

    uintptr_t entitylist = GetModuleBaseAddress(pid, L"ac_client.exe") + 0x18AC04;
    uintptr_t ammoAddr = FindDMAAddy(phandle, base, ammoOffsets);
    DWORD FoundStatic;
    DWORD heightaddr;
    DWORD isonenemy;
    
    DWORD entitylistaddress;
    ReadProcessMemory(phandle, (LPVOID)(entitylist), &entitylistaddress, sizeof(entitylistaddress), NULL);

    ReadProcessMemory(phandle, (LPVOID)(base), &FoundStatic, sizeof(FoundStatic), NULL);
    
    //the recoil value
    DWORD RecoilValueY;
    
    std::cout << "The static address" << (FoundStatic) << "\n";
    
    float horizontalangle;
    float verticalangle;
    DWORD entitypointer;
    float playerX;
    float playerZ;
    float playerY;
    char names [20];
    float enemyXO;
    float enemyYO;
    float enemyZO;
    float testangle = 75;
    int enemyhealth;
    std::cout << (LPVOID)(entitylistaddress) << "\n";
    bool locked = false;

    while (TRUE)
    {
        //getplayer pos
        ReadProcessMemory(phandle, (LPVOID)(FoundStatic + 0x2C), &playerX, sizeof(playerX), NULL);
        ReadProcessMemory(phandle, (LPVOID)(FoundStatic + 0x30), &playerY, sizeof(playerY), NULL);
        ReadProcessMemory(phandle, (LPVOID)(FoundStatic + 0x28), &playerZ, sizeof(playerZ), NULL);
        //get player angles
        ReadProcessMemory(phandle, (LPVOID)(FoundStatic + 0x34), &horizontalangle, sizeof(horizontalangle), NULL);
        ReadProcessMemory(phandle, (LPVOID)(FoundStatic + 0x38), &verticalangle, sizeof(verticalangle), NULL);
        
        
        //the offset for y is 0x30
        
        if (GetAsyncKeyState(VK_LBUTTON)) {
            
            for (int i = 1; i < 3; i++) {
                ReadProcessMemory(phandle, (LPVOID)(entitylistaddress + (0x4 * i)), &entitypointer, sizeof(entitypointer), NULL);
                
                ReadProcessMemory(phandle, (LPVOID)(entitylistaddress + (0x4 * i)), &entitypointer, sizeof(entitypointer), NULL);

                ReadProcessMemory(phandle, (LPVOID)(entitypointer + 0x2C), &enemyXO, sizeof(enemyXO), NULL);
                ReadProcessMemory(phandle, (LPVOID)(entitypointer + 0x30), &enemyYO, sizeof(enemyYO), NULL);
                ReadProcessMemory(phandle, (LPVOID)(entitypointer + 0x28), &enemyZO, sizeof(enemyZO), NULL);
                ReadProcessMemory(phandle, (LPVOID)(entitypointer + 0xEC), &enemyhealth, sizeof(enemyhealth), NULL);
                ReadProcessMemory(phandle, (LPVOID)(entitypointer + 0x205), &names, sizeof(names), NULL);
              
                float distance = sqrt(pow((enemyXO - playerX), 2) + pow((enemyYO - playerY), 2) + pow((enemyZO - playerZ), 2));
                
                if (distance > 40)
                    continue;

                if (enemyhealth <= 0)
                    continue;

              
                
                
                else {
                    
                    float xdistance = (enemyXO - playerX);
                    float zdistance = (enemyZO - playerZ);
                    float calculation = (atan(abs(zdistance / xdistance)) / PI) * 180;

                    if ((xdistance <= 0) && (zdistance >= 0)) {
                        std::cout << "works1";
                        float angleXZ = (atan(abs(zdistance / xdistance)) /PI) * 180;
                        WriteProcessMemory(phandle, (LPVOID)(FoundStatic + 0x34), &(angleXZ), sizeof(angleXZ), NULL);
                    }
                    else if ((xdistance > 0) && (zdistance > 0)) {
                        std::cout << "works2";
                        float angleXZ = 180 - ((atan(abs(zdistance / xdistance)) / PI) * 180) ;
                        WriteProcessMemory(phandle, (LPVOID)(FoundStatic + 0x34), &(angleXZ), sizeof(angleXZ), NULL);
                    }
                    //this one works
                    else if ((xdistance > 0) && (zdistance < 0)) {
                        std::cout << "works3";
                        float angleXZ = 180 + ((atan(abs(zdistance / xdistance)) / PI) * 180);
                        WriteProcessMemory(phandle, (LPVOID)(FoundStatic + 0x34), &(angleXZ), sizeof(angleXZ), NULL);
                    }
                    else if ((xdistance < 0) && (zdistance < 0)) {
                        std::cout << "works4";
                        float angleXZ = 360 - ((atan(abs(zdistance / xdistance)) / PI) * 180);
                        WriteProcessMemory(phandle, (LPVOID)(FoundStatic + 0x34), &(angleXZ), sizeof(angleXZ), NULL);
                    }

                    float distanceH = sqrt(pow(xdistance, 2) + pow(zdistance, 2));
                    float distanceV = (enemyYO - playerY) / distanceH;
                    float angleY = (atan(distanceV) * 180) / PI;
                    WriteProcessMemory(phandle, (LPVOID)(FoundStatic + 0x38), &angleY, sizeof(angleY), NULL);
                  


                    //calculate vertical distance and angle
                    /*
                    float distanceV = (enemyYO - playerY) / distanceH;
                    float angleY = (atan(distanceV) * 180) / PI;
                    */

                    std::cout << (enemyXO - playerX) << " " << (enemyZO - playerZ) << " " << horizontalangle << " " << zdistance / xdistance << "\n";
                    


                    /*
                    //vector test
                    
                    //enemy vector
                    float evectorx = (enemyXO - playerX);
                    float evectorz = (enemyZO - playerZ);
                    
                    //player vector
                    float pvectorx = cos(horizontalangle/180 * PI);
                    float pvectorz = sin(horizontalangle/180 * PI);
                    
                    float dotproduct = (evectorx * pvectorx) + (evectorz * pvectorz);
                    float magnitude = sqrt(pow(evectorx, 2) + pow(evectorz, 2)) * sqrt(pow(pvectorx, 2) + pow(pvectorz, 2));
                    float angleXZ = (acos(dotproduct / magnitude) * 180 / PI);

                    std::cout << angleXZ << "\n";
                    */


                    /*
                    WriteProcessMemory(phandle, (LPVOID)(FoundStatic + 0x34), &(angleXZ), sizeof(angleXZ), NULL);
                    
                    WriteProcessMemory(phandle, (LPVOID)(FoundStatic + 0x38), &angleY, sizeof(angleY), NULL);
                    */
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    
                }
                   
                    


            }



        }
        //loop through entity list

        
       
        


        
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}


