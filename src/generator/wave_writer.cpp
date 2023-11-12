#include "generator/wave_writer.h"
#include "generator/byteswap.h"
#include <iostream>     // std::cout, std::ostream, std::ios
#include <fstream>      // std::filebuf
 
// WAVE Format: http://soundfile.sapp.org/doc/WaveFormat/
void WAVWriter::writeSamplesToBinaryStream(Sampler *sampler, std::ofstream *wavStream)
{
    DataSubChunk dataSubChunk;
    dataSubChunk.Subchunk2ID   = htobe32(0x64617461); // "data"
    dataSubChunk.Subchunk2Size = htole32(sampler->getSampleData().size() * sampler->getNumChannels() * sampler->getBitsPerSample()/8);

    // WTF MSFT: mixed big- and little endian in the *same* header structs? You've got to be kidding me...

    FmtSubChunk fmtSubChunk;
    fmtSubChunk.Subchunk1ID   = htole32(0x666d7420); // "fmt "
    fmtSubChunk.Subchunk1Size = htole32(16);         // size of the rest of this subchunk
    fmtSubChunk.AudioFormat   = htole32(1);          // PCM (i.e. linear quantization)
    fmtSubChunk.NumChannels   = htole32(sampler->getNumChannels());
    fmtSubChunk.SampleRate    = htole32(sampler->getSampleRateHz());
    fmtSubChunk.ByteRate      = htole32(sampler->getSampleRateHz() * sampler->getNumChannels() * sampler->getBitsPerSample()/8);
    fmtSubChunk.BlockAlign    = htole32(sampler->getNumChannels() * sampler->getBitsPerSample()/8);
    fmtSubChunk.BitsPerSample = htole32(sampler->getBitsPerSample());

    RIFFHeader riffHeader;
    riffHeader.ChunkID   = htobe32(0x52494646); // "RIFF"
    riffHeader.ChunkSize = htole32(4 + (8 + fmtSubChunk.Subchunk1Size) + (8 + dataSubChunk.Subchunk2Size));
    riffHeader.Format    = htobe32(0x57415645); // "WAVE"

    wavStream->write((char *)&riffHeader, sizeof(riffHeader));
    wavStream->write((char *)&fmtSubChunk, sizeof(fmtSubChunk));
    wavStream->write((char *)&dataSubChunk, sizeof(dataSubChunk));
    // C++ apparently guarantees, that the first element of a vector points to consecutive memory of the data
    wavStream->write((char *)&sampler->getSampleData()[0], sizeof(char)*sampler->getSampleData().size());
}

/* int main() {
    const int sampleRateHz    = 44100;    // number of samples per second
    const int numChannels     = 1;        // Mono
    const int bitsPerSample   = CHAR_BIT; // 8 bits
    const double volume       = 0.75;     // 0.0 .. 1.0
    const double noteDuration = 3;     // seconds

    PureToneGenerator pureTone     = PureToneGenerator();
    SquareWaveGenerator squareWave = SquareWaveGenerator();
    ViolinGenerator violin         = ViolinGenerator();
    ChirpGenerator chirp           = ChirpGenerator();

    NoEnvelope noEnvelope     = NoEnvelope();
    ADSREnvelope adsrEnvelope = ADSREnvelope(noteDuration);

    Sampler sampler = Sampler(sampleRateHz, bitsPerSample, numChannels);

    // Mary had a Little Lamb: http://www.choose-piano-lessons.com/piano-notes.html
    const int marySong[] =
    {
    //  Ma-----ry    had      a     lit----le    lamb
        E4,    D4,    C4,    D4,    E4,    E4,    E4,
    //  lit----le    lamb,   lit----le    lamb
        D4,    D4,    D4,    E4,    E4,    E4,
    //  Ma-----ry    had      a     lit----le    lamb
        E4,    D4,    C4,    D4,    E4,    E4,    E4,
    //  Its  fleece  was    white   as    snow.
        E4,    D4,    D4,    E4,    D4,    C4
    };
	
	const int marySong2[] =
    {
	C3, D3, F3 , D4,E4 , G4,A5, B5, C6,  D6};

    const int maryLength = sizeof(marySong2)/sizeof(marySong2[0]);

    // pure, sinusoidal tone; no envelope
    for(int i=0; i<maryLength; i++)
    {
        sampler.sample(&pureTone, marySong2[i], noteDuration, &noEnvelope, volume);
    }

     

    sampler.sample(&chirp, C4, noteDuration, &adsrEnvelope, volume);
    sampler.sample(&chirp, D4, noteDuration, &adsrEnvelope, volume);
    sampler.sample(&chirp, C5, noteDuration, &adsrEnvelope, volume);

    std::ofstream maryFile("output/mary.wav", std::ios::out | std::ios::binary);
    WAVWriter::writeSamplesToBinaryStream(&sampler, &maryFile);
    maryFile.close();
    std::cout << "Wrote output/mary.wav" << std::endl;

     

    return 0;
} */
