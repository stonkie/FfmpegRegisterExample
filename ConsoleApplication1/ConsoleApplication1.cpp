#include <fstream>
#include <iostream>
#include <windows.h>

enum class AVCodecID {
    AV_CODEC_ID_NONE,
    AV_CODEC_ID_MJPEG = 7,
};

struct AVPacket
{
    void* buf;
    long long pts;
    long long dts;
    char* data;
    int size;
};

typedef void* (__cdecl* find_decoder_proc)(AVCodecID);
typedef void* (__cdecl* alloc_context3_proc)(void*);
typedef int (__cdecl* open2_proc)(void*, void*, void*);
typedef AVPacket* (__cdecl* packet_alloc_proc)();
typedef AVPacket* (__cdecl* send_packet_proc)(void*, AVPacket*);

int main()
{
    double input1 = 1.1;
    double output1 = 0.0;
    __asm
    {
        fld input1;
        fstp output1;
    }

    std::wcout << L"Before, load " << input1 << " store : " << output1 << L"\r\n\r\n";
    
    HMODULE module = LoadLibraryW(L"avcodec-59.dll");

    const auto find_decoder = reinterpret_cast<find_decoder_proc>(GetProcAddress(module, "avcodec_find_decoder"));
    void* avcodec = find_decoder(AVCodecID::AV_CODEC_ID_MJPEG);
    
    const auto alloc_context3 = reinterpret_cast<alloc_context3_proc>(GetProcAddress(module, "avcodec_alloc_context3"));
    void* context = alloc_context3(avcodec);

    const auto open2 = reinterpret_cast<open2_proc>(GetProcAddress(module, "avcodec_open2"));
    const int error_code = open2(context, avcodec, nullptr);

    if (error_code < 0)
    {
        std::wcout << error_code;
        return error_code;
    }

    const auto packet_alloc = reinterpret_cast<packet_alloc_proc>(GetProcAddress(module, "av_packet_alloc"));
    AVPacket* packet = packet_alloc();

    std::ifstream file(L"1920x1080_yuv422.jpg", std::ios::binary | std::ios::ate);
    unsigned int size = static_cast<unsigned int>(file.tellg());
    file.seekg(0, std::ios::beg);

    char* buffer = new char[size];
    
    const std::ifstream::basic_istream& stream = file.read(buffer, size);
    if (stream.bad())
    {
        std::wcout << stream.rdstate();
        return error_code;
    }

    packet->data = buffer;
    packet->size = size;

    const auto send_packet = reinterpret_cast<send_packet_proc>(GetProcAddress(module, "avcodec_send_packet"));
    send_packet(context, packet);
    
    double input2 = 2.2;
    double output2 = 0.0;
    __asm
    {
        fld input2;
        fstp output2;
    }

    std::wcout << L"After, load " << input2 << " store : " << output2 << L"\r\n\r\n";

    return 0;
}
