<br>

<div style="display: flex; align-items: center; gap: 20px;">
  <div style="font-size: 2em; font-weight: bold;">
  CSC202 Workspace Information
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
Welcome to **CSC-202: Programming Embedded Microcontrollers in C and Assembly** at Monroe Community College! This repository contains the essential files to set up your workspace for **Code Composer Studio - Theia (CCS-Theia)**, the Integrated Development Environment (IDE) you'll be using throughout the course.  

These files form the foundation of your embedded systems programming environment, enabling you to develop, build, debug, and test projects in both C and Assembly quickly and efficiently.

---

<br>

## 🛠️ Hardware
This workspace is tailored for the following hardware components:

- **Texas Instruments MSPM0G3507 LaunchPad (LP-MSPM0G3507)**
- **MCC CSC202 Expansion Board**
- **LCD1602 Module**
- **4x4 Matrix Keypad**

The LaunchPad and Expansion Board serve as the core hardware platforms for course exercises, providing microcontroller interfacing capabilities and I/O operations for your projects.

---

<br>

## 🖥️ Development Environment
The following software versions were used during the original development of this workspace. To avoid compatibility issues, please use these versions.

- Code Composer Studio – Theia Edition: Version 1.5.1
- MSPM0 SDK: Version 2.04.00.06
- TI ARM Clang Compiler: v4.0.0.LTS
- SysConfig: v1.21.1
- XDS110 Drivers: V3.0.0.32

⚠️ Different versions may cause build errors, debugging errors, or unexpected behavior.

---

<br>

## 💻 Setting Up Projects for CSC202 Lab Assignments
Within this workspace, you will find a default project named **"Default_project"**. This project includes all the critical support files and code tailored for CSC202 and **should not be modified directly**.  

To maintain the integrity of the default files and to organize your work effectively, **always duplicate this project and rename the copy** before starting any lab or assignment.

### ✏️ How to Duplicate and Rename Your Project

1. **Duplicate the Default Project:**
   - Right-click on **"Default_project"** within your CCS-Theia workspace.
   - Select **"Duplicate"** to create a copy of the project.

2. **Rename the Project:**
   - Right-click the newly duplicated project.
   - Select **"Rename"**.
   - Assign a unique name corresponding to your lab or assignment (e.g., **Lab2**).

3. **Rename the main.c File:**
   - In the duplicated project, locate **main.c**.
   - Right-click and choose **"Rename"**.
   - Rename it to something descriptive such as **lab2_main.c** to match the project name.

### 💡 Why Renaming is Important

- **Prevents Confusion:** Unique project and file names help avoid accidental edits to the wrong files.
- **Improves Organization:** Keeps your workspace tidy as you progress through multiple labs and assignments.
  
---

<br>

## 📞 Contact Information

Bruce Link  
Monroe Community College  
Information & Computer Technologies Department  
[blink@monroecc.edu](mailto:blink@monroecc.edu)
