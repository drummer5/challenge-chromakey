# Coding Challenge: Real-Time Chroma-Key Studio

## Overview
The goal of this challenge is to demonstrate your proficiency in **C++17 (or newer)**, the **Qt Framework**, and **OpenCV**. You will build a desktop application that allows a user to perform "Chroma-Keying" (green-screen replacement) on a video file in real-time.

## The Problem Statement
Create a "Virtual Background" application that replaces a specific color in a video stream with the contents of a still background image. This task is a proxy for how you solve problems in our codebase. Refrain from the use of LLMs for this task, as we are evaluating your personal familiarity with the tools.

---

## Key Technical Requirements

### 1. Functional Requirements
* **Media Loading:** Load a video file (`.mp4`) and a background image (`.png`) via the Qt UI.
* **Playback Control:** Provide Play, Pause, and Restart buttons to control the video view.
* **Interactive Selection:** Allow the user to click a pixel in the video feed to "pick" the key color.
* **Real-Time Processing:** Replace the key color with the background image dynamically as the video plays.

### 2. Technical Constraints
* **Framework:** Qt 6.x (preferred) or Qt 5.15.
* **Processing:** OpenCV 4.x.
* **Language:** C++17 or higher.
* **Threading:** The UI thread **must** remain responsive (non-blocking). Processing logic must run on a dedicated worker thread.
* **Memory Management:** Avoid unnecessary deep copies of image buffers every frame. Demonstrate proper use of RAII and QObject parent-child ownership.
* **Note on AI Tools:** You are welcome to use standard documentation and libraries. However, if you use generative AI (ChatGPT, Claude, etc.) to produce the solution, be aware that the subsequent technical interview will focus exclusively on the low-level "why" of your implementation. You will be expected to explain memory ownership, thread synchronization primitives, and OpenCV buffer management in extreme detail. If you cannot explain a line of code, it will be considered a failure of the task.

---

## Provided Assets
Please use the following assets for your development and testing:
* **Video:** `green_screen_countdown.mp4`
* **Background:** `background.png`
* This template project.

---

## Setup & Build Instructions

### Linux / macOS
Ensure you have CMake, Qt, and OpenCV installed via your package manager (`apt` or `brew`).

```bash
# Create build directory
mkdir build && cd build

# Configure and Build
cmake ..
make

# Run
./ChromaKeyStudio

```

### Windows (Recommended via vcpkg)

We recommend using `vcpkg` to manage dependencies to avoid pathing issues.

```powershell
# 1. Install dependencies
git clone https://github.com/Microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg install qt5:x64-windows opencv:x64-windows

# 2. Configure using the vcpkg toolchain
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE="C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake" -A x64

# 3. Build
cmake --build . --config Release

```

---

## Evaluation Rubric

Your submission will be graded on the following:

| Category | Criteria |
| --- | --- |
| **Qt & Architecture** | Thread safety, Signal/Slot usage, non-blocking UI, and layout management. |
| **Modern C++** | Use of C++17 features, type safety, and avoiding raw `new/delete`. |
| **Image Processing** | Choice of color space (e.g., HSV vs RGB), mask refinement, and efficiency. |
| **Code Quality** | Error handling (e.g., file not found), documentation, and build system cleanliness. |

---

## Submission Instructions

1. Ensure your code is well-commented and follows a consistent naming convention.
2. Provide a short write-up in your final submission explaining:
* Your architectural choices for threading and data transfer.
* Why you chose your specific image processing approach.


3. Include your `CMakeLists.txt` and all source files. Do **not** include build artifacts or binaries.

**Good luck! We look forward to seeing your solution.**