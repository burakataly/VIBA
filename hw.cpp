#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <ctime>
#include <cstdlib> 

#define HEADER 0xAA

using namespace std;

// 1. Dosyayý oku › her karakteri byte (unsigned char) olarak sakla
vector<unsigned char> readBinaryFile(const string& filename) {
    vector<unsigned char> bytes;
    ifstream file(filename, ios::binary); // binary modda açýyoruz

    if (!file.is_open()) {
        cerr << "Dosya açýlamadý: " << filename << endl;
        return bytes;
    }

    char ch;
    while (file.get(ch)) {
        bytes.push_back(static_cast<unsigned char>(ch));
    }

    file.close();
    return bytes;
}

// 2. Byte dizisini bit dizisine çevir
vector<bool> bytesToBits(const vector<unsigned char>& bytes) {
    vector<bool> bits;

    for (unsigned char byte : bytes) {
        bitset<8> b(byte); // byte › 8 bitlik bitset
        // En anlamlý bitten baþlayarak sýrayla push et
        for (int i = 7; i >= 0; --i) {
            bits.push_back(b[i]);
        }
    }

    return bits;
}

vector<vector<bool>> divideIntoFrames(const vector<bool>& bits, int frameSize = 100) {
    vector<vector<bool>> frames;
    int totalBits = bits.size();

    for (int i = 0; i < totalBits; i += frameSize) {
        vector<bool> frame;
        for (int j = i; j < i + frameSize && j < totalBits; ++j) {
            frame.push_back(bits[j]);
        }
        frames.push_back(frame);
    }

    return frames;
}

vector<bool> calculateCRC16(const vector<bool>& data, const vector<bool>& generator) {
    const int crcLength = 16;
    vector<bool> paddedData = data;

    for (int i = 0; i < crcLength; ++i)
        paddedData.push_back(0);

    vector<bool> remainder = paddedData;

    for (int i = 0; i <= remainder.size() - generator.size(); ++i) {
        if (remainder[i] == 1) {
            for (int j = 0; j < generator.size(); ++j) {
                remainder[i + j] = remainder[i + j] ^ generator[j];
            }
        }
    }

    vector<bool> crc(remainder.end() - crcLength, remainder.end());
    return crc;
}

vector<bool> computeChecksum(const vector<vector<bool>>& frames) {
    unsigned int total = 0;

    for (int i = 0; i < frames.size(); ++i) {
        const vector<bool>& frame = frames[i];
        int crcStart = frame.size() - 16;

        // CRC'yi al ve one's complement'ini hesapla
        unsigned int crc = 0;
        for (int j = 0; j < 16; ++j) {
            bool bit = frame[crcStart + j];
            bit = !bit; // One’s complement (bit tersle)
            crc = (crc << 1) | bit;
        }

        total += crc;
        total &= 0xFFFF; // 16 bit overflow
    }

    // Two’s complement alma: 1's complement + 1
    total = (~total + 1) & 0xFFFF;

    // Bit dizisine çevir
    vector<bool> checksumBits(16);
    for (int i = 15; i >= 0; --i) {
        checksumBits[i] = (total & 1);
        total >>= 1;
    }

    return checksumBits;
}

bool randomChance(int percent) {
    return (rand() % 100) < percent;
}

vector<bool> corruptFrame(const vector<bool>& frame) {
    vector<bool> corrupted = frame;

    // Rastgele 1 bit ters çevirelim
    int index = rand() % frame.size();
    corrupted[index] = !corrupted[index];

    return corrupted;
}

bool checkCRC(const vector<bool>& fullFrame, const vector<bool>& generator) {
    int crcLen = 16;
    vector<bool> payload(fullFrame.begin(), fullFrame.end() - crcLen);
    vector<bool> receivedCRC(fullFrame.end() - crcLen, fullFrame.end());

    vector<bool> calculatedCRC = calculateCRC16(payload, generator);

    return receivedCRC == calculatedCRC;
}

bool isChecksumFrame(const vector<bool>& frame) {
    if (frame.size() < 8) return false;

    unsigned char value = 0;
    for (int i = 0; i < 8; ++i) {
        value = (value << 1) | frame[i];
    }

    return value == HEADER;
}

bool stopAndWaitReceive(const vector<bool>& frame, const vector<bool>& generator) {
    if (isChecksumFrame(frame)) {
        // %5 ihtimalle checksum bozulur (simülasyon)
        if (randomChance(5)) {
            cout << "[Simulasyon] Checksum frame bozuldu!\n";
            return false;
        }

        // ACK kaybý simülasyonu
        if (randomChance(15)) {
            cout << "[Simulasyon] Checksum ACK kayboldu!\n";
            return false;
        }

        cout << "[Alici] Checksum ACK gonderildi.\n";
        return true;
    }

    // %20 bozulma
    vector<bool> frameToCheck = frame;
    if (randomChance(20)) {
        cout << "[Simulasyon] Frame bozuldu!\n";
        frameToCheck = corruptFrame(frame);
    }

    bool valid = checkCRC(frameToCheck, generator);
    if (!valid) {
        cout << "[Alici] CRC hatasi! Frame gecersiz.\n";
        return false;
    }

    if (randomChance(15)) {
        cout << "[Simulasyon] ACK kayboldu!\n";
        return false;
    }

    cout << "[Alici] ACK gonderildi. Frame dogru alindi.\n";
    return true;
}

void stopAndWaitSend(const vector<vector<bool>>& frames, const vector<bool>& generator, const vector<bool>& checksumFrame) {
    srand(time(0)); // random seed

    for (int i = 0; i < frames.size(); ++i) {
        bool acknowledged = false;

        while (!acknowledged) {
            cout << "\n[Gonderici] Frame " << i + 1 << " gonderiliyor...\n";

            if (randomChance(10)) {
                cout << "[Simulasyon] Frame kayboldu!\n";
                continue;
            }

            acknowledged = stopAndWaitReceive(frames[i], generator);
        }
    }

    // Checksum frame ayrý gönderiliyor
    cout << "\n[Gonderici] Checksum frame gonderiliyor...\n";

    bool checksumAck = false;
    while (!checksumAck) {
        if (randomChance(10)) {
            cout << "[Simulasyon] Checksum frame kayboldu!\n";
            continue;
        }

        checksumAck = stopAndWaitReceive(checksumFrame, {}); // generator boþ çünkü CRC yok
    }

    cout << "\nTum frame'ler ve checksum basariyla gonderildi.\n";
}

// TEST
int main() {
    string dosyaAdi = "UDHR.dat";
    vector<unsigned char> bytes = readBinaryFile(dosyaAdi);
    vector<bool> bits = bytesToBits(bytes);
    vector<vector<bool>> frames = divideIntoFrames(bits);

    cout << "Toplam frame sayisi: " << frames.size() << endl;

//    for (int i = 0; i < frames.size(); ++i) {
//        cout << "Frame " << i + 1 << " (bit sayisi: " << frames[i].size() << "): ";
//        for (int j = 0; j < frames[i].size(); ++j) {
//            cout << frames[i][j];
//        }
//        cout << endl;
//    }
    
    vector<bool> crc16_polynomial = {
	    1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1 // x^16 + x^12 + x^5 + 1
	};

//	vector<bool> crc16_polynomial = {
//	    1,1,0,1 // x^16 + x^12 + x^5 + 1
//	};
//	
//	vector<bool> data = {
//	    1,0,0,1,0,0
//	};
	
//	vector<bool> crc = calculateCRC16(data, crc16_polynomial);
//	
//	for (int j = 0; j < crc.size(); ++j) {
//        cout << crc[j];
//    }
    
	vector<vector<bool>> framesWithCRC;

	for (int i = 0; i < frames.size(); ++i) {
	    vector<bool> crc = calculateCRC16(frames[i], crc16_polynomial);
	
	    vector<bool> fullFrame = frames[i]; // kopyasýný al
	    fullFrame.insert(fullFrame.end(), crc.begin(), crc.end());
	
	    framesWithCRC.push_back(fullFrame);
	}
	
//	for (int i = 0; i < framesWithCRC.size(); ++i) {
//        cout << "Frame " << i + 1 << " (bit sayisi: " << framesWithCRC[i].size() << "): ";
//        for (int j = 0; j < framesWithCRC[i].size(); ++j) {
//            cout << framesWithCRC[i][j];
//        }
//        cout << endl;
//    }
	
	vector<bool> checksumBits = computeChecksum(framesWithCRC);
	vector<bool> header = {1,0,1,0,1,0,1,0}; // 0xAA
	vector<bool> checksumFrame;
	checksumFrame.insert(checksumFrame.end(), header.begin(), header.end());
	checksumFrame.insert(checksumFrame.end(), checksumBits.begin(), checksumBits.end());
	
	stopAndWaitSend(framesWithCRC, crc16_polynomial, checksumFrame);

    return 0;
}
