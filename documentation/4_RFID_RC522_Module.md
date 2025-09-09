<br>

<div style="display: flex; align-items: center; gap: 20px;">
  <div style="font-size: 2em; font-weight: bold;">
  Using the RFID MFRC522 (HW-126) Module with the MSPM0G3507
  </div>
  <div style="position: absolute; top: 20px; right: 50px;">
  <a href="../Readme.md" title="Go back" style="text-decoration: none; font-size: 32px;">↩</a>
  </div>  
</div>
<br>
<hr style="border: none; border-bottom: 4px solid #FFF; width: 100%; margin-top: 0;">

> Version 1.3   
> Date: Fall 2025 

---  

## 🎯 Introduction  
The RFID module in your kit is the **HW-126**, which uses the **MFRC522** chip to read data wirelessly from passive RFID cards and tags. RFID (Radio Frequency Identification) is commonly used in applications like access control, transit cards, and hotel keys.

This module operates at **13.56 MHz**, a standard frequency for short-range contactless communication. It supports **MIFARE** cards and tags, which include a **4-byte UID** and **1KB of memory**, split into 16 sectors of 4 blocks each.

The HW-126 is a **passive RFID system**, meaning the card has no battery; it receives power from the reader when it is brought near the antenna.

---

<br>

## 📦 Module Specifications  

| Feature                  | Description                               |
|--------------------------|-------------------------------------------|
| Frequency                | 13.56 MHz                                 |
| Communication Protocol   | SPI (Serial Peripheral Interface)         |
| Supported Cards          | MIFARE 1K                                 |
| Card Memory              | 1KB (16 sectors × 4 blocks)               |
| Reader IC                | MFRC522                                   |
| Rewrite Cycles           | Up to 100,000 times                       |
| Data Retention           | Up to 10 years                            |

---

<br>

## 📍 MSPM0G3507 Pin Connections  

While the MFRC522 (HW-126) RFID module is not used in the standard lab assignments, a support file has been created to allow you to explore its use with the **MSPM0G3507** microcontroller. The RFID module connects via **SPI1**, and the IRQ pin is not used in this setup. The following table shows the correct wiring configuration:

| RFID Module Pin | MSPM0G3507 Pin      | Description                    |
|------------------|---------------------|--------------------------------|
| 3.3V             | 3.3V                | Power supply (do not use 5V)  |
| GND              | GND                 | Ground                         |
| RST              | PB2                 | Manual reset for the reader   |
| IRQ              | Not connected       | Not used                       |
| MISO             | SPI1-MOSI           | Serial data from reader        |
| MOSI             | SPI1-MISO           | Serial data to reader          |
| SCK              | SPI1-SCK            | Clock signal                   |
| SDA              | SPI1-SSn            | Slave select (chip select)     |

> ⚠️ **Important:** The logic level for MFRC522 is **3.3V**. Make sure your microcontroller operates at 3.3V or use level shifters if necessary.

---

<br>

## How It Works  

1. The MSPM0G3507 initializes SPI and resets the RFID module.
2. The MFRC522 sends a signal at 13.56 MHz to detect nearby tags.
3. When a tag is detected, the reader receives its **UID (Unique Identifier)**.
4. The microcontroller can read or write data blocks on the tag after authentication.
5. Cards can be rewritten up to 100,000 times and store data for 10 years.

---

<br>

## Basic Workflow  

Typical operations performed with this module:

- **Initialization**: Set up SPI, reset the RFID module, and configure it.
- **Card Detection**: Wait for a tag to come into range.
- **UID Reading**: Read and display the unique identifier.
- **Authentication**: Use key-based access to secure memory blocks.
- **Data Read/Write**: Interact with specific data blocks on the card.

You’ll use the MSPM0’s SPI API to communicate with the MFRC522. C driver code are provided to abstract the register-level communication with the chip.

---

<br>

## Practical Applications  

- Secure door locks  
- Attendance logging systems  
- Inventory tracking  
- Personal identification systems  
- Simple e-wallet simulations  

---

## Additional Resources  

- **MFRC522 Datasheet (NXP)**:  
  [MFRC522 PDF](https://www.nxp.com/docs/en/data-sheet/MFRC522.pdf)

- **Application Note - RFID with MFRC522**:  
  [NXP RFID App Notes](https://www.nxp.com/products/rfid-nfc)

- **Freenove HW-126 Module Info**:  
  Check Freenove documentation included with your kit

<br>

## 📞 Contact Information

**Bruce Link**  
Monroe Community College  
Information & Computer Technologies Department  
📧 [blink@monroecc.edu](mailto:blink@monroecc.edu)