# GigaLearnCPP Setup Guide 

# ⚠️ FOLLOW THIS GUIDE ALL THE WAY THROUGH! If you skipped a step/didn't read an instruction, and then you decide to ask me for help, I'm going to be very mad at you. ⚠️

## IMPORTANT
**If you do not understand how gigalearn works, it is VERY IMPORTANT to watch this tutorial to get an idea of how gigalearn works and some basic machine learning concepts. I can give you all the guides in the universe, but if your monkey brain does not understand a single thing, bot making will be very hard: 
[Introduction On Gigalearn](https://www.youtube.com/watch?v=QprErElWT-E)**

This guide will walk you through setting up **GigaLearnCPP** – a high‑performance C++ PPO training framework for Rocket League – entirely from the command line. You will build the training executable, run your first training session, and optionally render the bot’s gameplay.

**Who this guide is targeted towards:** Users with a clean Windows machine, comfortable with the command prompt. No prior C++ or Python experience is required, but basic familiarity with them is VERY helpful.

**Important:** This guide is written for **Python 3.11** and the **CPU version of libtorch**. If you have an NVIDIA GPU, you can follow the CUDA version notes.

----------

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Installation Steps](#installation-steps)
    * [2.1 Install Visual Studio Build Tools](#21-install-visual-studio-build-tools)
    * [2.2 Install CMake](#22-install-cmake)
    * [2.3 Install Git](#23-install-git)
    * [2.4 Install Python 3.11](#24-install-python-311)
    * [2.5 (Optional) Install CUDA for GPU training](#25-optional-install-cuda-for-gpu-training)
    * [2.6 Download RocketSim Collision Meshes](#26-download-rocketsim-collision-meshes)
3. [Cloning the Repository](#cloning-the-repository)
4. [Setting Up Python Virtual Environment](#setting-up-python-virtual-environment)
5. [Installing Required Python Packages](#installing-required-python-packages)
6. [Downloading and Placing LibTorch](#downloading-and-placing-libtorch)
7. [Building GigaLearnCPP](#building-gigalearncpp)
8. [Running Training](#running-training)
9. [Rendering the Bot](#rendering-the-bot)
10. [Understanding Checkpoints and Fresh Runs](#understanding-checkpoints-and-fresh-runs)
11. [Metrics and Skill Tracker](#metrics-and-skill-tracker)
12. [Common Errors and Troubleshooting](#common-errors-and-troubleshooting)
13. [Final Notes and Resources](#final-notes-and-resources)
14. [Final Advice](#Final-Advice)
    

----------

## Prerequisites

Before starting, ensure your system meets the following requirements:

-   **Windows 10/11** (64‑bit)
    
-   **Administrator rights** (to install software)
    
-   **Internet connection** (to download tools and dependencies)
    
-   **At least 8 GB RAM** (16+ GB recommended for training) 
    
-   **10 GB free disk space** (for the repository, libtorch, and checkpoints)    

----------

## Installation Steps

### 2.1 Install Visual Studio Build Tools

GigaLearnCPP requires a modern C++ compiler. The easiest way is to install the **Build Tools for Visual Studio 2026**.

1.  Download the installer from: [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2026)
    
2.  Run the installer and select **Desktop development with C++**.
    
3.  This will install MSVC compiler, CMake (optional, but we’ll install CMake separately), and the Windows SDK.
    
4.  Complete the installation and **restart your computer**.
    

**Alternative:** If you already have Visual Studio 2022 Community/Pro with C++ tools, you can skip this step.

### 2.2 Install CMake

CMake is used to generate the build files.

1.  Download the **Windows x64 Installer** from: [https://cmake.org/download/](https://cmake.org/download/)
    
2.  Run the installer.
    
3.  At the “Install Options” screen, check **“Add CMake to the system PATH for all users”**.
    
4.  Choose the default installation location.
    
5.  After installation, **close and reopen** your command prompt for the PATH changes to take effect.
    
6.  Verify CMake is available:
    
```
cmake --version
```

You should see something like `cmake version 3.28.x`.

### 2.3 Install Git

Git is needed to clone the repository.

1.  Download from: [Git For Windows](https://git-scm.com/download/win)
    
2.  Use the default settings during installation (add to PATH is automatically selected).
    
3.  Verify:
    
```
git --version
```

### 2.4 Install Python 3.11

We will use **Python 3.11** because it is the most stable and tested version for GigaLearnCPP.

1.  Download the Python 3.11 installer from: [Python 3.11 Installer Page](https://www.python.org/downloads/release/python-3110/)
    
2.  (Choose the **Windows installer (64-bit)**)
    
3.  **Crucial:** During installation, check **“Add Python to PATH”** at the bottom of the first screen.
    
4.  Then click **“Install Now”**.
    
5.  After installation, open a **new** command prompt and verify:
    
```
python --version
```

Output should be `Python 3.11.x`.

**Note:** If you have multiple Python versions, ensure `python` points to 3.11. You can also use `py -3.11` but we assume `python` works.

**Important ‼️**

If you ***already have python*** installed, check your **system environment variables** by typing `Edit the system environment variables` in the search bar and clicking on `Environment Settings` . Then double click on **path** (for the **user** that you are in) and make sure that `python 3.11` is in there. It should look something like this: `C:\Users\UserName\AppData\Local\Programs\Python\Python311\Scripts\` & `C:\Users\yiric\AppData\Local\Programs\Python\Python311\`. Make sure **both** of these are at the top of the **path**, the `Python311\Scripts\` folder should go above the main `Python311` folder in your **path**.

### 2.5 (Optional) Install CUDA for GPU Training

If you have an **NVIDIA GPU** (Compute Capability 3.5+), you can install CUDA to train much faster. Skip this section if you only have a CPU.

1.  Download CUDA 12.8 from: [Cuda Download](https://developer.nvidia.com/cuda-12-8-0-download-archive)
    
2.  Choose **Windows → x86_64 → exe (local)**.
    
3.  Run the installer and use the default settings. **Restart** your PC after installation.
    
4.  Verify CUDA is installed:  

```
nvcc --version
```

### 2.6 Download RocketSim Collision Meshes

RocketSim requires mesh files for the arena collision detection. These are not included in the repository.

1.  Go to the **RLArenaCollisionDumper** repository: [CollisionMeshDumper Github](https://github.com/ZealanL/RLArenaCollisionDumper)
    
2.  Follow the instructions there to **download** the `collision_meshes` folder.
    
4.  You will get a folder named `collision_meshes` containing `.cmf` files.
    
5.  Place this `collision_meshes` folder **inside the root of the GigaLearnCPP repository** after cloning. We will do that later.
    

----------

## Cloning the Repository

Open a **command prompt** (as normal user, not administrator) and navigate to the directory where you want to keep the code. For example:

```
cd C:\Users\YourUsername\Onedrive\Documents
mkdir RLBot
cd RLBot
```

Now clone the repository **recursively** (this will also download submodules):

```
git clone https://github.com/ZealanL/GigaLearnCPP-Leak.git --recurse-submodules
```

This may take a minute. After completion, you will have a folder named `GigaLearnCPP-Leak`. Move into that folder:

```
cd GigaLearnCPP-Leak
```

Place the `collision_meshes` folder here (copy or move it into `GigaLearnCPP-Leak`). The final path should be `GigaLearnCPP-Leak\collision_meshes`.

----------

## Setting Up Python Virtual Environment

To avoid conflicts with other Python projects, we create a virtual environment inside the repository.

```
python -m venv gigalearn_env
```

Activate it:

```
gigalearn_env\Scripts\activate
```

Your prompt should now show `(gigalearn_env)`. All subsequent Python commands will use this isolated environment.

**Tip:** To deactivate later, just type `deactivate`.

----------

## Installing Required Python Packages

With the virtual environment active, install the needed packages:

```
pip install wandb numpy matplotlib
```

-   **wandb** – for logging metrics (optional but recommended).
    
-   **numpy, matplotlib** – used by metric receiver scripts.
    

**Note:** If you don’t plan to use Weights & Biases, you can skip wandb, but the metric receiver script expects it. We’ll show how to disable metrics later.

----------

## Downloading and Placing LibTorch

LibTorch is PyTorch’s C++ library. You must download the **CPU** or **CUDA** version and place it inside the `GigaLearnCPP` subfolder.

### For CPU (no GPU)

1.  Go to the [Pytorch Homepage](https://pytorch.org/get-started/locally/)
    
2.  Select:
    
    -   PyTorch Build: Stable
        
    -   OS: Windows
        
    -   Package: LibTorch
        
    -   Language: C++
        
    -   Compute Platform: CPU
        
3.  Download the **CPU** version (e.g., `libtorch-win-shared-with-deps-2.5.1+cpu.zip`).
    
4.  Extract the zip file. Inside you will see a folder named `libtorch`.
    
5.  Copy that `libtorch` folder into:
    

Plaintext

```
GigaLearnCPP-Leak\GigaLearnCPP\libtorch
```

The final path should be `GigaLearnCPP-Leak\GigaLearnCPP\libtorch` with `bin`, `lib`, `include`, etc. inside.

### For CUDA (GPU)

If you installed CUDA, select **Compute Platform: CUDA 12.8** (or the version matching your CUDA installation). Download the corresponding zip and place it in the same location. The build process will automatically detect CUDA.

**Important:** The CMake configuration expects to find libtorch exactly at that path. Do not rename the folder.

----------

## Building GigaLearnCPP

Now we use CMake to generate the Visual Studio solution and build the executable, all from the command line.

### Step 1: Create a build folder

Inside the repository root (`GigaLearnCPP-Leak`), create a new folder for the build output:

```
mkdir build
cd build
```

### Step 2: Configure CMake

Run the following command **in one line** (replace the libtorch path if you placed it elsewhere). This example is for **CPU**; for CUDA, you can omit the `-DTorch_DIR` override (CMake will find CUDA automatically).

```
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=RelWithDebInfo -DTorch_DIR="C:/Users/YourUsername/RLBot/GigaLearnCPP-Leak/GigaLearnCPP/libtorch/share/cmake/Torch"
```

**Explanation:**

-   `-G "Visual Studio 17 2022"` – generates solution for VS2022.
    
-   `-A x64` – 64‑bit platform.
    
-   `-DCMAKE_BUILD_TYPE=RelWithDebInfo` – builds the project, in RelWithDebInfo.
    
-   `-DTorch_DIR=...` – points CMake to the TorchConfig.cmake file inside libtorch.
    

**Common error:** If CMake cannot find Python, you may need to set `-DPython_EXECUTABLE=...` to your Python executable path (the one inside your virtual environment). Example: `-DPython_EXECUTABLE="C:\Users\YourUsername\RLBot\GigaLearnCPP-Leak\venv\Scripts\python.exe"`

After running, you should see `-- Configuring done` and `-- Generating done`.
**VERY IMPORTANT: If you ever add a new file to gigalearn, such as a state setter, YOU MUST reconfigure cmake again. If you are just making an edit to a file, you just have to rebuild it.**

### Step 3: Build the executable

```
cmake --build . --config Release --target GigaLearnBot
```

This will compile all sources and produce `GigaLearnBot.exe` inside `build\RelWithDebInfo\`. The build time can be 3–10 minutes depending on your CPU.

**Note:** If you see warnings about `C4251`, they are benign and can be ignored.

----------

## Running Training

Before starting training, ensure you have a `checkpoints` folder where the bot will save its models. The executable will create it automatically if it doesn’t exist.

Navigate to the output folder:

```
cd RelWithDebInfo
```

Now run the training executable:

```
GigaLearnBot.exe
```

You will see console output showing the training progress, including:

-   Model parameter counts
    
-   Checkpoint save directory
    
-   PPO metrics (average step reward, policy entropy, etc.)
    
-   Steps per second
    

Training will continue until you press **Q** (uppercase) in the console – this will save a checkpoint and exit gracefully.

**Important:** The first few iterations will be slower because the environment is initialising. 

### Changing Hyperparameters

To adjust training parameters (batch size, learning rate, etc.), you must edit `src/ExampleMain.cpp` and rebuild. Look for the `cfg.ppo` block.

### Starting a Fresh Run

If you want to start training from scratch (discard previous checkpoints), either:

1.  Delete the `checkpoints` folder inside `build\RelWithDebInfo`, or
    
2.  Change the `cfg.checkpointFolder` string in `ExampleMain.cpp` to a new name (e.g., `"GGLRUN2"`) and rebuild.
    

----------

## Rendering the Bot

GigaLearnCPP includes a renderer that visualises the bot’s gameplay using **RocketSimVis**. You must first obtain the visualiser.

### Get RocketSimVis

Clone the RocketSimVis repository from:

```
git clone https://github.com/ZealanL/RocketSimVis.git
```

This folder can be placed **anywhere on your system** (e.g., `C:\RLBot\RocketSimVis`). It contains a `run.bat` script that launches the visualiser.

### Start the Visualiser

Open a **separate** command prompt. Navigate to the RocketSimVis folder and double click on the **run.bat** file.

```
run.bat
```

You will see a window opening. Keep it running.

### Run the Bot in Render Mode

Back in your `build\ReRelWithDebInfo` folder, run:

```
GigaLearnBot.exe --render
```

The bot will now load the **latest checkpoint** from the `checkpoints` folder and start a single game, sending frames to RocketSimVis. You can watch the bot play.

**Important:**

-   The `--render` flag disables training and uses a deterministic policy (no exploration).
    
-   Rendering and training cannot happen in the same process. You can have one terminal training (without `--render`) and another terminal rendering (with `--render`) – both can share the same `checkpoints` folder. The rendering instance will always load the latest saved policy.
    

----------
## Batch Files
-   You probably dont wish to start up training again every single time by opening up a new terminal, going to your enviorment, start up training, etc. 
    
-   Batch files allow you to just double click and automatically opens up a terminal, autoconfigures/builds, and runs training.
    
-   You can make a shortcut on your desktop to easily run it.
-----------
## Understanding Checkpoints and Fresh Runs

-   **Checkpoints** are saved automatically every `cfg.tsPerSave` timesteps (default 1,000,000) and when you press **Q**.
    
-   They are stored in `build\RelWithDebInfo\checkpoints\<timestep>\` as `POLICY.lt`, `CRITIC.lt`, `SHARED_HEAD.lt`, and `RUNNING_STATS.json`.
    
-   The bot always loads the **highest timestep** subfolder when started (or the one specified in code).
    

**To start a completely new training run:**

-   **Option A:** Delete the `checkpoints` folder(if the run absolutely sucks).
    
-   **Option B:** In `src/ExampleMain.cpp`, change `cfg.checkpointFolder = "checkpoints_new";` and rebuild.
    

**To resume training from a specific checkpoint:**

-   The bot automatically does that – just run normally.
    
-   If you want to force a specific checkpoint, uncomment the `checkpointPath` line in `rlbotmain.cpp` (if using RLBot) or modify `Learner::Load()` to load a custom path.
    

----------

## Metrics and Skill Tracker

GigaLearnCPP can log training metrics to **Weights & Biases** (wandb) for easy visualisation.

### Enabling wandb metrics

1.  Create a free account on [Wandb](https://wandb.ai/site/) and log in from your terminal:   

```
wandb login
```

2.  In `ExampleMain.cpp`, set `cfg.sendMetrics = true;` and optionally change `cfg.metricsProjectName`, `cfg.metricsGroupName`, `cfg.metricsRunName`.
    
3.  Rebuild and run. You will see a wandb link in the console.
    

**Note:** If you don’t want to use wandb, set `cfg.sendMetrics = false` – the bot will still train normally.
------------
### Skill Tracker (ELO rating)

The skill tracker evaluates the bot against older versions of itself and assigns an ELO‑like rating. To enable it:

C++

```
cfg.skillTracker.enabled = true;
cfg.skillTracker.numArenas = 8;        // adjust based on CPU threads
cfg.skillTracker.simTime = 45;         // seconds per evaluation game
cfg.skillTracker.updateInterval = 16;  // iterations between evaluations

```

The skill rating will appear in the wandb logs.

----------

## Common Errors and Troubleshooting

**1. CMake Error: Could not find Torch**

-   **Cause:** libtorch not placed correctly or `Torch_DIR` pointing to wrong folder.
    
-   **Solution:**
    
    -   Ensure `libtorch` is inside `GigaLearnCPP\libtorch` and that `TorchConfig.cmake` exists under `libtorch\share\cmake\Torch`.
        
    -   Double‑check the path in `-DTorch_DIR`.
        

**2. Python.h: No such file or directory or ModuleNotFoundError: No module named 'python_scripts'**

-   **Cause:** Python development headers not found, or the Python environment is not correctly set up.
    
-   **Solution:**
    
    -   Use a Python virtual environment as described. Activate it before running CMake.
        
    -   In CMake, explicitly set `-DPython_EXECUTABLE` to the full path of `python.exe` inside your virtual environment.
        
    -   Also set `-DPython_ROOT_DIR` to the virtual environment folder.
        

**3. SRE module mismatch or ModuleNotFoundError: No module named '_socket'**

-   **Cause:** Mixed Python versions (the bot is built against one version but runtime finds another).
    
-   **Solution:**
    
    -   Use a clean Python 3.11 environment (no other Python installations interfering).
        
    -   Ensure the Python DLLs are copied correctly. The CMake script automatically copies them, but if you still get errors, manually copy the `Lib` folder from your Python installation into the `RelWithDebInfo` folder.
        
    -   Alternatively, set `cfg.sendMetrics = false` and `cfg.renderMode = false` to disable Python altogether.
        

**4. ROCKETSIM FATAL ERROR: No arena meshes found for gamemode soccar**

-   **Cause:** Missing `collision_meshes` folder or wrong path.
    
-   **Solution:**
    
    -   Copy the `collision_meshes` folder into the same directory as `GigaLearnBot.exe` (i.e., `build\RelWithDebInfo`).
        
    -   Or, place it in the repository root and the build step will copy it (the `ExampleMain.cpp` uses `RocketSim::Init("collision_meshes")`, which is relative to the working directory).
        

**5. error C2676: binary '[': 'std::unordered_set<...>' does not define this operator**

-   **Cause:** You attempted to index `arena->_cars` as an array, but it is an unordered set.
    
-   **Solution:** Use iterators to loop through cars, as shown in the provided state setters.
  
**6. Bot runs but does nothing (always low rewards)**

-   **Cause:** Reward weights too low.
    
-   **Solution:**
        
    -   Increase the weight of the `TouchBallReward` or `VelocityPlayerToBallReward`  to see if the bot starts moving. **You need to start up training with a few rewards, not just goal reward.**
        

**7. Training is extremely slow (under 30k steps/sec)**

-   **Cause:** Too many parallel games (cfg.numGames) for your CPU.
    
-   **Solution:**
    
    -   Reduce `cfg.numGames` to 128 or 64. Monitor CPU usage in Task Manager.
      
    -   Don't use massive policy sizes. A shared head of `[512, 512, 512]`and a policy/critic size of `[256, 256, 256]` is good enough.
        
    -   Disable the skill tracker (cfg.skillTracker.enabled = false) as it uses extra CPU(this doesn't matter too much).
        
    -   Use GPU(Cuda) if available.
        

**8. Could not connect to server when rendering**

-   **Cause:** RocketSimVis not running, or port mismatch.
    
-   **Solution:**
    
    -   Start `run.bat` in the RocketSimVis folder first(make sure the run.bat is not placed somewhere else randomly apart from the RocketSimVis folder).
        
    -   Check that the port in `RenderSender.cpp` (default 9237) matches the port used by RocketSimVis (9237). No change needed.
--------------
**COMMON ERRORS**

-   **Cause:** Using different obs/acts and different policy sizes
    
-   **Solution:**
        
    -   Increase the weight of the `GoalReward` or `TouchBallReward` temporarily to see if the bot starts moving.
      
    -   Check the observation size printed at startup (Obs size: X) and the action size amount (Action size. X). It should match the size used during training. If you changed `DefaultObsPadded(3)` but trained with `AdvancedObs`, the bot will have an error.
      
    -   **If you are switching action parsers or observation builders, you better start a new training run.**
      
    -   **If you are switching policy or shared head sizes, you also better start a new training run.**
-------------------------------------------------------------------------------------------------------------------------------------------------------

## Final Notes and Resources

-   **GigaLearnCPP Public Archive:** [Official Github Repo](https://github.com/ZealanL/GigaLearnCPP-Leak)

-   **Gamma Calculator:** [GammaDesmos](https://www.desmos.com/calculator/oxplhwmy89)

-   **GigaLearnCPP Introduction Video** [Introduction On Gigalearn](https://www.youtube.com/watch?v=QprErElWT-E)
    
-   **RLGym Discord Server:** [RlGym Discord Server Invite Link](https://discord.gg/E6CDtwgP8F)
    
-   **YouTube Tutorial (RLGym v2):** [Rlgym v2 python tutorial](https://www.youtube.com/watch?v=_IbWTCQNsxE)
    
-   **Example RLGym v2 Bot (Python):** [Rlgym v2 python bot example](https://github.com/PiggyVevo/ExampleRocketLeagueBot)
    
-   **My YouTube Channel:** [My Youtybe Channel](https://www.youtube.com/@RrichardsWorld?sub_confirmation=1)

-   **DM me on discord for help:** `richardsworld` **Do not dm me unless you read the whole github word for word, like to the point of memorization. I can garuntee 95% of questions (which it is lmaoo) are already answered in the github. If you dm me about something clearly stated in the github, I'm instantly blocking you, unless it was vague in the github.**
    

### Final Advice

-   Always run the command prompt as a normal user – no admin rights are needed after installation.
    
-   Use the **virtual environment** every time you run training or rendering to keep dependencies isolated.

-   Replace the `ExampleMain.cpp` file in the [Gigalearn Repo](https://github.com/ZealanL/GigaLearnCPP-Leak/blob/main/src/ExampleMain.cpp) with the one in this github. It should be located in the `GigaLeanCPP-Leak/src` folder.
  
-   **If you encounter an error not listed, check the console output carefully. LEARN TO FRICKIN READ THE CONSOLE OUTPUT LOGS, 70%+ OF ERRORS ARE SELF EXPLANATORY**
    
-   For further help, join the [RlGym Discord](https://discord.gg/E6CDtwgP8F) and ask for help in the `#question` channel. (do not spam the `general` channel for help.)
    

**Happy training! :D**

# [SUBSCRIBE](https://www.youtube.com/@RrichardsWorld?sub_confirmation=1) To My Youtube Channel For More Tutorials! 🔥
