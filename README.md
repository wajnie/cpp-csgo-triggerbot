# Introduction
Triggerbot for Counter-Strike Global Offensive. Here I am going to show you the basics of making a mini-cheat for csgo. Just a simple triggerbot. Install Visual Studio or some other good editor.
# WARNING!
**Do not use it to cheat in csgo! It will get you instantly banned! This tutorial is for educational purposes***
# 1. Setup
To make a triggerbot first you have to install and use [ProcMem](https://www.unknowncheats.me/forum/downloads.php?do=file&id=13249).

First, create your `main.cpp` file and add `ProcMem.cpp`, `ProcMem.h` from the downloaded .rar file. You do this in Visual Studio by **right clicking source files** -> Add -> Existing item and choosing `ProcMem.cpp`. Then you do exactly the same but in Header Files with `ProcMem.h`.
Next we need to initialize ProcMem in our main file (`main.cpp`) so we can use functions in it. Easy, huh?
That's how it looks like in code:
```cpp
#include "ProcMem.h" // Reads memory
ProcMem Mem; // That's the shortcut
```
But to actually read our proccess's memory (`csgo`) we have to choose a process
```cpp
Mem.Process("csgo.exe"); // Chooses our process
```
After we did it we can read memory from it but reading more advanced things like `PlayerBase` we need to get the `client.dll`
```cpp
DWORD ClientDLL = Mem.Module("client.dll"); // Creates a module we are reading memory from
```
Now the most boring part, which is offsets of our crosshair and other things like health blah blah...
**Has to be updated when csgo updates. I was making this triggerbot with the CS Warzone version, so change the offsets. Just google it**
```cpp
// HAS to be updated when counter strike is updated.
const DWORD playerBase = 0xA68A14;
const DWORD entityBase = 0x4A0B0C4;
const DWORD crosshairOffset = 0x23F8;
// Doesn't require updating
const DWORD teamOffset = 0xF0;
const DWORD healthOffset = 0xFC;
const DWORD EntLoopDist = 0x10;
```
# 2. Making a Triggerbot

So now we to have get information about yourself. You are the LocalPlayer. To get information about LocalPlayer we have to read `client.dll`. `client.dll` has most information about us and our enemies/teammates.
```cpp
// As i said, LocalPlayer (You)
DWORD LocalPlayer = Mem.Read<DWORD>(ClientDLL + PlayerBase);
// That is our teammates
int LocalTeam = Mem.Read<int>(LocalPlayer + teamOffset);
// That's our crosshair ID, we will use it for reading what's "in our crosshair"
int CrossHairID = Mem.Read<int>(LocalPlayer + CrosshairOffset);
```
After that we have to create a Triggerbot function. I will make it shorter and just name it Bot. We have to read memory which is required to make the triggerbot work
```cpp
void Bot()
{
    DWORD EnemyInCH = Mem.Read<DWORD>(ClientDLL + EntityBase + ((CrossHairID - 1) * EntLoopDist)); // CH is just crosshair btw
    int EnemyHealth = Mem.Read<int>(EnemyInCH + healthOffset); // Enemy in our crosshair
    int EnemyTeam = Mem.Read<int>(EnemyInCH + teamOffset); // Enemy in crosshair's team, we need it to distinguish are we aiming at the enemy or the teammate does. Logic.
} 
```
Now for the `if`s. To don't make it shoot at our "friends" here is the first one
```cpp
if (LocalTeam != EnemyTeam)
{
    // left click or just shoot lol
}
```
But notice that our triggerbot is gonna shoot death bodies/enemies with 0 health. Here is how we can do it. We are just gonna check is EnemyHealth higher than 0 if is then shoot. We will get to the shooting later.
```cpp
if (EnemyHealth > 0)
{
    // left click or just shoot lol
}
```
For the gods of c++ we will make the code "prettier"
```cpp
if (LocalTeam != EnemyTeam && EnemyHealth > 0)
{
    // shoot
}
```
That's *almost* everything. **BUT WAIT!** It does not shoot! Because it's only memory reading triggerbot. Now we are gonna make it click/shoot instead of forcing it through writing memory. We will be using `mouse_event`. Check mouse_event on [MSDN](https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-mouse_event) and just try to get it. I get it after watching a tutorial. But anyways, there is the full code. **NOTE** If you do not understand something go back and try again.
```cp
void Bot()
{
    DWORD EnemyInCH = Mem.Read<DWORD>(ClientDLL + EntityBase + ((CrossHairID - 1) * EntLoopDist));
    int EnemyHealth = Mem.Read<int>(EnemyInCH + healthOffset);
    int EnemyTeam = Mem.Read<int>(EnemyInCH + teamOffset);
    if (LocalTeam != EnemyTeam && EnemyHealth > 0)
    {
    // Add a little delay yourself by Sleep()
    mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
    // You can use Sleep() here too, that line is made for autos like ak47. Not useful with pistol
    mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);
    // Now you can make a cooldown beetween shots using Sleep() again.
    }
```
Now our final part, just add the triggerbot to your main in a loop
```cpp
int main()
{
    while(true)
    {
        Trigger();
        // Add a Sleep() here if you have potato pc like me
    }
}
```
