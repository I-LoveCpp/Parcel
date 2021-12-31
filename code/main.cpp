#include "Head.h"

using namespace std;

const string edition = "1.2";
string title = "Parcel " + edition + " Console";

const int NR = 6e4;
long double BPM = 50;

int play_music = 0;

bool running = false;
char ch;

int random_number()
{
	srand(time(NULL));
	return (rand() % 10000) + (rand() % 10000) * (rand() % 10000);
}

void init()
{
	SetConsoleTitleA(title.c_str());
	system("mode con cols=40 lines=19");
	HindCursor();
}

void record()
{
	ifstream in("D:\\Data\\Parcel\\BPM.Ceh", ios::binary);
	if (!in)
	{
		CreateDirectory(_T("D:\\Data\\"), NULL);
		SetFileAttributes(_T("D:\\Data\\"), FILE_ATTRIBUTE_HIDDEN);
		CreateDirectory(_T("D:\\Data\\Parcel\\"), NULL);
		ofstream out3("D:\\Data\\Parcel\\BPM.Ceh", ios::binary);
		out3 << "60";
	}
	else
	{
		if (MessageBoxA(0, "是否继承上次的记录", title.c_str(), MB_YESNO) == 6) in >> BPM;
		BPM = min(BPM, 200);
	}
}

void playsound()
{
	char buf[2000];
	char str[2000] = { 0 };
	int i = 0;
	//use mciSendCommand
	MCI_OPEN_PARMS mciOpen;
	MCIERROR mciError;
	//SetWindowText(NULL,"12345");
	mciOpen.lpstrDeviceType = L"mpegvideo";
	mciOpen.lpstrElementName = L"Dong.wav";
	mciError = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)&mciOpen);
	
	UINT DeviceID = mciOpen.wDeviceID;
	MCI_PLAY_PARMS mciPlay;
	mciError = mciSendCommand(DeviceID, MCI_PLAY, 0, (DWORD)&mciPlay);
}

void startsound()
{
	thread task10(playsound);
	task10.join();
}

void main_task()
{
	Button Continue = NewButton(8, 19, 7, "继续");
	while (1)
	{
		if (running == false)
		{
			system("cls");
			while (1)
			{
				if (running == true) break;
				if (Preserve(Continue))
				{
					system("cls");
					if(BPM < 200 && BPM != 50) BPM += random_number() % 10 + 5;
					running = true;
					break;
				}
				Sleep(50);
			}
			system("cls");
		}
		else
		{
			int cnt = random_number() % 10 + 10;
			while (1)
			{
				startsound();
				cnt--;
				Sleep(ceil(NR / BPM));
				if (KEY_DOWN(VK_LCONTROL)) running = false;
				if (cnt <= 0 && random_number() % 2 == 0) running = false;
				if (running == false) break;
			}
		}
	}
}

void get_key()
{
	while (1)
	{
		ch = _getch();
		if (ch == 13 || ch == 32)
		{
			system("cls");
			if (BPM < 200 && BPM != 50) BPM += random_number() % 10 + 5;
			running = true;
		}
		if (ch == 27) exit(0);
		Sleep(1);
	}
}

void show_bpm()
{
	while (1)
	{
		if (running)
		{
			if (BPM < 200) BPM += 1;
			ofstream out("D:\\Data\\Parcel\\BPM.Ceh", ios::binary);
			out << BPM;
			system("cls");
			gotoxy(8, 8);
			color(7);
			printf("BPM %.1lf", BPM);
			Sleep(500);
		}
		Sleep(1);
	}
}

int main()
{
	init();
	record();

	thread task01(main_task);
	thread task02(get_key);
	thread task03(show_bpm);

	task01.join();
	task02.join();
	task03.join();
	return 0;
}