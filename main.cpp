#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#pragma comment(linker, "/Merge:.rdata=.text")
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <fstream>
#include <sstream>
// Prototyped Exe blocker for windows
//
// Currently using basic console window, will implement using
// windows API.
using std::cout;
using std::cin;

PROCESSENTRY32 entry;
HANDLE snapshot;
std::vector<std::string> exesToBlock;

void runBlockingExes(void);
void addExes(void);
void removeExes(void);
void exitProgram(void);
void displayExes(void);
void displayMenu(void);
void menuInput(void);
void menuValidation(char* user_choice);
void saveExes();
bool loadFile();
void displayBlockedExes(void);
int  main()
{
	SetConsoleTitle(("ExeBlocker | Chris Humm"));

	if (loadFile() == false)
		return 0;
	displayMenu();
	for (;;)
	{
		SleepEx(500, false);
		menuInput();
	}
	return 0;
}

bool loadFile()
{
	std::string exeRetrieved = "";
	std::ifstream blockedExes("exes.txt");
	if (blockedExes.is_open())
	{
		while (std::getline(blockedExes, exeRetrieved))
		{
			if (exeRetrieved != "")
			{
				exesToBlock.push_back(exeRetrieved);
				exeRetrieved = "";
			}
		}
		blockedExes.close();

	}
	else
	{
		std::ofstream newFile("exes.txt");
		if (newFile.is_open()) //is the file open?
		{
			newFile << ""; //write the file
			newFile.close(); //close it
		}
	}

	return true;


}
void displayExes(void)
{
	if (!(exesToBlock.size() == -0))
	{
		cout << "Current Blocked Exes\n";

		for (int i = 0; i < exesToBlock.size(); ++i)
		{
			cout << exesToBlock[i] + "\n";
		}
	}
}
void displayMenu(void)
{
	system("cls");
	cout << "1)Run Exe Blocker\n2)Add Blacklisted Exes\n3)Remove Blacklisted Exes\n4)Exit Program";
}

void menuValidation(char* user_choice)
{
	for (;;)
	{
		*user_choice = _getch();
		switch (*user_choice)
		{
		case 'y':
			return;
			break;
		case 'Y':
			return;
			break;
		case 'n':
			return;
			break;
		case 'N':
			return;
			break;
		default:
			break;
		}
	}
	return;
}
void menuInput(void)
{
	char user_choice = ' ';
	user_choice = _getch();
	if (user_choice != ' ' && isalpha(user_choice) == false)
	{
		switch (user_choice)
		{
		case '1':
			runBlockingExes();
			break;
		case '2':
			addExes();
			break;
		case '3':
			removeExes();
			break;
		case '4':
			exitProgram();
			break;
		}
	}
	else
	{
	}

}
void addExes(void)
{
	for (;;)
	{
		system("cls");
		displayExes();

		char user_choice = ' ';
		cout << "Do you want to block an exe? Y/N?";
		menuValidation(&user_choice);
		if (user_choice == 'y' || user_choice == 'Y')
		{
			cout << "\nWrite exe name: ";
			std::string exeName = "";
			cin >> exeName;

			bool added_exe = false;
			std::string::size_type pos = exeName.find(".exe");
			if (pos == std::string::npos)
			{
				cout << "\n\nNo extension found, was this a mistake? Y/N";
				menuValidation(&user_choice);
				if (user_choice == 'y' || user_choice == 'Y')
				{
					exeName.append(".exe");
				}
				else
				{
					cout << "\n\nDo you still want to continue? Y/N";
					menuValidation(&user_choice);
					if (user_choice == 'y' || user_choice == 'Y')
					{
						added_exe = true;
						saveExes();
					}
				}
			}
			else
			{
				added_exe = true;
				saveExes();
			}
			if (added_exe == true)
			{
				exesToBlock.push_back(exeName);
				added_exe = false;
			}
		}
		else
		{
			cout << "\n\n\n\nDo you wish to return to menu? Y/N";
			menuValidation(&user_choice);
			if (user_choice == 'y' || user_choice == 'Y')
			{
				displayMenu();
				return;
			}
		}
	}
}

void saveExes()
{
	std::string exeRetrieved = "";
	std::ifstream blockedExes("exes.txt");
	
	bool duplicate_found = false;
	if (blockedExes.is_open())
	{
		std::getline(blockedExes, exeRetrieved);

		if (exeRetrieved == "")
		{
			for (int i = 0; i < exesToBlock.size();++i)
			{
				if (exeRetrieved == exesToBlock[i])
				{
					duplicate_found = true;
					break;
				}
				if (duplicate_found == false) // save new exe
				{
					blockedExes.close();
					std::ofstream writeBlocked("exes.txt");
					writeBlocked << exesToBlock[i]; //write the file
					writeBlocked.close();
					duplicate_found = false;
				}
			}
		}
		else
		{
				blockedExes.close();
				blockedExes.open("exes.txt");
				std::stringstream buffer;
				buffer << blockedExes.rdbuf();

				std::string temp = buffer.str();

				for (int i = 0; i < exesToBlock.size();++i)
				{
					if (temp.find(exesToBlock[i]) == true)
					{
						duplicate_found = true;
					}
					else // save new exe
					{
						blockedExes.close();
						std::ofstream writeBlocked("exes.txt", std::ios_base::app);
						writeBlocked << exesToBlock[i] +"\n"; //write the file
						writeBlocked.close();
						duplicate_found = false;
					}
				}
			}
		}
		blockedExes.close();

	}

void removeExes(void) 
{ 
	system("cls"); 
	char user_choice = ' ';
	std::cout << "Remove EXEs TODO. Please exit :)";
	cout << "\n\n\n\nDo you wish to return to menu? Y/N";
	menuValidation(&user_choice);
	if (user_choice == 'y' || user_choice == 'Y')
	{
		displayMenu();
		return;
	}
}

void exitProgram(void) 
{ 
	exit(0);
}

void runBlockingExes(void)
{
	// implement quitting system
	system("cls");
	
	for (;;)
	{
		displayBlockedExes();
		entry.dwSize = sizeof(PROCESSENTRY32);
		SleepEx(100, 0);
		snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

		for (int i = 0; i < exesToBlock.size(); ++i)
		{
			if (Process32First(snapshot, &entry) == TRUE)
			{
				while (Process32Next(snapshot, &entry) == TRUE)
				{
					if (strcmp(entry.szExeFile, exesToBlock[i].c_str()) == 0)
					{
						CloseHandle(snapshot);
						snapshot = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);

#ifndef DEBUG
						OutputDebugStringA("\nTerminated ");
						OutputDebugStringA(exesToBlock[i].c_str());
#endif

						TerminateProcess(snapshot, 9);
						CloseHandle(snapshot);

					}
				}
			}
		}
	}
}

void displayBlockedExes()
{
	for(int i = 0; i < exesToBlock.size(); ++i)
	{
		std::cout << exesToBlock[i].c_str() << std::endl;
	}
	return;
}