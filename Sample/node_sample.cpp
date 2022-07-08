#include <iostream>
#include <StaticCatsAudioLibrary.h>
#include <SCAL_Sound.h>
#include <SCAL_Node.h>

int main()
{
    // 1.������  
    
    // �����Initialize�̑O�ɌĂ�ł�������
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    if (!scal::Initialize())
    {
        return -1;
    }

    // 2.�m�[�h�̍쐬
    scal::Node node;

    if (!node.Create())
    {
        return -1;
    }

    //node.SetVolume(0.6f);
    //node.SetFilter(XAUDIO2_FILTER_TYPE::LowPassFilter, 0.5f, 1.0f);

    // 3.�T�E���h�̍쐬
    scal::Sound sound;

    sound.Load("Sound/test.wav");

    // 4.�T�E���h���m�[�h�ɏo��
    sound.AddOutputNode(&node);

    // 5.�T�E���h���Đ�
    sound.Play();

    std::cin.get();

    sound.Stop();

    return 0;
}