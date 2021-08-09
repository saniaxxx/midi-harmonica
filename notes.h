/*
    BLE_MIDI_HARMONICA
*/

// Ниже определения нот в формате C=До D=Ре E=Ми F=Фа G=Соль A=Ля B=Си
// Строчная буква d=диез, b=бемоль. Цифра = номер октавы.
// Например Db2 = Ре бемоль 2й октавы
// Число после определения ноты - код данной ноты по стандарту MIDI

#define C1  0x18
#define Db1 0x19
#define D1  0x1A
#define Eb1 0x1B
#define E1  0x1C
#define F1  0x1D
#define Fd1 0x1E
#define G1  0x1F
#define Ab1 0x20
#define A1  0x21
#define Bb1 0x22
#define B1  0x23

#define C2  0x24
#define Db2 0x25
#define D2  0x26
#define Eb2 0x27
#define E2  0x28
#define F2  0x29
#define Fd2 0x2A
#define G2  0x2B
#define Ab2 0x2C
#define A2  0x2D
#define Bb2 0x2E
#define B2  0x2F

#define C3  0x30
#define Db3 0x31
#define D3  0x32
#define Eb3 0x33
#define E3  0x34
#define F3  0x35
#define Fd3 0x36
#define G3  0x37
#define Ab3 0x38
#define A3  0x39
#define Bb3 0x3A
#define B3  0x3B

#define C4  0x3C
#define Db4 0x3D
#define D4  0x3E
#define Eb4 0x3F
#define E4  0x40
#define F4  0x41
#define Fd4 0x42
#define G4  0x43
#define Ab4 0x44
#define A4  0x45
#define Bb4 0x46
#define B4  0x47

#define C5  0x48
#define Db5 0x49
#define D5  0x4A
#define Eb5 0x4B
#define E5  0x4C
#define F5  0x4D
#define Fd5 0x4E
#define G5  0x4F
#define Ab5 0x50
#define A5  0x51
#define Bb5 0x52
#define B5  0x53

#define C6  0x54
#define Db6 0x55
#define D6  0x56
#define Eb6 0x57
#define E6  0x58
#define F6  0x59
#define Fd6 0x5A
#define G6  0x5B
#define Ab6 0x5C
#define A6  0x5D
#define Bb6 0x5E
#define B6  0x5F

#define C7  0x60
#define Db7 0x61
#define D7  0x62
#define Eb7 0x63
#define E87 0x64
#define F7  0x65
#define Fd7 0x66
#define G7  0x67
#define Ab7 0x68
#define A7  0x69
#define Bb7 0x6A
#define B7  0x6B

#define C8  0x6C
#define Db8 0x6D
#define D8  0x6E
#define Eb8 0x6F
#define E8  0x70
#define F8  0x71
#define Fd8 0x72
#define G8  0x73
#define Ab8 0x74
#define A8  0x75
#define Bb8 0x76
#define B8  0x77

#define NNN 0xFF // Отсутствие ноты
