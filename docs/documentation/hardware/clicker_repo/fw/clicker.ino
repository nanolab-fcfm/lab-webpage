#define A 5  // GPIO5 (D1 on NodeMCU)
#define B 4  // GPIO4 (D2 on NodeMCU)

String commandBuffer = "";  // Buffer to store incoming command
int CT = 0;  // Current Temperature
int TT = 0;  // Target Temperature
int pressDelay = 500; // Delay (ms) between button presses, adjustable manually

void setup() {
    pinMode(A, OUTPUT);
    digitalWrite(A, LOW);  // Ensure D1 (GPIO5) starts at 0V

    pinMode(B, OUTPUT);
    digitalWrite(B, LOW);  // Ensure D2 (GPIO4) starts at 0V

    Serial.begin(115200);  // Start serial communication
}

// Function to press a pin for 100ms (short press)
void shortPress(int pin) {
    digitalWrite(pin, HIGH);
    delay(100);
    digitalWrite(pin, LOW);
    delay(pressDelay); // Wait before next action
}

// Function to press a pin for 500ms (long press)
void longPress(int pin) {
    digitalWrite(pin, HIGH);
    delay(500);
    digitalWrite(pin, LOW);
    delay(pressDelay); // Wait before next action
}

// Function to press both A and B for 100ms simultaneously
void shortPressAB() {
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    delay(100);
    digitalWrite(A, LOW);
    digitalWrite(B, LOW);
    delay(pressDelay); // Wait before next action
}

// Function to round up to the next multiple of 10 correctly
int roundUpToNext10(int num) {
    if (num % 10 == 0) {
        return num + 10;  // If already at a multiple of 10, go to the next one
    } else {
        return ((num / 10) * 10) + 10;  // Otherwise, round up normally
    }
}

// Function to round down correctly based on whether the number is a multiple of 10
int roundDownCorrectly(int num) {
    if (num % 10 == 0) {
        return num - 10;  // If at a multiple of 10, move to the closest lower one
    } else {
        return ((num / 10) * 10) - 10;  // If not a multiple of 10, move two steps down
    }
}

// Function to automatically adjust from CT to TT (minimizing time)
void reachTargetTemperature() {
    // Call AB command 2 times first
    for (int i = 0; i < 2; i++) {
        shortPressAB();
    }

    shortPress(A);  // Step 1: Start with a short press of A
    shortPress(A);  // Step 2: Start with a short press of A

    while (CT != TT) {
        if (CT < TT) {
            while (CT + 10 <= TT) {
                int nextTemp = roundUpToNext10(CT);
                if (nextTemp > TT) break;  // Ensure we don't overshoot
                
                longPress(A);
                CT = nextTemp;
            }

            while (CT < TT) {
                shortPress(A);
                CT += 1;
            }
        } 
        else if (CT > TT) {
            while (CT - 10 >= TT) {
                int nextTemp = roundDownCorrectly(CT);
                if (nextTemp < TT) break;  // Ensure we don't overshoot
                
                longPress(B);
                CT = nextTemp;
            }

            while (CT > TT) {
                shortPress(B);
                CT -= 1;
            }
        }
    }

   
    // **Set Current Temperature to Target Temperature**
    CT = TT;
}

void loop() {
    while (Serial.available() > 0) {
        char incomingChar = Serial.read();

        // Ignore newline and carriage return characters
        if (incomingChar == '\n' || incomingChar == '\r') {
            if (commandBuffer.length() > 0) {
                processCommand(commandBuffer);
                commandBuffer = "";  // Clear buffer after processing
            }
            return;
        }

        commandBuffer += incomingChar;  // Append character to buffer
    }
}

void processCommand(String command) {
    if (command == "A") {
        shortPress(A);
    } else if (command == "AA") {
        longPress(A);
    } else if (command == "B") {
        shortPress(B);
    } else if (command == "BB") {
        longPress(B);
    } else if (command == "AB") {
        shortPressAB();
    } else if (command.startsWith("SCT")) {
        CT = command.substring(3).toInt();
    } else if (command.startsWith("STT")) {
        TT = command.substring(3).toInt();
    } else if (command == "RCT") {
        Serial.println(CT);
    } else if (command == "RTT") {
        Serial.println(TT);
    } else if (command == "GO") {
        reachTargetTemperature();
    } else if (command == "*IDN?") {
        Serial.println("ROSATECH,CLK100,P0000003,1.0.0");
    } else {
        Serial.print("ERROR! Unknown command: ");
        Serial.println(command);
    }
}
