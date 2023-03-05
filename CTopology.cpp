#include "CTopology.h"
void CTopology::Qenopy(PCWSTR filepath, HWND videoHwnd) {
    m_videoHwnd = videoHwnd;
    MFCreateTopology(&m_pTopology);
    CreateMediaSource(filepath);
    CreateTopology();
}
void CTopology::CreateMediaSource(PCWSTR filepath) {
    MF_OBJECT_TYPE objectType = MF_OBJECT_INVALID;
    IMFSourceResolver* pSourceResolver;
    IUnknown* pSource;
    MFCreateSourceResolver(&pSourceResolver);
    pSourceResolver->CreateObjectFromURL(filepath, MF_RESOLUTION_MEDIASOURCE, nullptr,
        &objectType, &pSource);
    m_pSource = (IMFMediaSource*)pSource;
}
void CTopology::CreateTopology(void) {
    IMFPresentationDescriptor* pPresDescriptor;
    DWORD nSourceStreams{ 0 };
    m_pSource->CreatePresentationDescriptor(&pPresDescriptor);
    pPresDescriptor->GetStreamDescriptorCount(&nSourceStreams);
    for (DWORD x = 0; x < nSourceStreams; x++) {
        BuildTopology(pPresDescriptor, x);
    }
}
void CTopology::BuildTopology(IMFPresentationDescriptor* pPresDescriptor, DWORD nStream) {
    IMFStreamDescriptor* pStreamDescriptor;
    IMFTopologyNode* pSourceNode;
    IMFTopologyNode* pOutputNode;
    BOOL streamSelected = FALSE;
    pPresDescriptor->GetStreamDescriptorByIndex(nStream, &streamSelected, &pStreamDescriptor);
    if (streamSelected) {
        MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &pSourceNode);
        pSourceNode->SetUnknown(MF_TOPONODE_SOURCE, m_pSource);
        pSourceNode->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, pPresDescriptor);
        pSourceNode->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, pStreamDescriptor);
        IMFMediaTypeHandler* pHandler;
        IMFActivate* pRendererActivate{ nullptr };
        GUID majorType = GUID_NULL;
        pStreamDescriptor->GetMediaTypeHandler(&pHandler);
        pHandler->GetMajorType(&majorType);
        if (majorType == MFMediaType_Audio) {
            MFCreateAudioRendererActivate(&pRendererActivate);
        }
        else if (majorType == MFMediaType_Video) {
            MFCreateVideoRendererActivate(m_videoHwnd, &pRendererActivate);
        }
        MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &pOutputNode);
        pOutputNode->SetObject(pRendererActivate);
        m_pTopology->AddNode(pSourceNode);
        m_pTopology->AddNode(pOutputNode);
        pSourceNode->ConnectOutput(0, pOutputNode, 0);
    }
}
CTopology::~CTopology(void) {
    if (m_pSource != nullptr) {
        m_pSource->Shutdown();
        m_pSource->Release();
        m_pSource = nullptr;
    }
}
