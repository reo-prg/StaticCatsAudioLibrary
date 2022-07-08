#include <iostream>
#include <StaticCatsAudioLibrary.h>
#include <SCAL_Sound.h>

int main()
{
    // これはInitializeの前に呼んでください
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    if (!scal::Initialize())
    {
        return -1;
    }

    scal::Sound sound;

    sound.Load("Sound/test.wav");
    sound.Play();

    std::cin.get();

    sound.Stop();

    return 0;
}