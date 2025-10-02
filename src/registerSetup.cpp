#include <Arduino.h>
#include "registerSetup.h"
#include <DMAChannel.h>
#include <SD.h>

#define BUFFER_SIZE 1024*1  // Number of samples per buffer

// DMA Channel objects
DMAChannel dma1;
DMAChannel dma2;
// DMAChannel dma3;
// DMAChannel dma4;

uint8_t dma1_channel;
uint8_t dma2_channel;

// Buffer definitions - Use DMAMEM to avoid conflicts with QNEthernet memory pools
DMAMEM volatile uint16_t adcBuffer1[BUFFER_SIZE] __attribute__((aligned(4)));
DMAMEM volatile uint16_t adcBuffer2[BUFFER_SIZE] __attribute__((aligned(4)));
// DMAMEM volatile uint16_t adcBuffer3[BUFFER_SIZE] __attribute__((aligned(4)));
// DMAMEM volatile uint16_t adcBuffer4[BUFFER_SIZE] __attribute__((aligned(4)));

volatile bool useBuffer1 = true;
volatile bool buffer1Ready = false;
volatile bool buffer2Ready = false;
// volatile bool buffer3Ready = false;
// volatile bool buffer4Ready = false;
volatile uint32_t sampleCount = 0;

// SD Card and recording variables
bool sdCardReady = false;
bool isRecording = false;
File csvFile;
String currentFileName = "";
uint32_t recordingStartTime = 0;
uint32_t buffersRecorded = 0;

#define SD_CS_PIN BUILTIN_SDCARD  // Teensy 4.1 built-in SD card

volatile uint32_t bufferSwitchCount = 0;

// DMA interrupt handlers - Must use C linkage for proper vector table setup
void dma_ch1_isr() {

    // // Invalidate cache for completed buffer
    // arm_dcache_delete((void*)adcBuffer1, sizeof(adcBuffer1));
    // arm_dcache_delete((void*)adcBuffer2, sizeof(adcBuffer2));

    // Setup DMA2 for next buffer and disable DMA1
    dma1.disable();
    dma1.clearComplete();
    dma1.clearInterrupt();
    
    // dma2.destinationBuffer(adcBuffer2, BUFFER_SIZE);
    dma2.enable();

    // Serial.println("[IRQ] DMA CH1 TRIGGERED!");
    
    // Mark buffer 1 ready
    buffer1Ready = true;
    sampleCount += BUFFER_SIZE;
    
    // Debug: Show first few values
    Serial.print("ISR1 - First values: ");
    for (int i = 0; i < 8; i++) {
        Serial.print(adcBuffer1[i]);
        Serial.print(i < 7 ? "," : "\n");
    }
    
    // Serial.println("DMA1 -> DMA2");
}

void dma_ch2_isr() {

    // // Invalidate cache for completed buffer
    // arm_dcache_delete((void*)adcBuffer1, sizeof(adcBuffer1));
    // arm_dcache_delete((void*)adcBuffer2, sizeof(adcBuffer2));

    // Setup DMA1 for next buffer and disable DMA2
    dma2.disable();
    dma2.clearComplete();
    dma2.clearInterrupt();

    // dma1.destinationBuffer(adcBuffer1, BUFFER_SIZE);
    dma1.enable();

    // Serial.println("[IRQ] DMA CH2 TRIGGERED!");
    
    // Mark buffer 2 ready
    buffer2Ready = true;
    sampleCount += BUFFER_SIZE;
    
    // Debug: Show first few values
    Serial.print("ISR2 - First values: ");
    for (int i = 0; i < 8; i++) {
        Serial.print(adcBuffer2[i]);
        Serial.print(i < 7 ? "," : "\n");
    }
    
    // Serial.println("DMA2 -> DMA1");
}

void setupPIT_ADCETC_DMA() {
  // Clear ADC data buffers
  memset((void*)adcBuffer1, 0, sizeof(adcBuffer1));
  memset((void*)adcBuffer2, 0, sizeof(adcBuffer2));

  // // CRITICAL: Flush CPU cache so DMA sees cleared buffers
  // arm_dcache_flush((void*)adcBuffer1, sizeof(adcBuffer1));
  // arm_dcache_flush((void*)adcBuffer2, sizeof(adcBuffer2));
  
  // Initialize variables
  buffer1Ready = false;
  buffer2Ready = false;
  sampleCount = 0;
  
  Serial.println("Setting up PIT, ADC_ETC, and DMA...");
  
  // 1. Enable clocks
  CCM_CCGR1 |= CCM_CCGR1_ADC1(CCM_CCGR_ON);  // Enable ADC1 clock
  CCM_CCGR1 |= CCM_CCGR1_PIT(CCM_CCGR_ON);  // Enable PIT clock
  CCM_CCGR2 |= CCM_CCGR2_XBAR1(CCM_CCGR_ON);  // Enable XBAR clock  
  CCM_CCGR5 |= CCM_CCGR5_DMA(CCM_CCGR_ON);   // Enable DMA clock
  ARM_DEMCR |= ARM_DEMCR_TRCENA;
  ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;

  Serial.println("Clocks enabled");
  
  // 2. Configure XBAR to route PIT0 trigger to ADC_ETC
  XBARA1_SEL51 &= ~(0xFF << 8);  // Clear bits 15:8
//   XBARA1_SEL51 |= (XBARA1_IN_PIT_TRIGGER1 << 8);
  XBARA1_SEL0 |= (XBARA1_IN_PIT_TRIGGER1 << 0); // Route PIT1 trigger to XBAR1_0 output
  
  XBARA1_CTRL0 |= XBARA_CTRL_DEN0;
  XBARA1_CTRL0 |= XBARA_CTRL_EDGE0(0x1); // Rising edge trigger

  XBARA1_SEL0 |= (XBARA1_IN_PIT_TRIGGER1 << 8); // Route PIT1 trigger to XBAR1_0 output
  
  XBARA1_CTRL0 |= XBARA_CTRL_DEN1;
  XBARA1_CTRL0 |= XBARA_CTRL_EDGE1(0x1); // Rising edge trigger

  Serial.println("XBAR configured");
  
  // 3. Configure PIT for 100ms intervals  
  PIT_MCR &= ~PIT_MCR_MDIS;
  PIT_MCR &= ~PIT_MCR_FRZ;
  PIT_LDVAL1 = (int)2400;
  PIT_TCTRL1 |= PIT_TCTRL_TEN;

  PIT_LDVAL2 = 0x10000000;
  PIT_TCTRL2 |= PIT_TCTRL_TEN;
  PIT_TCTRL2 |= PIT_TCTRL_CHN;

  Serial.println("PIT configured");
  
  // 4. Configure ADC1
  ADC1_CFG = 0b00000000000000000000001000000000;
  ADC1_CFG |= ADC_CFG_ADTRG;
  ADC1_CFG &= ~ADC_CFG_OVWREN;
  ADC1_CFG |= ADC_CFG_MODE(0x2);
  ADC1_GC |= ADC_GC_DMAEN;
  ADC1_GC &= ~ADC_GC_AVGE;
  ADC1_HC0 = ADC_HC_ADCH(0b10000);

  Serial.println("ADC1 configured");
  
  // 5. Configure ADC_ETC properly
  ADC_ETC_CTRL &= ~ADC_ETC_CTRL_SOFTRST;  // Release reset
  ADC_ETC_CTRL |= ADC_ETC_CTRL_TRIG_ENABLE(0x1);  // Enable TRIG0
  ADC_ETC_CTRL &= ~ADC_ETC_CTRL_DMA_MODE_SEL;     // Use old DMA mode
  
  // Enable DMA for TRIG0
  ADC_ETC_DMA_CTRL |= ADC_ETC_DMA_CTRL_TRIQ_ENABLE(0x1);
  
  // Configure TRIG0 chain
  ADC_ETC_TRIG0_CTRL = 0;  // Clear control register
  ADC_ETC_TRIG0_CTRL |= ADC_ETC_TRIG_CTRL_TRIG_CHAIN(0);  // Single conversion
  
  // Configure the chain for ADC1
  ADC_ETC_TRIG0_CHAIN_1_0 = 0;  // Clear chain register
  ADC_ETC_TRIG0_CHAIN_1_0 |= ADC_ETC_TRIG_CHAIN_CSEL0(0x0);   // Select ADC1
  ADC_ETC_TRIG0_CHAIN_1_0 |= ADC_ETC_TRIG_CHAIN_HWTS0(0x1);   // Use hardware trigger source 1
  
  Serial.println("ADC_ETC configured - TRIG0 -> ADC1");
  
  // 6. Configure DMA channels using DMAChannel library - PIT Timer Test Mode  
  // Use PIT2 current value register (CVAL) which changes over time
  dma1.disable();
  dma1.clearComplete();
//   dma1.source(ADC_ETC_TRIG0_RESULT_1_0);  // PIT2 current value register 
  dma1.source(ARM_DWT_CYCCNT);  // PIT2 current value register 
  dma1.destinationBuffer(adcBuffer1, BUFFER_SIZE);
  dma1.transferSize(4);  // 16-bit transfers
  dma1.transferCount(BUFFER_SIZE);
  dma1.triggerAtHardwareEvent(DMAMUX_SOURCE_XBAR1_0);  // Use XBAR1_0 trigger
//   dma1.triggerAtHardwareEvent(DMAMUX_SOURCE_ADC1);
  dma1.attachInterrupt(dma_ch1_isr);
  dma1.interruptAtCompletion();
  dma1_channel = dma1.channel;
  // dma1.disableOnCompletion();  // Disable channel when done
  
  dma2.disable();  // Start disabled, will be enabled by DMA1 ISR
  dma2.clearComplete();
  // Setup DMA2 but keep it disabled initially
//   dma2.source(ADC_ETC_TRIG0_RESULT_1_0);  // PIT2 current value register 
  dma2.source(ARM_DWT_CYCCNT);  // PIT2 current value register
  dma2.destinationBuffer(adcBuffer2, BUFFER_SIZE);
  dma2.transferSize(4);  // 16-bit transfers
  dma2.transferCount(BUFFER_SIZE);
  dma2.triggerAtHardwareEvent(DMAMUX_SOURCE_XBAR1_1);  // Use XBAR1_0 trigger
//   dma2.triggerAtHardwareEvent(DMAMUX_SOURCE_ADC1); 
  dma2.attachInterrupt(dma_ch2_isr);
  dma2.interruptAtCompletion();
  dma2_channel = dma2.channel;
  // dma2.disableOnCompletion();  // Disable channel when done

//   dma3.source(ADC_ETC_TRIG0_RESULT_1_0);  // ADC result register
//   dma3.destinationBuffer(adcBuffer3, BUFFER_SIZE);
//   dma3.transferSize(2);  // 16-bit transfers
//   dma3.transferCount(BUFFER_SIZE);
//   dma3.triggerAtHardwareEvent(DMAMUX_SOURCE_ADC1);  // Use ADC_ETC trigger to read PIT values
//   dma3.disableOnCompletion();  // Disable channel when done
// //   dma3.attachInterrupt(dma_ch2_isr);
// //   dma3.interruptAtCompletion();

//   dma4.source(ADC_ETC_TRIG0_RESULT_1_0);  // ADC result register
//   dma4.destinationBuffer(adcBuffer4, BUFFER_SIZE);
//   dma4.transferSize(2);  // 16-bit transfers
//   dma4.transferCount(BUFFER_SIZE);
//   dma4.triggerAtHardwareEvent(DMAMUX_SOURCE_ADC1);  // Use ADC_ETC trigger to read PIT values
//   dma4.disableOnCompletion();  // Disable channel when done
// //   dma4.attachInterrupt(dma_ch2_isr);
// //   dma4.interruptAtCompletion();

  Serial.println("DMA channels configured with DMAChannel library");
  Serial.println("DMA1 channel: " + String(dma1_channel) + ", DMA2 channel: " + String(dma2_channel));
  Serial.println("DMA interrupts managed by DMAChannel library");
  
  // Test: Verify ISR functions are linked properly
  Serial.println("ISR functions: dma_ch1_isr and dma_ch2_isr attached");
  
  // Diagnostic: Check PIT timer status
  Serial.println("\n=== DIAGNOSTIC INFO ===");
  Serial.println("PIT_MCR: 0x" + String(PIT_MCR, HEX));
  Serial.println("PIT_TCTRL2: 0x" + String(PIT_TCTRL2, HEX));
  Serial.println("PIT_CVAL2: " + String(PIT_CVAL2));
  Serial.println("XBAR1_OUT0: 0x" + String(XBARA1_SEL0, HEX));
  Serial.println("DMA1 enabled: " + String(dma1.complete() ? "NO" : "YES"));
  Serial.println("========================\n");
  
  // Start with DMA channel 1
  dma1.enable(); // arm first buffer
  Serial.println("DMA channel 1 enabled - setup complete");
}

void regSetup() {
    static bool setupComplete = false;
    
    // Prevent multiple initialization
    if (setupComplete) {
        Serial.println("regSetup already called, skipping...");
        return;
    }
    
    Serial.println("Starting regSetup...");
    setupPIT_ADCETC_DMA();
    
    // Initialize SD Card
    Serial.println("Initializing SD card...");
    if (SD.begin(SD_CS_PIN)) {
        sdCardReady = true;
        Serial.println("SD card initialization successful!");
        
        // Create data directory if it doesn't exist
        if (!SD.exists("/data")) {
            SD.mkdir("/data");
            Serial.println("Created /data directory");
        }
    } else {
        Serial.println("SD card initialization failed!");
        sdCardReady = false;
    }
    
    setupComplete = true;
    Serial.println("Setup complete - ADC sampling every 100ms via PIT→ADC_ETC→DMA");
}

void regLoop() {
    // Check and process buffer 1 if ready
    if (buffer1Ready) {
        // Serial.println("Processing buffer 1...");
        // // Write to CSV if recording
        // if (isRecording && sdCardReady) {
        //     Serial.println("Writing buffer 1 to CSV");
        //     writeBufferToCSV(adcBuffer1, 1);
        //       // Debug: print first few values to check for zeros
        //       Serial.print("Buf1 head: ");
        //       for (int i = 0; i < 8; ++i) { Serial.print(adcBuffer1[i]); Serial.print(i<7?",":"\n"); }
        // } else {
        //     Serial.println("NOT writing - Recording: " + String(isRecording ? "Y" : "N") + ", SD: " + String(sdCardReady ? "Y" : "N"));
        // }
        buffer1Ready = false; // Mark as processed
    }
    
    // Check and process buffer 2 if ready
    if (buffer2Ready) {
        // Serial.println("Processing buffer 2...");
        // // Write to CSV if recording
        // if (isRecording && sdCardReady) {
        //     Serial.println("Writing buffer 2 to CSV");
        //     writeBufferToCSV(adcBuffer2, 2);
        //       // Debug: print first few values to check for zeros
        //       Serial.print("Buf2 head: ");
        //       for (int i = 0; i < 8; ++i) { Serial.print(adcBuffer2[i]); Serial.print(i<7?",":"\n"); }
        // } else {
        //     Serial.println("NOT writing - Recording: " + String(isRecording ? "Y" : "N") + ", SD: " + String(sdCardReady ? "Y" : "N"));
        // }
        buffer2Ready = false; // Mark as processed
    }
    
    // Debug: Show DMA status every 5 seconds
    static uint32_t lastDebugTime = 0;
    // if (millis() - lastDebugTime > 5000) {
    //     Serial.print("IRQ Status - Samples: ");
    //     Serial.print(sampleCount);
    //     Serial.print(", Buf1: ");
    //     Serial.print(buffer1Ready ? "RDY" : "---");
    //     Serial.print(", Buf2: ");
    //     Serial.print(buffer2Ready ? "RDY" : "---");
        
    //     // Check DMA channel status
    //     Serial.print(", CH1: ");
    //     Serial.print(dma1.complete() ? "DONE" : "RUN");
    //     Serial.print(", CH2: ");
    //     Serial.print(dma2.complete() ? "DONE" : "RUN");
        
    //     // Check if PIT timer is counting
    //     static uint32_t lastPitValue = 0;
    //     uint32_t currentPitValue = PIT_CVAL2;
    //     Serial.print(", PIT: ");
    //     Serial.print(currentPitValue != lastPitValue ? "COUNTING" : "STOPPED");
    //     lastPitValue = currentPitValue;
        
    //     Serial.println();
    //     lastDebugTime = millis();
    // }
}

// Start recording ADC data to CSV file
void startRecording() {
    if (!sdCardReady) {
        Serial.println("ERROR: SD card not ready!");
        return;
    }
    
    if (isRecording) {
        Serial.println("Already recording!");
        return;
    }
    
    // Create unique filename with timestamp
    recordingStartTime = millis();
    currentFileName = "/data/adc_data_" + String(recordingStartTime) + "test" + ".csv";
    
    // Create /data directory if it doesn't exist
    if (!SD.exists("/data")) {
        if (!SD.mkdir("/data")) {
            Serial.println("ERROR: Could not create /data directory");
            return;
        }
    }
    
    // Open file for writing
    csvFile = SD.open(currentFileName.c_str(), FILE_WRITE);
    if (!csvFile) {
        Serial.print("ERROR: Could not create file ");
        Serial.println(currentFileName);
        return;
    }
    
    // Write CSV header
    csvFile.println("hw_timestamp,buffer_num,sample_index,adc_value,voltage");
    csvFile.flush();
    
    isRecording = true;
    buffersRecorded = 0;
    
    Serial.println("Recording started: " + currentFileName);
    Serial.println("SD ready: " + String(sdCardReady ? "YES" : "NO"));
    Serial.println("Recording flag: " + String(isRecording ? "YES" : "NO"));
}

// Stop recording and close file
void stopRecording() {
    if (!isRecording) {
        Serial.println("Not currently recording.");
        return;
    }
    
    if (csvFile) {
        csvFile.close();
    }
    
    isRecording = false;
    
    Serial.println("Recording stopped: " + currentFileName);
    Serial.println("Total buffers recorded: " + String(buffersRecorded));
    Serial.println("Total recording time: " + String((millis() - recordingStartTime) / 1000.0) + " seconds");
}

// Write buffer data to CSV file
void writeBufferToCSV(volatile uint16_t buffer[BUFFER_SIZE], int bufferNum) {
    if (!isRecording || !csvFile) {
        return;
    }
    
    // Write each sample in the buffer to CSV
    for (int i = 0; i < BUFFER_SIZE; i++) {
        uint16_t hwTimestamp = buffer[i];  // Hardware timestamp from buffer[0]
        uint16_t adcValue = buffer[i];     // ADC value from DMA3 in buffer[1]
        float voltage = (adcValue * 3.3) / 4095.0;  // Convert to voltage (12-bit ADC, 3.3V reference)
        
        csvFile.print(hwTimestamp);
        csvFile.print(",");
        csvFile.print(bufferNum);
        csvFile.print(",");
        csvFile.print(i);
        csvFile.print(",");
        csvFile.print(adcValue);
        csvFile.print(",");
        csvFile.println(voltage, 4);  // 4 decimal places
    }
    
    csvFile.flush();  // Ensure data is written to SD card
    buffersRecorded++;
}

// Simple function to get list of CSV files
String getFileList() {
    String fileList = "";
    
    if (!sdCardReady) {
        return "{\"error\":\"SD card not ready\"}";
    }
    
    File dataDir = SD.open("/data");
    if (!dataDir || !dataDir.isDirectory()) {
        return "{\"files\":[]}";
    }
    
    fileList = "{\"files\":[";
    bool first = true;
    
    while (true) {
        File entry = dataDir.openNextFile();
        if (!entry) break;
        
        String fileName = entry.name();
        if (fileName.endsWith(".csv")) {
            if (!first) fileList += ",";
            fileList += "{\"name\":\"" + fileName + "\",\"size\":" + String(entry.size()) + "}";
            first = false;
        }
        entry.close();
    }
    dataDir.close();
    
    fileList += "]}";
    return fileList;
}

// Simple function to delete a file
bool deleteFile(const String &filename) {
    if (!sdCardReady || isRecording) {
        return false;
    }
    
    String fullPath = "/data/" + filename;
    return SD.remove(fullPath.c_str());
}

// Simple function to get recording status
String getRecordingStatus() {
    String status = "{";
    status += "\"recording\":" + String(isRecording ? "true" : "false");
    status += ",\"sdReady\":" + String(sdCardReady ? "true" : "false");
    if (isRecording) {
        status += ",\"filename\":\"" + currentFileName + "\"";
        status += ",\"buffersRecorded\":" + String(buffersRecorded);
        status += ",\"recordingTime\":" + String((millis() - recordingStartTime) / 1000.0);
    }
    status += "}";
    return status;
}