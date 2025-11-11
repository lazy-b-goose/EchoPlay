#include <iostream>
#include <bass.h>
#include <Windows.h>

using namespace std;

void LongInterface(HCHANNEL stream, int TimeAll);
void PauseInterface(HCHANNEL stream);
void ValInterface(int valInt);
void TrackInterface(HCHANNEL stream, int TimeAll);
void Interface(int a, HSTREAM stream, int TimeAll);

void Pause(HSTREAM stream, int h);
bool TrekOn(HSTREAM& stream, string t);

int main() {
    setlocale(LC_ALL, "RU");

    //cout << "Hello world" << std::endl;

    // Инициализация BASS
    if (!BASS_Init(-1, 44100, 0, 0, NULL)) {
        cout << "Ошибка инициализации BASS\n";
        return 1;
    }
    HSTREAM stream;
    HSTREAM& STREAM = stream;

    cout << string(48, ' ') << "EchoPlay v1.1.1\n" << string(28, '\n');

    // Загрузка трека

    string t = "аудио/POLMATERI_-_YArche_zvjozd_73991388.mp3";
    TrekOn(STREAM, t);

    int val = 5;                                 //громкость звука
    BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, val * 0.01f);
    BASS_ChannelPlay(stream, TRUE);                 // Воспроизведение
    //время трека
    int TimeAll = BASS_ChannelBytes2Seconds(stream, BASS_ChannelGetLength(stream, BASS_POS_BYTE));      //общая длина

    Interface(val, stream, TimeAll);
    //Управление    
    int TimeI = 0;
    while (BASS_ChannelIsActive(stream)) {                              //нажат ли пробел
        TimeI++;
        if (GetAsyncKeyState(VK_SPACE) & 0x8000/*пробел*/) {                                      //нажатие пробела  
            if (BASS_ChannelIsActive(stream) == BASS_ACTIVE_PLAYING) {                      //трек играет
                BASS_ChannelPause(stream);
                Interface(val, stream, TimeAll);
            }
            else if (BASS_ChannelIsActive(stream) == BASS_ACTIVE_PAUSED) {                  //трек не играет
                BASS_ChannelPlay(stream, FALSE);
                Interface(val, stream, TimeAll);
            }
            Sleep(150);
        }
        if (GetAsyncKeyState(VK_RETURN) & 0x8000/*Enter*/) {                                     //нажатие Enter
            BASS_Free();
            return 0;
        }
        if (GetAsyncKeyState(VK_ADD) & 0x8000/* + */) {                                        //нажатие +
            if (val < 100) {
                val += 5;
                BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, val * 0.01f);
                Interface(val, stream, TimeAll);
            }
            Sleep(25);
        }
        if (GetAsyncKeyState(VK_SUBTRACT/* - */) & 0x8000) {                                   //нажатие -
            if (val > 0) {
                val -= 5;
                BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, val * 0.01f);
                Interface(val, stream, TimeAll);
            }
            Sleep(25);
        }
        if (GetAsyncKeyState(VK_RIGHT/* > */) & 0x8000) {
            Pause(stream, 150);
            int h = BASS_ChannelGetPosition(stream, BASS_POS_BYTE) + BASS_ChannelSeconds2Bytes(stream, 5);
            BASS_ChannelSetPosition(stream, h, BASS_POS_BYTE);
            BASS_ChannelPause(stream);
            Interface(val, stream, TimeAll);
        }
        if (GetAsyncKeyState(VK_LEFT/* < */) & 0x8000) {
            Pause(stream, 150);
            int h = BASS_ChannelGetPosition(stream, BASS_POS_BYTE) - BASS_ChannelSeconds2Bytes(stream, 5);
            BASS_ChannelSetPosition(stream, h, BASS_POS_BYTE);
            BASS_ChannelPause(stream);
            Interface(val, stream, TimeAll);
        }
        if (TimeI == 10) {
            Interface(val, stream, TimeAll);
            TimeI = 0;
        }
        Sleep(50);
    }




    BASS_Free();
    return 0;
}




//интерфейс длины трека
void LongInterface(HCHANNEL stream, int TimeAll) {
    int TimeNow = BASS_ChannelBytes2Seconds(stream, BASS_ChannelGetPosition(stream, BASS_POS_BYTE));          //время сейчас
    char TimeLong[90];
    //сколько '=' нужно
    for (int i = 0; i < (TimeNow * 90) / TimeAll; i++) {
        TimeLong[i] = '=';
    }
    for (int i = (TimeNow * 90) / TimeAll; i < 90;i++) {
        TimeLong[i] = '-';
    }
    //делаем линию
    cout << string(2, ' ');
    if (TimeNow % 60 < 10/*времени прошло*/) {
        cout << TimeNow / 60 << ".0" << TimeNow % 60;
    }
    else {
        cout << TimeNow / 60 << '.' << TimeNow % 60;
    }
    for (int i = 0; i < 90;i++) {
        cout << TimeLong[i];
    }
    if (TimeAll % 60 < 10/*времени всего*/) {
        cout << TimeAll / 60 << ".0" << TimeAll % 60;
    }
    else {
        cout << TimeAll / 60 << '.' << TimeAll % 60;
    }
}
//интерфейс паузы
void PauseInterface(HCHANNEL stream) {
    cout << string(2, ' ');
    if (BASS_ChannelIsActive(stream) == BASS_ACTIVE_PLAYING) {                  //трек играет
        cout << "[>] \n";
    }
    else if (BASS_ChannelIsActive(stream) == BASS_ACTIVE_PAUSED) {              //трек на паузе
        cout << "[||]\n";
    }
    cout << '\n';
}
//интерфейс громкости
void ValInterface(int val) {
    char valSound[20];
    //сколько '=' нужно
    for (int i = 0; i < val / 5;i++) {
        valSound[i] = '=';
    }
    for (int i = val / 5; i < 20;i++) {
        valSound[i] = ' ';
    }
    //делаем таблицу
    cout << string(92, ' ') << string(22, '-') << '\n';
    cout << string(90, ' ') << "- |";
    for (int i = 0; i < 20;i++) {
        cout << valSound[i];
    }
    cout << "| +" << '\n';
    cout << string(92, ' ') << string(22, '-') << '\n';
}


void TrackInterface(HCHANNEL stream, int TimeAll) {
    LongInterface(stream, TimeAll);
    PauseInterface(stream);
}

//интерфейс
void Interface(int a, HSTREAM stream, int TimeAll) {
    cout << "\x1b[5A\r";            //вверх курсор
    TrackInterface(stream, TimeAll);
    ValInterface(a);                //звук
}




//пауза на h времени
void Pause(HSTREAM stream, int h) {
    BASS_ChannelPause(stream);
    Sleep(h);
    BASS_ChannelPlay(stream, FALSE);
}

bool TrekOn(HSTREAM& stream, string t) {
    stream = BASS_StreamCreateFile(FALSE, t.c_str(), 0, 0, 0);
    if (!stream) {
        cout << "Ошибка загрузки трека\n";
        BASS_Free();
        return 1;
    }
}




