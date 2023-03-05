#include "MyPlayer.h"
MyPlayer::MyPlayer(HWND nd) :m_hwndVideo(nd){}
void MyPlayer::OpenFile(PCWSTR sURL) {
    CreateSession();
    m_topoBuilder.Qenopy(sURL, m_hwndVideo);
    m_pSession->SetTopology(0, m_topoBuilder.m_pTopology);
}
void MyPlayer::Play(void) {
    PROPVARIANT varStart;
    PropVariantInit(&varStart);
    varStart.vt = VT_EMPTY;
    m_pSession->Start(&GUID_NULL, &varStart);
    PropVariantClear(&varStart);
}
void MyPlayer::Pause(void) {
    m_pSession->Pause();
}
void MyPlayer::CreateSession(void) {
    if (m_pSession != nullptr) {
        CloseSession();
    }
    MFCreateMediaSession(nullptr, &m_pSession);
}
void MyPlayer::CloseSession(void) {
    m_pSession->Close();
    m_pSession->Shutdown();
    m_pSession = nullptr;
}

