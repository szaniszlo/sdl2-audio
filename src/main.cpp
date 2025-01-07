#include <SDL2/SDL.h>
#include <iostream>

const int SAMPLE_RATE = 44100;
const int BUFFER_SIZE = 4096;

typedef struct
{
    float current_step;
    float step_size;
    float volume;
} oscillator;

oscillator oscillate(float rate, float volume)
{
    oscillator o = {
        .current_step = 0,
        .volume = volume,
        .step_size = (2 * M_PI) / rate,
    };
    return o;
}

float next(oscillator *os)
{
    os->current_step += os->step_size; // TODO: take care of overflow
    return sinf(os->current_step) * os->volume;
}

float A2_freq = (float)SAMPLE_RATE / 110.00f;
oscillator A2_oscillator = oscillate(A2_freq, 0.8f);

float A3_freq = (float)SAMPLE_RATE / 220.00f;
oscillator A3_oscillator = oscillate(A3_freq, 0.8f);

float E4_freq = (float)SAMPLE_RATE / 329.63;
oscillator E4_oscillator = oscillate(E4_freq, 0.8f);

float A4_freq = (float)SAMPLE_RATE / 440.00f;
oscillator A4_oscillator = oscillate(A4_freq, 0.8f);

float C5_freq = (float)SAMPLE_RATE / 523.25;
oscillator C5_oscillator = oscillate(C5_freq, 0.8f);

void oscillator_callback(void *userdata, Uint8 *stream, int len)
{

    std::cout << "callback: len=" << len << std::endl;

    float factor = 0.2f;

    float *fstream = (float *)stream;
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        float v = next(&A2_oscillator) * factor;
        v += next(&A3_oscillator) * factor;
        v += next(&E4_oscillator) * factor;
        v += next(&A4_oscillator) * factor;
        v += next(&C5_oscillator) * factor;
        fstream[i] = v;
    }
}

int main()
{
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0)
    {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_AudioSpec spec = {
        .format = AUDIO_F32,
        .channels = 1,
        .freq = SAMPLE_RATE,
        .samples = BUFFER_SIZE,
        .callback = oscillator_callback,
    };

    if (SDL_OpenAudio(&spec, NULL) < 0)
    {
        printf("Failed to open Audio Device: %s\n", SDL_GetError());
        return 1;
    }

    SDL_PauseAudio(0);
    while (true)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                return 0;
            }
        }
    }
    return 0;
}
