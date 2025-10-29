#include <iostream>
#include <bass.h>
#include <Windows.h>
#include <cstdlib>

using namespace std;

void ValInterface(int valInt);
void Interface(int a, HSTREAM stream);

int main() {
    setlocale(LC_ALL, "RU");
#ifdef _WIN32
    system("chcp 65001 > nul");                     // Windows: включаем поддержку UTF-8
#endif

    //cout << "Hello world" << std::endl;
    
    cout << "Для завершения программы нажмите клавишу Enter\n";
    cout << "To end the program, press the key Enter";
    cout << string(28, '\n');

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


    float val = 0.1f;                               //громкость звука
    int valInt = 1;
    BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, val);

    BASS_ChannelPlay(stream, TRUE);                 // Воспроизведение

    //Управление 
    Interface(valInt, stream);
    while (BASS_ChannelIsActive(stream)) {                                                                                    //нажат ли пробел
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {                                      //нажатие пробела  
            if (BASS_ChannelIsActive(stream) == BASS_ACTIVE_PLAYING) {                      //трек играет
                BASS_ChannelPause(stream);
                Interface(valInt, stream);
            }
            else if (BASS_ChannelIsActive(stream) == BASS_ACTIVE_PAUSED) {                  //трек не играет
                BASS_ChannelPlay(stream, FALSE);
                Interface(valInt, stream);
            }
            Sleep(150);
        }
        Sleep(10);
        if (GetAsyncKeyState(VK_RETURN) & 0x8000) {                                     //нажатие Enter
            return 0;
        }
        Sleep(10);
        if (GetAsyncKeyState(VK_ADD) & 0x8000) {                                        //нажатие +
            if (val < 1) {
                val += 0.1f;
                valInt += 1;
                BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, val);
                Interface(valInt, stream);
            }
        }
        Sleep(10);
        if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000) {                                   //нажатие -
            if (val > 1e-6) {
                val -= 0.1f;
                valInt -= 1;
                BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, val);
                Interface(valInt, stream);
            }
        }
        Sleep(10);
    }





    return 0;
}




//интерфейс текущей громкости
void ValInterface(int valInt) {                         
    char valSound[10];
    //сколько '=' нужно
    for (int i = 0; i < valInt;i++) {
        valSound[i] = '=';
    }
    for (int i = valInt; i < 10;i++) {
        valSound[i] = ' ';
    }
    //делаем таблицу
    cout << string(102, ' ') << string(12, '-') << '\n';
    cout << "\r" << string(100, ' ') << "- |";
    for (int i = 0; i < 10;i++) {
        cout << valSound[i];
    }
    cout << "| +" << '\n';
    cout << string(102, ' ') << string(12, '\-') << '\n';
}
//интерфейс
void Interface(int a, HSTREAM stream){
    cout << "\x1b[5A\r";            //вверх курсор
    //пауза
    cout << string(102, ' ');
    if (BASS_ChannelIsActive(stream) == BASS_ACTIVE_PLAYING) {
        cout << "[>] \n";
    }
    else if (BASS_ChannelIsActive(stream) == BASS_ACTIVE_PAUSED) {
        cout << "[||]\n";
    }
    cout << '\n';
    ValInterface(a);                //звук
}