#pragma once
#include <windows.h>
#include <Mferror.h>
#include "CTopology.h"
#include <evr.h>
class MyPlayer
{
    void CreateSession();
    void CloseSession();
    CTopology m_topoBuilder{};
    IMFMediaSession* m_pSession{ nullptr };
    HWND m_hwndVideo;
public:
    MyPlayer(HWND);
    void OpenFile(PCWSTR);
    void Play();
    void Pause();
    void Repaint();
};

