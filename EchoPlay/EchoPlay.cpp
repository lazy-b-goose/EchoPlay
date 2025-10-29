#include <iostream>
#include <bass.h>
#include <Windows.h>

using namespace std;

void ValInterface(int valInt);
void Interface(int a, HSTREAM stream);

int main() {
    setlocale(LC_ALL, "RU");

    //cout << "Hello world" << std::endl;
    cout << string(48, ' ') << "EchoPlay v1.0.0f\n" << string(28, '\n');

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
           
    int val = 1;                                 //громкость звука
    BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, val * 0.01f);

    BASS_ChannelPlay(stream, TRUE);                 // Воспроизведение

    Interface(val, stream);
    //Управление 
    while (BASS_ChannelIsActive(stream)) {                                                                                    //нажат ли пробел
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {                                      //нажатие пробела  
            if (BASS_ChannelIsActive(stream) == BASS_ACTIVE_PLAYING) {                      //трек играет
                BASS_ChannelPause(stream);
                Interface(val, stream);
            }
            else if (BASS_ChannelIsActive(stream) == BASS_ACTIVE_PAUSED) {                  //трек не играет
                BASS_ChannelPlay(stream, FALSE);
                Interface(val, stream);
            }
            Sleep(150);
        }
        if (GetAsyncKeyState(VK_RETURN) & 0x8000) {                                     //нажатие Enter
            return 0;
        }
        if (GetAsyncKeyState(VK_ADD) & 0x8000) {                                        //нажатие +
            if (val < 10) {
                val += 1;
                BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, val*0.01f);
                Interface(val, stream);
            }
            Sleep(25);
        }
        if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000) {                                   //нажатие -
            if (val > 0) {
                val -= 1;
                BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, val * 0.01f);
                Interface(val, stream);
            }
            Sleep(25);
        }
        Sleep(50);
    }





    return 0;
}




//интерфейс текущей громкости
void ValInterface(int val) {                         
    char valSound[10];
    //сколько '=' нужно
    for (int i = 0; i < val;i++) {
        valSound[i] = '=';
    }
    for (int i = val; i < 10;i++) {
        valSound[i] = ' ';
    }
    //делаем таблицу
    cout << string(102, ' ') << string(12, '-') << '\n';
    cout << string(100, ' ') << "- |";
    for (int i = 0; i < 10;i++) {
        cout << valSound[i];
    }
    cout << "| +" << '\n';
    cout << string(102, ' ') << string(12, '-') << '\n';
}
//интерфейс
void Interface(int a, HSTREAM stream){
    cout << "\x1b[5A\r";            //вверх курсор
    //пауза
    cout << string(102, ' ');
    if (BASS_ChannelIsActive(stream) == BASS_ACTIVE_PLAYING) {                  //трек играет
        cout << "[>] \n";
    }
    else if (BASS_ChannelIsActive(stream) == BASS_ACTIVE_PAUSED) {              //трек на паузе
        cout << "[||]\n";
    }
    cout << '\n';
    ValInterface(a);                //звук
}