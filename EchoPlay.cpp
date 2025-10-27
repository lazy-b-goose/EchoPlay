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

    // Ожидание завершения

    cout << "Воспроизведение... Нажмите Enter для выхода\n";
    while (BASS_ChannelIsActive(stream)) {                                          //пока воспроизводиться                                                     //нажат ли пробел
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {                                  //нажатие пробела  
            if (BASS_ChannelIsActive(stream) == BASS_ACTIVE_PLAYING) {          //трек играет
                BASS_ChannelPause(stream);
                cout << "пауза\n";
            }
            else if (BASS_ChannelIsActive(stream) == BASS_ACTIVE_PAUSED) {          //трек не играет
                BASS_ChannelPlay(stream, FALSE);
                cout << "продолжить\n";
            }
        Sleep(100);
        }
        if (GetAsyncKeyState(VK_RETURN) & 0x8000) {                             //нажат ли Enter
            return 0;
        }
        Sleep(100);
    }
    return 0;
}