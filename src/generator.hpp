#ifndef generator_h
#define generator_h

#include <Arduino.h>

template <int N>
class Generator
{
private:
    const char *_charset;
    int _charset_len;

    void reset() {
        for (int i = 0; i < N; i++) {
            _charset_indexes[i] = 0x00;
        }
        for (int i = 0; i < N+1; i++) {
            _buff[i] = 0x00;
        }
    }

    char _buff[N+1];
    byte _charset_indexes[N];

    bool _eos;

public:
    Generator(const char *charset) {
        _charset = charset;
        _charset_len = strlen(charset);
        _eos = false;
        reset();
    }

    Generator(const char *charset, const int (&init_charset_indexes)[N]) : Generator(charset) {
        for (int i = 0; i < N; i++) {
            _charset_indexes[i] = min(init_charset_indexes[i], _charset_len-1);
        } 
    }

    char *next() {
        for (int i = 0; i < N; i++) {
            _buff[i] = _charset[_charset_indexes[i]];
        } 
        
        for (int i = N-1; i >= 0; i--) {
            _charset_indexes[i]++;
            if (_charset_indexes[i] >= _charset_len) {
                _charset_indexes[i] = 0;
                if (i == 0) {
                    _eos = true;
                }
                continue;
            } else {
                break;
            }
        }

        return _buff;
    }

    bool isEndOfStream() {
        return _eos;
    }  
};

#endif /* generator_h */
