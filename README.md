Depth Frame Action Collection
=============================
*A plugin for FrandTheft Auto V that captures game depth frames as characters perform certain actions*

<img src="/Samples/clap_rgb.gif">
<img src="/Samples/clap_depth.gif">

## Requirements
* [Grand Theft Auto V](https://store.steampowered.com/app/271590/Grand_Theft_Auto_V/)
* [ScriptHookV by Alexander Blade](http://www.dev-c.com/gtav/scripthookv/) 
* [GTA Vision Export (Native)](https://github.com/umautobots/GTAVisionExport)
* [OPTIONAL (allows for further control of in-game world/settings): Enhanced Native Trainer for GTA V](https://www.gta5-mods.com/scripts/enhanced-native-trainer-zemanez-and-others)

## Installation
1. Install [ScriptHookV](http://www.dev-c.com/gtav/scripthookv/) following its accompanying instructions by placing the appropriate files in your game directory (the folder which contains GTA5.exe).
2. Put the contents of */SaveGame* into *Documents/Rockstar Games/GTA V/Profiles/your profile numer/*. This effectively gives you full access to the entire map and every location without having to naturally progress through the game.
3. Pull the [GTA Vision Export](https://github.com/umautobots/GTAVisionExport) repo and follow directions to build the *native* version of the project.  Make sure that you copy both `GTAVisionNative.asi` and `GTAVisionNative.lib` are in the game directory. 
4. Create an environment variable **(GTAVisionLib)** pointing to the build path of `GTAVisionNative.lib`. Using either the one you copied into the game directory or the one found under *GTAVisionExport/native/build/src/Release/* should work (I used the latter to ensure that I never move the library).
5. Copy `ActionCollection.asi` from *ActionCollection/bin/Release* to your game directory. 
6. Copy the directory *ActionCollection/bin/Release/Action_Collection* to your game directory. This folder contains all the files needed to initialize the plugin and the log file. 

## Adding Actions
Navigate to your newly copied *your game directory/Action_Collection* folder and open `actions.json`. Here you can manually add any new actions you want the character to perform and collect data on. List of known and stable actions can be found [here](https://pastebin.com/6mrYTdQv) and can be visualized through a YouTube video [here](https://youtu.be/zO9BwwrsytE). The JSON is formatted as follows:
```json
{
    "WORLD_HUMAN_DRINKING": {
        "duration": 15000,
        "sample_rate": 1000
    },
    "WORLD_HUMAN_CHEERING": {
        "duration": 15000,
        "sample_rate": 1000
    },
    "WORLD_HUMAN_STAND_MOBILE": {
        "duration": 15000,
        "sample_rate": 1000
    },
    "WORLD_HUMAN_TOURIST_MOBILE": {
        "duration": 15000,
        "sample_rate": 1000
    }
}
```
The title of the object specifies the in-game name of the action, taken from [this list](https://pastebin.com/6mrYTdQv). The "duration" field specifies how long the action will be performed where 1000 is ~1 second (so effectively in milliseconds). In the above example, each action is performed for 15 seconds. The "sample_rate" specifies how often you want to capture a depth frame. In this example, we are capturing a depth frame every second throughout the 15 seconds the action plays. 

***IMPORTANT***: If you add an action, you *must* create a corresponding folder of the same name in the *your game directory/Action_Collection/Depth_Samples/* folder. This is where the depth frames are stored during data collection. 

## Adding Models
Adding a new character model is fairly straightfoward. In the *your game directory/Action_Collection* folder you can find `models.json`. You can add a model by adding its corresponding hash (list of known models/hashes [here](https://wiki.gtanet.work/index.php/Peds)) to the JSON array". JSON is formatted as follows:
```json
{
    "models":[
        1074457665, 
        226559113,
        357551935, 
        1189322339,
        1567728751, 
        1644266841,
        -815646164,
        797459875,
        -198252413,
        -636391810,
        1250841910, 
        600300561,
        1633872967, 
        -912318012
    ]
}
```

## Adding a Location
Adding a location for a character to perform an action in requires you to run the game with the plugin loaded. In game, go to the in-game location you would like the character to perform an action in. Press **F7** and your location should be recorded and automatically be added to the `your game directory/Action_Collection/locations.txt`.

***NOTE***: If you installed the optional *Enhanced Native Trainer for GTA V*, function key presses between this plugin (ActionCollection.asi) and the trainer plugin (EnhancedNativeTrainer.asi) may overlap. Remember that you can always change the keys used by Enhanced Native Trainer by adjusting their `ent-config.xml` file. 

## Changing the Range of Data Collection
In *your game directory/Action_Collection/Depth_Samples/* you can find `start.cfg`. This file tells the plugin where to start and end its data collection. "loc_start" and "loc_end" fields tell the plugin where in the list of locations from `locations.txt` you would like to start and end data collection respectively (0 index based). Similarly, "model_start" and "model_end" tell the plugin which model you would like to start and end you data collection respectively (also 0 index based). Useful if you only want to collect data from a subset of your specified models or locations. 

## Using the Plugin
Finally! Now that you've gone through the installation process and changed the settings files to your liking, you can move onto actually using the plugin! Once you have everything set-up, data collection is fairely straightfoward. In-game, press **F6** to start your data collection and the script should automatically take over and change the models and actions iteratively. 

## Finding and understanding the the data
You can find your data in *your game directory/Action_Collection/Depth_Samples/* where the depth frames are sorted into the folder for each action respectively. The naming convention of the frames can be explained through an example: `loc_0_-198252413_frame_3` where "loc_0" represents the location where the action was performed (according to the location file) and "-198252413" represents the hash used to spawn the model taken from the `models.json` file and "frame_3" represents where the is in the action sequence. Frames of the same action sequence will always have the same location and model hash. 

***NOTE***: Included is also a small python script, `frames_to_gif.py` that you can use to visualize your .raw files. Script by default makes a gif out a specified location and action but easily flexible to visualize whatever you need it to visualize. 

## In-Game Settings
This plugin was developed and tested on a PC of the following specifications:
* CPU: AMD Ryzen 5 1600 6-core processor
* GPU: NVIDIA GeForce RTX 2070
* Memory: 16 GB
* OS: Windows 10

I also used to the following in game graphics settings, which seemingly provided the best performance in my case: 
* Screen Type: Windowed
* Resolution: 1920 x 1080
* FXAA: off
* MSAA: off 
* Vsync: off

Open issues in the [GTA Vision Export](https://github.com/umautobots/GTAVisionExport) show that that these settings must be as follows in order for that library to work, so play with these settings at your own risk. 
