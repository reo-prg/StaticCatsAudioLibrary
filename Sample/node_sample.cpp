#include <iostream>
#include <StaticCatsAudioLibrary.h>
#include <SCAL_Sound.h>
#include <SCAL_Node.h>

int main()
{
    // 1.初期化  
    
    // これはInitializeの前に呼んでください
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    if (!scal::Initialize())
    {
        return -1;
    }

    // 2.ノードの作成
    scal::Node node;

    if (!node.Create())
    {
        return -1;
    }

    //node.SetVolume(0.6f);
    //node.SetFilter(XAUDIO2_FILTER_TYPE::LowPassFilter, 0.5f, 1.0f);

    // 3.サウンドの作成
    scal::Sound sound;

    sound.Load("Sound/test.wav");

    // 4.サウンドをノードに出力
    sound.AddOutputNode(&node);

    // 5.サウンドを再生
    sound.Play();

    std::cin.get();

    sound.Stop();

    return 0;
}