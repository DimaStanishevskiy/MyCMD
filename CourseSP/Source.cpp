#define _CRT_SECURE_NO_WARNINGS
#include <iostream> 
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <io.h>
using namespace std;

void Copy();
void Mkdir();
void Attrib();
void Time();
void Help();

char command[10][255];
char buff[255]{ 0 };
int main(int argc, char *argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	while (1)
	{

		for (int i = 0; i < 4; i++)
			strcpy(command[i], "");
		strcpy(buff, "");
		cin.clear();
		cin.sync();
		cout << "Command: "; cin.getline(buff, 254);

		int cnt = 0;
		for (int i = 0; buff[i] != '\0'; i++)
		{
			int k = 0;
			for (; (buff[i] != ' ') && (buff[i] != '\0'); i++)
			{
				if (buff[i] == '"')
				{
					for (; (buff[++i] != '"') && (buff[i] != '\0');)
					{
						command[cnt][k++] = buff[i];
					}
				}
				else
				{
					command[cnt][k++] = buff[i];
				}
			}
			if (!(k == 0))
			{
				command[cnt][k++] = '\0';
				++cnt;
			}
			if (buff[i] == '\0') break;
		}
		if (_stricmp(command[0], "Help") == 0)
		{
			Help();
		}
		else if (_stricmp(command[0], "Copy") == 0)
		{
			Copy();
		}
		else if (_stricmp(command[0], "Mkdir") == 0)
		{
			Mkdir();
		}
		else if (_stricmp(command[0], "Attrib") == 0)
		{
			Attrib();
		}
		else if (_stricmp(command[0], "Time") == 0)
		{
			Time();
		}
		else if (_stricmp(command[0], "Exit") == 0)
		{
			break;
		}
		else
		{
			cout << "Error" << endl;
		}
		//cin.get();
	}
	return 0;
}

void Time()
{
	if (!strcmp(command[1], "/?"))
	{
		cout << "Time [s] [""��� ���� ������� ����������""]" << endl;
	}
	else {
		if (!strcmp(command[1], "S"))
		{
			SYSTEMTIME Time;
			int masTime[4] = { -1,-1,-1, -1 };
			char *p;
			if (strcmp(command[2], ""))
			{
				p = strtok(command[2], ":");
				masTime[0] = atoi(p);
				for (int i = 1;(i < 4) && p; i++)
				{
					p = strtok('\0', ":");
					if (!p) break;
					else masTime[i] = atoi(p);
				}

				if ((masTime[2] > -1) && (masTime[3] == -1))
				{


					if ((masTime[0] > 24) || (masTime[1] > 59) || (masTime[2] > 59))
					{
						printf("%s", "���������� �������� ���\n");
					}
					else
					{
						GetLocalTime(&Time);
						Time.wHour = masTime[0];
						Time.wMinute = masTime[1];
						Time.wSecond = masTime[2];
						SetLocalTime(&Time);
						printf("%s", "��� ������ ��������\n");
					}
				}
				else cout << "�������" << endl;
			}
			else cout << "�������" << endl;
		}
		else
		{
			SYSTEMTIME Time;
			GetLocalTime(&Time);
			printf("%s %d:%d:%d%c", "��������� ���:", Time.wHour, Time.wMinute, Time.wSecond, '\n');
		}
	}

}
void Attrib()
{
	if (!strcmp(command[1], "/?"))
	{
		cout << "Attrib [+A|-A] [+H|-H] [+R|-R] [+I|-I]" << endl;
	}
	else
	{
		if (!_access(command[1], 0))
		{
			DWORD AttribFile = GetFileAttributes(command[1]);

			for (int i = 2; i < 11; i++)
			{
				if (!strcmp(command[i], "")) break;
				if (!strcmp(command[i], "+A")) AttribFile = AttribFile | FILE_ATTRIBUTE_ARCHIVE;
				else if (!strcmp(command[i], "-A")) AttribFile = AttribFile ^ FILE_ATTRIBUTE_ARCHIVE;
				else if (!strcmp(command[i], "+H")) AttribFile = AttribFile | FILE_ATTRIBUTE_HIDDEN;
				else if (!strcmp(command[i], "-H")) AttribFile = AttribFile ^ FILE_ATTRIBUTE_HIDDEN;
				else if (!strcmp(command[i], "+R")) AttribFile = AttribFile | FILE_ATTRIBUTE_READONLY;
				else if (!strcmp(command[i], "-R")) AttribFile = AttribFile ^ FILE_ATTRIBUTE_READONLY;
				else if (!strcmp(command[i], "+I")) AttribFile = AttribFile ^ FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
				else if (!strcmp(command[i], "-I")) AttribFile = AttribFile | FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
				else cout << "������� " << command[i] << "�� ����" << endl;
			}
			SetFileAttributes(command[1], AttribFile);
			cout << "�������� �����: ";
			if (AttribFile&FILE_ATTRIBUTE_ARCHIVE) cout << "�������� ";
			if (AttribFile&FILE_ATTRIBUTE_HIDDEN) cout << "���������� ";
			if (AttribFile&FILE_ATTRIBUTE_READONLY) cout << "�������������� ";
			if (!(AttribFile&FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)) cout << "������������ ";
			cout << endl;
		}
		else cout << "���� �� ����" << endl;
	}
}
void Mkdir()
{
	if (!strcmp(command[1], "/?"))
	{
		cout << "Mkdir ��������" << endl;
	}
	else
	{
		if (strcmp(command[1], ""))
		{
			if (CreateDirectory(command[1], NULL))
				cout << "������ �������� " << command[1] << endl;
			else
			{
				for (int i = 1;; i++)
				{
					char Buff[100];
					char tempName[255];
					strcpy(tempName, command[1]);
					strcat(tempName, "(");
					strcat(tempName, _itoa(i, Buff, 10));
					strcat(tempName, ")");
					if (CreateDirectory(tempName, NULL))
					{
						cout << "������ �������� " << tempName << endl;
						break;
					}

				}
			}
		}
		else cout << "��������� ��� ���������� ���" << endl;

	}
}
BOOL CopyDir(char indir[128], char outdir[128])
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	CreateDirectory(outdir, NULL);//����� � ��� ������ ��������
	char maska[128] = { 0 };
	strcat(maska, indir);
	strcat(maska, "/*");//��������� ���� �����;
	hFind = FindFirstFile(maska, &FindFileData);//����� �����
	if (hFind == NULL) return FALSE;
	do
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)// ���� �������� ����� � �� ����
		{
			if (strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, ".."))
			{
				//printf("Copy Directory  %s\n", FindFileData.cFileName);
				char indir2[128] = { 0 };
				char outdir2[128] = { 0 };
				strcat(indir2, indir);strcat(indir2, "/");strcat(indir2, FindFileData.cFileName);strcat(indir2, "/");
				strcat(outdir2, outdir);strcat(outdir2, "/");strcat(outdir2, FindFileData.cFileName);
				CopyDir(indir2, outdir2);//����������� ������ �������

			}
		}
		else//�������� ����
		{
			//printf("Copy File  %s\n", FindFileData.cFileName);
			char indir2[128] = { 0 };
			char outdir2[128] = { 0 };
			strcat(indir2, indir);strcat(indir2, "/");strcat(indir2, FindFileData.cFileName);
			strcat(outdir2, outdir);strcat(outdir2, "/");strcat(outdir2, FindFileData.cFileName);
			if (!CopyFile(indir2, outdir2, TRUE))
			{
				cout << "���� " << FindFileData.cFileName << " ����, �������(y/n): ";
				char ans;
				cin >> ans;
				if (ans == 'y')

					if (!CopyFile(indir2, outdir2, FALSE))
						cout << "�������� ��������";
			}//�������� ����
		}
	} while (FindNextFile(hFind, &FindFileData));//���� ���� ��� ����������
	FindClose(hFind);
	return TRUE;
}
void Copy()
{
	if (!strcmp(command[1], "/?"))
	{
		cout << "Copy ������������� �����������������" << endl;
	}
	else

		if ((!_access(command[1], 0)) && (!_access(command[2], 0)))
		{
			char indir[128]; strcpy(indir, command[1]);
			char outdir[128]; strcpy(outdir, command[2]);
			char chName[128];
			GetShortPathName(indir, chName, 128);

			WIN32_FIND_DATA FindFileData;
			FindFirstFile(indir, &FindFileData);
			strcat(outdir, "\\");
			strcat(outdir, FindFileData.cFileName);

			if (GetFileAttributes(indir) & FILE_ATTRIBUTE_DIRECTORY)
			{
				CopyDir(indir, outdir);
			}
			else
			{

				if (!CopyFile(indir, outdir, TRUE))
				{
					cout << "���� ����, �������(y/n): ";
					char ans;
					cin >> ans;
					if (ans == 'y')
						CopyFile(indir, outdir, FALSE);
				}
			}
			cout << "������ ����������" << endl;
			cin.get();
		}
		else  cout << "�������: ���� ���� �����������" << endl;
}
void Help()
{
	cout << "_________________________________________________" << endl;
	cout << "Time - ��������� �� ���� ��������� ����" << endl;
	cout << "Attrib - ��������� �� ���� �������� �����" << endl;
	cout << "Mkdir - ��������� �����" << endl;
	cout << "Copy - ��������� ����� ��� �����" << endl;
	cout << "_________________________________________________" << endl;
}