#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

struct AudioData {
    int sampleRate;
    int durationMs;
    int amplitude;
    int frequency;
};

struct AudioGenerator {
    uint8_t* generate_audio(struct AudioData* audioData) {
        size_t bufferLength = (size_t)(audioData->durationMs * audioData->sampleRate * 2);
        uint8_t* buffer = (uint8_t*)malloc(bufferLength);
        double delta = 2.0 * M_PI * audioData->frequency / audioData->sampleRate;

        for (size_t i = 0; i < bufferLength / 2; i++) {
            short sample = (short)(audioData->amplitude * sin(delta * i));
            buffer[i * 2] = (uint8_t)(sample & 0xFF);
            buffer[i * 2 + 1] = (uint8_t)((sample >> 8) & 0xFF);
        }

        // Writing WAV header (assuming little-endian architecture)
        FILE* file = fopen("sound.wav", "wb");
        if (file) {
            fwrite("RIFF", 1, 4, file);
            
            uint32_t fileSize = 36 + (uint32_t)bufferLength;
            fwrite(&fileSize, sizeof(uint32_t), 1, file);
            fwrite("WAVEfmt ", 1, 8, file);
            
            uint32_t fmtSize = 16;
            fwrite(&fmtSize, sizeof(uint32_t), 1, file);
            
            uint16_t audioFormat = 1;
            fwrite(&audioFormat, sizeof(uint16_t), 1, file);
            
            uint16_t numChannels = 1;
            fwrite(&numChannels, sizeof(uint16_t), 1, file);
            fwrite(&(audioData->sampleRate), sizeof(uint32_t), 1, file);
            
            uint32_t byteRate = audioData->sampleRate * 2;
            fwrite(&byteRate, sizeof(uint32_t), 1, file);
            
            uint16_t blockAlign = 2;
            fwrite(&blockAlign, sizeof(uint16_t), 1, file);
            
            uint16_t bitsPerSample = 16;
            fwrite(&bitsPerSample, sizeof(uint16_t), 1, file);
            fwrite("data", 1, 4, file);
            fwrite(&bufferLength, sizeof(uint32_t), 1, file);
            fwrite(buffer, 1, bufferLength, file);
            
            fclose(file);
            
            printf("A wav file was successfully generated.\n");
        }
        else {
            printf("Error creating the WAV file.\n");
        }
        
        return buffer;
    }
};

int main() {
    struct AudioData audioData;
    struct AudioGenerator audioGenerator;
    
    audioData.sampleRate = 44100;
    audioData.durationMs = 1000;
    audioData.amplitude = 32760;
    audioData.frequency = 440;
    
    audioGenerator.generate_audio(&audioData);
    
    return 0;
}
