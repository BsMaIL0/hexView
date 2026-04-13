#include <iostream>
#include <fstream>
#include <iomanip>

int comp(char* s1, char* s2) {
  int i = 0;
  while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i]) {
    i++;
  }
  return s1[i] - s2[i];
}

void printHex(char* buf, int sz, unsigned int shift) {
  for (int i = 0; i < 8; i++) {
    if (i < sz) {
      unsigned char byte = static_cast<unsigned char>(buf[i]);
      std::cout << std::hex << std::setw(2) << std::setfill('0')
                << static_cast<int>(byte) << ' ';
    } else {
      std::cout << "   ";
    }
  }

  std::cout << "| ";

  for (int i = 8; i < 16; i++) {
    if (i < sz) {
      unsigned char byte = static_cast<unsigned char>(buf[i]);
      std::cout << std::hex << std::setw(2) << std::setfill('0')
                << static_cast<int>(byte) << ' ';
    } else {
      std::cout << "   ";
    }
  }

  std::cout << ' ';

  for (int i = 0; i < sz; i++) {
    unsigned char c = static_cast<unsigned char>(buf[i]);
    if (c < 32) {
      std::cout << '.';
    } else {
      std::cout << c;
    }
  }
}

int main(int argc, char** argv) {
  const char* inputFile = 0;
  long n = 0;
  int s = 0;
  unsigned int const sz = 16;

  for (int i = 1; i < argc; i++) {
    if (comp(argv[i], (char*)"-n") == 0) {
      if (i + 1 < argc) {
        n = atoi(argv[i + 1]);
        i++;
      }
    } else if (comp(argv[i], (char*)"-s") == 0) {
      if (i + 1 < argc) {
        s = atoi(argv[i + 1]);
        i++;
      }
    } else if (inputFile == 0) {
      inputFile = argv[i];
    }
  }

  std::ifstream in(inputFile, std::ios::binary);

  char buf[sz];
  int bytesReaded;

  in.seekg(0, std::ios::end); // идём в конец
  long fileSize = in.tellg(); // узнаём конец файла

  in.seekg(s, std::ios_base::beg); // идём на позицию с которой начинаем

  if (n > fileSize) {
    n = fileSize;
  } else if (n == 0) {
    n = fileSize;
    n -= s;
  }

  s = 0;

  while (!in.eof() && n > 0) {
    if (n < 16) {
      in.read(buf, n);
    } else {
      in.read(buf, sz);
    }
    bytesReaded = (int)in.gcount();
    std::cout << std::uppercase << std::setw(10) << std::setfill('0')
              << std::hex << s << ": ";
    printHex(buf, bytesReaded, s);
    s += bytesReaded;
    n -= bytesReaded;
    if (n != 0) {
      std::cout << '\n';
    }
  }
  return 0;
}