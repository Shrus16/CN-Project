// Define pin numbers for sensors
const int soilMoisturePin = A1;  // Analog pin for soil moisture sensor
const int temperaturePin = A0;   // Analog pin for temperature sensor

// Define pin numbers for H-bridge motor driver
const int motorEnablePin = 5;    // Enable pin for motor speed control
const int motorInput1 = 9;       // Input 1 for motor direction
const int motorInput2 = 10;       // Input 2 for motor direction

// Constants for water requirements
const int MIN_SOIL_MOISTURE = 30;  // Minimum acceptable soil moisture percentage
const int MAX_SOIL_MOISTURE = 70;  // Maximum acceptable soil moisture percentage
const int TEMPERATURE_THRESHOLD = 5;  // Temperature threshold for increased water requirements
const int BASE_WATER_REQUIREMENT = 10;  // Base water requirement for the crop
const float TEMPERATURE_FACTOR = 0.2;  // Factor for temperature impact on water requirement

void setup() {
  Serial.begin(9600);  // Initialize serial communication for debugging

  // Motor driver setup
  pinMode(motorEnablePin, OUTPUT);
  pinMode(motorInput1, OUTPUT);
  pinMode(motorInput2, OUTPUT);
}

void loop() {
  // Read soil moisture and temperature values
  int soilMoistureValue = analogRead(soilMoisturePin);
  int temperatureValue = analogRead(temperaturePin);

  // Convert analog values to actual sensor readings
  float soilMoisturePercentage = map(soilMoistureValue, 0, 1023, 0, 100);
  float temperatureCelsius = map(temperatureValue, 0, 1023, 0, 100);  // Assuming 0-50°C range

  // Print sensor readings to the Serial Monitor
  Serial.print("Soil Moisture: ");
  Serial.print(soilMoisturePercentage);
  Serial.print("%   |   Temperature: ");
  Serial.print(temperatureCelsius);
  Serial.println("°C");

  // Calculate water requirements
  int waterRequirements = calculateWaterRequirements(soilMoisturePercentage, temperatureCelsius);

  // Print water requirements to the Serial Monitor
  Serial.print("Water Requirements: ");
  Serial.print(waterRequirements);
  Serial.println(" units");
  // Motor control based on water requirements
  if (waterRequirements > 0) {
    // If additional water is needed, turn on the motor in one direction
    digitalWrite(motorInput1, HIGH);
    digitalWrite(motorInput2, LOW);
    analogWrite(motorEnablePin, 150);  // Adjust motor speed as needed
  } else {
    // Otherwise, turn off the motor
    digitalWrite(motorInput1, LOW);
    digitalWrite(motorInput2, LOW);
    analogWrite(motorEnablePin, 0);
  }

  // Add a delay between readings
  delay(1000);  // Adjust as needed
}

// Function to calculate water requirements
int calculateWaterRequirements(float soilMoisture, float temperature) {
  if (soilMoisture < MIN_SOIL_MOISTURE || soilMoisture > MAX_SOIL_MOISTURE) {
    return BASE_WATER_REQUIREMENT;  // Base water requirement if soil moisture is outside acceptable range
  } else {
    int waterRequirements = BASE_WATER_REQUIREMENT;

    // Adjust water requirement based on temperature
    if (temperature > TEMPERATURE_THRESHOLD) {
      waterRequirements += int(TEMPERATURE_FACTOR * (temperature - TEMPERATURE_THRESHOLD));
    }

    return waterRequirements;
  }
}

