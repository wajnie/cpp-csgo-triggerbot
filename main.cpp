// READ README.md FIRST BEFORE YOU WILL DO SOMETHING WITH IT




#include "ProcMem.h" // Reads memory
ProcMem Mem; // That's the shortcut
Mem.Process("csgo.exe"); // Chooses our process
DWORD ClientDLL = Mem.Module("client.dll"); // Creates a module we are reading memory from

// HAS to be updated when counter strike is updated.
const DWORD playerBase = 0xA68A14;
const DWORD entityBase = 0x4A0B0C4;
const DWORD crosshairOffset = 0x23F8;
// Doesn't require updating
const DWORD teamOffset = 0xF0;
const DWORD healthOffset = 0xFC;
const DWORD EntLoopDist = 0x10;
// As i said, LocalPlayer (You)
DWORD LocalPlayer = Mem.Read<DWORD>(ClientDLL + PlayerBase);
// That is our teammates
int LocalTeam = Mem.Read<int>(LocalPlayer + teamOffset);
// That's our crosshair ID, we will use it for reading what's "in our crosshair"
int CrossHairID = Mem.Read<int>(LocalPlayer + CrosshairOffset);

void Bot()
{
    DWORD EnemyInCH = Mem.Read<DWORD>(ClientDLL + EntityBase + ((CrossHairID - 1) * EntLoopDist));
    int EnemyHealth = Mem.Read<int>(EnemyInCH + healthOffset);
    int EnemyTeam = Mem.Read<int>(EnemyInCH + teamOffset);
    if (LocalTeam != EnemyTeam && EnemyHealth > 0) {
        // Add a little delay yourself by Sleep()
        mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
        // You can use Sleep() here too, that line is made for autos like ak47. Not useful with pistol
        mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);
        // Now you can make a cooldown beetween shots using Sleep() again.
    }

    int main()
    {
        while (true) {
            Bot();
            // Add a Sleep() here if you have potato pc like me
        }
    }