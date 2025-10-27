#include <iostream>
#include <bass.h>

using namespace std;

int main() {
    setlocale(LC_ALL, "RU");

    std::cout << "Hello world" << std::endl;

    // Инициализация BASS
    if (!BASS_Init(-1, 44100, 0, 0, NULL)) {
        printf("Ошибка инициализации BASS\n");
        return 1;
    }

    // Загрузка трека
    HSTREAM stream = BASS_StreamCreateFile(FALSE, "audio\\POLMATERI_-_YArche_zvjozd_73991388.mp3", 0, 0, 0);
    if (!stream) {
        printf("Ошибка загрузки трека\n");
        BASS_Free();
        return 1;
    }

    // Воспроизведение
    BASS_ChannelPlay(stream, FALSE);

    // Ожидание завершения (или можно использовать цикл)
    printf("Воспроизведение... Нажмите Enter для выхода\n");
    getchar();

    // Очистка
    BASS_Free();
    return 0;

    return 0;
}