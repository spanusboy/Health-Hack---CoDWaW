#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

#define PROCESS_NAME "CoDWaW.exe"
#define POLL_RATE 100
#define HEALTH_DRECREMENT_ADDRESS 0x004F31F4
#define HEALTH_DRECREMENT_ORIGINAL_OP_CODE "\x89\x96\xC8\x01\x00\x00"
#define HEALTH_DRECREMENT_NOP_OP_CODE "\x90\x90\x90\x90\x90\x90"
#define HEALTH_DRECREMENT_OP_CODE_LENGTH 6

DWORD get_process_id_by_process_name(const char* process_name_)
{
	PROCESSENTRY32 process_entry = {sizeof(PROCESSENTRY32)};
	HANDLE processes_snapshot;
	DWORD process_id = 0;

	//Procurando o processo do jogo
	processes_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (Process32First(processes_snapshot, &process_entry))
	{
		do
		{
			if (!strcmp(process_entry.szExeFile, process_name_))
			{
				process_id = process_entry.th32ProcessID;
				break;
			}
		} while (Process32Next(processes_snapshot, &process_entry));
	}

	//Limpando os recursos
	CloseHandle(processes_snapshot);

	return process_id;
}

int main(int argc_, char** argv_)
{
	//pegando o processo
	std::cout << "Aguardando o Prcesso Abrir." << std::endl;
	DWORD process = NULL;
	while (process == NULL)
	{
		process = get_process_id_by_process_name(PROCESS_NAME);
	}

	HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process);

	//checando se é valido o que foi pego
	if (process_handle != NULL)
	{
		bool hack_enable = false;
		while (true)
		{
			system("cls");
			std::cout << "Hack de Vida Infinita" << std::endl;
			std::cout << "=====================" << std::endl;
			if (!hack_enable)
			{
				std::cout << "[F8] - Vida Infinita" << std::endl;
			}
			else
			{
				std::cout << "[F8] - Remover Vida Infinita" << std::endl;
			}

			SHORT keypress = GetAsyncKeyState(VK_F8);
			if (keypress)
			{
				hack_enable = !hack_enable;

				if (hack_enable)
				{
					WriteProcessMemory(process_handle, (LPVOID)HEALTH_DRECREMENT_ADDRESS, HEALTH_DRECREMENT_NOP_OP_CODE, HEALTH_DRECREMENT_OP_CODE_LENGTH, NULL);
				}
				else
				{
					WriteProcessMemory(process_handle, (LPVOID)HEALTH_DRECREMENT_ADDRESS, HEALTH_DRECREMENT_ORIGINAL_OP_CODE, HEALTH_DRECREMENT_OP_CODE_LENGTH, NULL);
				}
			}
			Sleep(POLL_RATE);
		}
	}
}