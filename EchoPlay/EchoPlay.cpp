#include <iostream>
#include <bass.h>
#include <Windows.h>

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");

    cout << "Hello world" << std::endl;

    // Инициализация BASS
    if (!BASS_Init(-1, 44100, 0, 0, NULL)) {
        cout << "Ошибка инициализации BASS\n";
        return 1;
    }

    // Загрузка трека
    HSTREAM stream = BASS_StreamCreateFile(FALSE, "audio\\POLMATERI_-_YArche_zvjozd_73991388.mp3", 0, 0, 0);
    if (!stream) {
        cout << "Ошибка загрузки трека\n";
        BASS_Free();
        return 1;
    }

    // Воспроизведение
    BASS_ChannelPlay(stream, TRUE);

    // Управление
    float val = 0.1f;
    BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, val);
    cout << "Воспроизведение... Нажмите Enter для выхода\n";
    while (BASS_ChannelIsActive(stream)) {                                          //пока воспроизводиться                                                     //нажат ли пробел
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {                                      //нажатие пробела  
            if (BASS_ChannelIsActive(stream) == BASS_ACTIVE_PLAYING) {                      //трек играет
                BASS_ChannelPause(stream);
                cout << "пауза\n";
            }
            else if (BASS_ChannelIsActive(stream) == BASS_ACTIVE_PAUSED) {                  //трек не играет
                BASS_ChannelPlay(stream, FALSE);
                cout << "продолжить\n";
            }
            Sleep(150);
        }
        Sleep(20);
        if (GetAsyncKeyState(VK_RETURN) & 0x8000) {                                     //нажатие Enter
            return 0;
        }
        Sleep(20);
        if (GetAsyncKeyState(VK_ADD) & 0x8000) {                                        //нажатие +
            if (val < 1) {
                val += 0.1f;
                BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, val);;
            }
            else {
                cout << "Максимальная громкость\n";
            }
        }
        Sleep(20);
        if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000) {                                   //нажатие -
            if (val > 1e-6) {
                val -= 0.1f;
                BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, val);
            }
            else {
                cout << "Минимальная громкость\n";
            }
        }
        Sleep(20);
    }



















    return 0;
}