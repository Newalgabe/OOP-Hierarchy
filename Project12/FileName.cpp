#include <iostream>
#include <cstdio>

using namespace std;

class FileHandler 
{
public:
    void OpenFile(const char* path, const char* mode) 
    {
        if (fopen_s(&file, path, mode) == 0) 
            cout << "File opened successfully: " << path << endl;
        else
            cout << "Failed to open file: " << path << endl;
    }

    virtual void Display(const char* path)
    {
        if (file == nullptr) 
        {
            cout << "No file opened." << endl;
            return;
        }

        cout << "File contents: " << endl;
        char character;
        while (fread(&character, sizeof(char), 1, file) > 0) 
            cout << character;
        CloseFile();
    }

    void CloseFile() 
    {
        if (file != nullptr) 
        {
            fclose(file);
            file = nullptr;
        }
    }

protected:
    FILE* file = nullptr;
};

class ASCIIFileHandler : public FileHandler
{
public:
    void Display(const char* path) override
    {
        OpenFile(path, "r");
        if (file == nullptr)
            return;

        cout << "File contents as ASCII codes: " << endl;
        char character;
        while (fread(&character, sizeof(char), 1, file) > 0)
            cout << int(character) << " ";
        CloseFile();
    }
};

class BinaryFileHandler : public FileHandler
{
public:
    void Display(const char* path) override
    {
        OpenFile(path, "rb");
        if (file == nullptr) 
            return;

        cout << "File contents in binary: " << endl;
        unsigned char buffer[256];
        size_t bytesRead;
        while ((bytesRead = fread(buffer, sizeof(unsigned char), sizeof(buffer), file)) > 0) 
        {
            for (size_t i = 0; i < bytesRead; ++i)
                cout << unsigned(buffer[i]) << " ";
        }
        CloseFile();
    }

    void WriteBinaryData(const char* path, const unsigned char* data, size_t size) 
    {
        OpenFile(path, "wb");
        if (file == nullptr)
            return;

        fwrite(data, sizeof(unsigned char), size, file);
        CloseFile();
    }
};

int main() {
    ASCIIFileHandler asciiHandler;
    asciiHandler.Display("example.txt");

    BinaryFileHandler binaryHandler;
    binaryHandler.Display("example.bin");

    unsigned char data[] = { 0x48, 0x65, 0x6C, 0x6C, 0x6F };
    binaryHandler.WriteBinaryData("output.bin", data, sizeof(data));

    binaryHandler.Display("output.bin");

    return 0;
}
