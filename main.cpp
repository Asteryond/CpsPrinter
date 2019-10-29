#include <stdio.h>
#include <Windows.h>
#include <limits.h>
#include <stdlib.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

//检测按键是否被按下
#define isKeyPress(nVirtKey) ((GetKeyState(nVirtKey) & (1 << (sizeof(short) * 8 - 1))) != 0)
//检测按键是否触发
#define isKeyTgr(nVirtKey) ((GetKeyState(nVirtKey) & 1) != 0)

void clickKey(BYTE key);
void shift_key(BYTE key);

//点击某个按键
void printKey(BYTE key);

//模拟点击按键输出str
void printByClick(const char* str, int delay = 0);

//获取剪贴板文本内容
char* getClipBoardTxt();

void start()
{


	cout << "先复制需要粘贴的内容..." << endl;
	cout << "按CTRL + SHIFT + , 开始打印..." << endl;
	while (1)
	{
		if (isKeyPress(VK_CONTROL) && isKeyPress(VK_SHIFT) && isKeyPress(VK_OEM_COMMA))
		{
			Sleep(500);
			cout << "打印开始..." << endl;
			printByClick(getClipBoardTxt(),10);
			cout << "打印结束." << endl;
		}

	}
}
int main()
{
	start();
	return 0;
}

void clickKey(BYTE key)
{
	keybd_event(key, 0, 0, 0);
	keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
}

void shift_key(BYTE key)
{
	keybd_event(VK_SHIFT, 0, 0, 0);
	clickKey(key);
	keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
}
void printKey(BYTE key)
{
	//大写
	if (key >= 'A' && key <= 'Z')
	{
		if (isKeyTgr(VK_CAPITAL))
		{
			clickKey(toupper(key));
		}
		else
		{
			clickKey(VK_CAPITAL);
			clickKey(toupper(key));
			clickKey(VK_CAPITAL);
		}

	}
	//小写
	else if(key >= 'a' && key <= 'z')
	{
		if (isKeyTgr(VK_CAPITAL))
		{
			clickKey(VK_CAPITAL);
			clickKey(toupper(key));
		}
		else
		{
			clickKey(toupper(key));
		}
	}
	//处理其它符号
	else
	{
		//逗号
		switch (key)
		{
		case ',':
			clickKey(VK_OEM_COMMA);
			break;
		case '.' :
			clickKey(VK_OEM_PERIOD);
			break;
		case '\t':
			clickKey(VK_TAB);
			break;	
		case ' ':
			clickKey(VK_SPACE);
			break;
		case '\n':
			clickKey(VK_RETURN);
			break;
		case '\'':
			shift_key(VK_OEM_8);
			break;
		case '?':
			shift_key(VK_OEM_2);
			break;
		case '!':
			shift_key('1');
			break;
		}
	}
	

}

void printByClick(const char* str, int delay)
{
	for (int i = 0; i < strlen(str); i++)
	{
		printKey(str[i]);
		Sleep(delay);
	}
}


char* getClipBoardTxt()
{
	char* m_pBase = nullptr;

	if (OpenClipboard(NULL))
	{
		if (IsClipboardFormatAvailable(CF_TEXT))
		{
			HGLOBAL hGlobalClip = GetClipboardData(CF_TEXT);
			m_pBase = (char*)GlobalLock(hGlobalClip);
			if(m_pBase) GlobalUnlock(hGlobalClip);
			CloseClipboard();
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}
	return m_pBase;
}
