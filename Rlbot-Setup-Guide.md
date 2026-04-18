# Porting Your Trained Bot to RLBot 

Once you have a trained model (`.lt` files), you can let your bot play in actual Rocket League matches using the **RLBot** framework. The **GGLBot** repository provides a lightweight, inference‑only executable that loads your model and communicates with RLBot.

> **Note**: The GGLBot executable runs in **deterministic mode** by default (no exploration). This is ideal for playing.

----------

## Step 1: Clone the GGLBot Repository/Add RLBOT
1. Download the newest .exe files for the `Rlbot GUI` and the `Rlbot Server` from here: https://github.com/RLBot/gui/releases & https://github.com/RLBot/core/releases.
Double click both of the `.exe` files to start up training.
3. Open a command prompt and clone the repository **recursively**:

```cmd
git clone https://github.com/SubparN0va/GGLBot.git --recurse-submodules
cd GGLBot
```
----------

## Step 2: Copy Your Trained Model Files

Your trained models are the `.lt` files produced by GigaLearnCPP. They are located inside the `checkpoints` folder (e.g., `build\Release\checkpoints\12345678\`). Copy them into the `GGLBot\rlbot\` folder.

Specifically, you need:

-   `POLICY.lt`
    
-   `SHARED_HEAD.lt` (if you used a shared head)

`CRITIC.lt` is not needed to port your bot into **rlbot**, it is only used for training.
    

Example:

```cmd
mkdir rlbot
copy "C:\path\to\your\training\checkpoints\12345678\POLICY.lt" rlbot\
copy "C:\path\to\your\training\checkpoints\12345678\SHARED_HEAD.lt" rlbot\
```
> **Important**: The file names must be exactly `POLICY.lt` and `SHARED_HEAD.lt` (uppercase). If your shared head model has a different name(which it shouldn't), rename it.

----------

## Step 3: Edit `RLBotMain.cpp` to Match Your Training Configuration

Open `GGLBot\src\RLBotMain.cpp`. You will see a section near the top where you must set:

-   Observation builder (must match the one you used for training)
    
-   Action parser (must match)
    
-   Model architecture (shared head and policy layer sizes, activation, layer norm)
    
-   Observation size (`obsSize`)
    

**Example configuration** (adjust to your training settings):

```c++

// ------------------------------------------------------------------------
// Set the following to match the configuration your model was trained with
// ------------------------------------------------------------------------
ctx->obs = std::make_shared<RLGC::AdvancedObs>();   // or DefaultObsPadded(3) if you used padded
ctx->act = std::make_shared<RLGC::DefaultAction>();
ctx->params.tickSkip = 8;
ctx->params.actionDelay = ctx->params.tickSkip - 1;
int obsSize = 109;   // check your training logs for the exact size
// Shared head config (if you used shared head)
GGL::InferPartialModelConfig sharedHeadCfg;
sharedHeadCfg.layerSizes = { 512, 512, 512 };    // must match training
sharedHeadCfg.addLayerNorm = true;
sharedHeadCfg.activationType = GGL::ModelActivationType::LEAKY_RELU;   // or RELU
sharedHeadCfg.addOutputLayer = false;       // always false for shared head
// Policy config
GGL::InferPartialModelConfig policyCfg;
policyCfg.layerSizes = { 256, 256, 256 };   // must match training
policyCfg.addLayerNorm = true;
policyCfg.activationType = GGL::ModelActivationType::LEAKY_RELU;
policyCfg.addOutputLayer = true;            // policy outputs actions
// Force CPU (set to true if you have a GPU and trained with GPU)
bool useGPU = false;
```
> **How to find `obsSize`**: When you run training, the console prints `Obs size: XXX` at startup. Use that number.

> **Layer sizes**: Look at your `ExampleMain.cpp` where you defined `cfg.ppo.sharedHead.layerSizes` and `cfg.ppo.policy.layerSizes`. Copy exactly.

----------

## Step 4: (Optional) Hardcode a Specific Checkpoint

By default, the bot loads the latest checkpoint from the `rlbot\checkpoints` folder. If you want to force a specific model, uncomment the `checkpointPath` line and set the full path to your `POLICY.lt` file.

```cpp
// checkpointPath = "C:/GGLBot/rlbot/POLICY.lt";
```
Use forward slashes (`/`) or double backslashes (`\\`).

----------

## Step 5: Build the GGLBot Executable

Inside the `GGLBot` folder, create a build directory and configure CMake. The project expects libtorch in a default location (`%LOCALAPPDATA%\RLBot5\bots\libtorch_cpu`). We’ll set that up first.

### 5.1 Place libtorch in the expected location

Copy your libtorch folder (the same one you used for training) to:

```cmd
%LOCALAPPDATA%\RLBot5\bots\libtorch_cpu
```
For example:

```cmd

xcopy /E /I "C:\Users\YourUsername\RLBot\GigaLearnCPP-Leak\GigaLearnCPP\libtorch" "%LOCALAPPDATA%\RLBot5\bots\libtorch_cpu"
```
**Note if you are using cuda, it might be libtorch_cuda.**
If you prefer a different location, you can override `LIBTORCH_ROOT` during CMake configuration.

### 5.2 Build

```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release --target GGLBot
```
After a successful build, the executable `GGLBot.exe` will be automatically copied to the `rlbot` folder.

----------

## Step 6: Prepare the RLBot Folder

Your `rlbot` folder should now contain:

-   `GGLBot.exe`
    
-   `POLICY.lt` and `SHARED_HEAD.lt`
    
-   `bot.toml` (already in the repository)
    
-   `loadout.toml`
    
-   `run.bat` (sets the PATH to libtorch and launches the exe)
    

If any of the `.toml` files are missing, copy them from the root of the GGLBot repository.

Edit `bot.toml` to change the bot’s name and other metadata if desired. The `run_command` is already set to `run.bat`.

----------

## Step 7: Add the Bot to RLBot

1.  Launch **RLBot v5** (the GUI).
    
2.  Click **“Add Bot”** → **“Add from directory”**.
    
3.  Select the `rlbot` folder (the one containing `bot.toml`).
    
4.  The bot will appear in the list. You can now start a match.
    

The bot will connect automatically and play using your trained policy.

> **Note**: The bot runs in **deterministic** mode (always chooses the action with the highest probability). This is the default and cannot be changed without recompiling.

----------

## Common Issues

### ❌ Bot doesn’t appear in RLBot

-   Check that `cpp_executable_path` in `rlbot\CppPythonAgent.cfg` points to the correct `GGLBot.exe`.
    
-   Ensure `run.bat` is present and that `bot.toml` references it correctly.
    

### ❌ `Could not connect to server`

-   The port in `RLBotMain.cpp` must match the port in `rlbot\port.cfg` (default 23234).
  
-   **YOU MUST LUANCH THE SERVER ALONG WITH THE GUI**
      

### ❌ `Error: No valid checkpoint path found`

-   The bot cannot find `POLICY.lt` or the `SHARED_HEAD.lt`. Place the `.lt` files directly in the `rlbot` folder, or create a `checkpoints` subfolder with the appropriate timestamp subfolder.
    

### ❌ `libtorch.dll not found`

-   The `run.bat` script sets the `PATH` to `%LOCALAPPDATA%\RLBot5\bots\libtorch_cpu\lib`. Verify that folder exists and contains `torch.dll`, `c10.dll`, etc.
    
-   Alternatively, copy all DLLs from your libtorch `lib` folder into the `rlbot` folder.
    

### ❌ Observation size mismatch

-   The bot will crash with an error about `obs size`. Double‑check `obsSize` in `RLBotMain.cpp` – it must exactly match the size used during training.
    

### ❌ Model loading error (parameter size mismatch)

-   Your layer sizes, activation type, or layer norm settings in `RLBotMain.cpp` do not match the checkpoint. Compare with your `ExampleMain.cpp` and correct.
    
## YOU MUST USE THE SAME ACTION PARSERS/OBSERVATION BUILDERS YOU USED DURING TRAINING, OTHERWISE IT WILL CRASH. 
----------

## Summary

1.  **Train** your bot with GigaLearnCPP.
    
2.  **Copy** the `.lt` files to the GGLBot `rlbot` folder.
    
3.  **Configure**  `RLBotMain.cpp` with your exact training settings.
    
4.  **Build** GGLBot (ensure libtorch is in the expected location).
    
5.  **Add** the `rlbot` folder to RLBot, open up the **Rlbot GUI** and the **Rlbot Server**, click add bot, and add the `bot.toml` file.
    

Your bot will now compete in Rocket League matches using the same policy you trained.
