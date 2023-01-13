const int kMiso = 12;
const int kMosi = 11;
const int kClk = 13;
const int kCs = 10;

#define DEBUG 1
#ifdef DEBUG
#define debug(...) Serial.print(__VA_ARGS__);
#define debugln(...) Serial.println(__VA_ARGS__);
#else
#define debug(...) ;
#define debugln(...) ;
#endif

void setup() {
  Serial.begin(57600);

  pinMode(kMiso, INPUT_PULLUP);
  pinMode(kMosi, OUTPUT);
  pinMode(kClk, OUTPUT);
  pinMode(kCs, OUTPUT);

  // disable chip
  digitalWrite(kCs, HIGH);
  digitalWrite(kMosi, HIGH);
  digitalWrite(kClk, LOW);

  delay(2);

  pulse_clock(100);

  // Chip enable
  digitalWrite(kCs, LOW);

  send_cmd_frame(0x40, 0x0, 0xc8);
  uint8_t buf = 0xff;

  buf = read_byte();
  debugln(buf, HEX);

  // Chip disable
  digitalWrite(kCs, HIGH);
}

void pulse_clock(int num) {
  while(num > 0) {
    digitalWrite(kClk, HIGH);
    digitalWrite(kClk, LOW);
    num--;
  }
}

void send_cmd_frame(uint8_t cmd, uint32_t arg, uint8_t crc) {
  write_byte(cmd);

  for (size_t i = 0; i < 4; i++) {
    write_byte(reinterpret_cast<uint8_t*>(&arg)[i]);
  }
  write_byte(crc);
}

void write_byte(uint8_t b) {
  debug("write_byte ");
  for (uint8_t mask = 0x80; mask >= 0x01; mask >>= 1) {
    if ((mask & b) == 0) {
      digitalWrite(kMosi, LOW);
      debug("0");
    } else {
      digitalWrite(kMosi, HIGH);
      debug("1");
    }
    digitalWrite(kClk, HIGH);
    // delay(1);
    digitalWrite(kClk, LOW);
    // delay(1);
  }
  debugln("");
}

uint8_t read_byte() {
  uint8_t b = 0x0;
  uint8_t mask = 0x80;
  digitalWrite(kMosi, HIGH);
  debug("read_byte: ");

  for (uint8_t mask = 0x80; mask >= 0x01; mask >>= 1) {
    digitalWrite(kClk, HIGH);
    if (digitalRead(kMiso) == HIGH) {
      debug("1");
      b |= mask;
    } else {
      debug("1");
    }
    // delay(1);
    digitalWrite(kClk, LOW);
    // delay(1);
  }

  debugln("");

  return b;
}

void loop() {
}
