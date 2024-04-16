#pragma once
#include <sapi.h> //导入语音头文件
#include<iostream>
#include<Windows.h>
#include <string>
#include <atlstr.h>
#pragma comment(lib,"sapi.lib") //导入语音头文件库

LPCTSTR trans(std::string str) {//识别输入的语言
    CString cstr(str.c_str());
    if (LPCTSTR speakContent = (LPCTSTR)cstr) {
        return speakContent;
    }
    else {
        return(L"转化失败");
    }


}

void  MSSSpeak(LPCTSTR speakContent)// speakContent为LPCTSTR型的字符串，调用此函数即可将文字转为语音使用格式 MSSSpeak(trans(str);)
{
    ISpVoice* pVoice = NULL;

    //初始化COM接口

    if (FAILED(::CoInitialize(NULL)))
        MessageBox(NULL, (LPCWSTR)L"COM接口初始化失败！", (LPCWSTR)L"提示", MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);

    //获取SpVoice接口

    HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);


    if (SUCCEEDED(hr))
    {
        pVoice->SetVolume((USHORT)100); //设置音量，范围是 0 -100
        pVoice->SetRate(2); //设置速度，范围是 -10 - 10
        hr = pVoice->Speak(speakContent, 0, NULL);   //Speak函数的第二个参数如果设置成SPF_IS_FILENAME  则第一个参数指向一个文本(txt)文件

        pVoice->Release();

        pVoice = NULL;
    }

    //释放com资源
    ::CoUninitialize();
}
