#pragma once
#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>
#include <windows.h>
#include <evr.h>
class CTopology
{
    IMFMediaSource* m_pSource;            
    IMFVideoDisplayControl* m_pVideoDisplay;
    HWND m_videoHwnd;                    
    void CreateMediaSource(PCWSTR);
    void CreateTopology(void);
    void BuildTopology(IMFPresentationDescriptor*,DWORD);
public:
    IMFTopology* m_pTopology;
    ~CTopology(void);
    void Qenopy(PCWSTR, HWND);
};

