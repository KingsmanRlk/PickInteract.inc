#include <a_samp>
#include <streamer>
#include <sscanf2>
#include <YSI_Data\y_iterate>
#include <YSI_Coding\y_hooks>

#include <PickInteract>

new g_HospitalPickupIndex;

public OnGameModeInit()
{
    // Pickup Amarelo de Informação (Tecla F - Vermelho Padrão)
    AddPickupInteract(1239, 1172.0, -1323.0, 15.4, "F");

    // Pickup de Entrada em HQ (Tecla E - Azul)
    g_HospitalPickupIndex = AddPickupInteract(1240, 2033.1, -1412.3, 17.2, "E", "#3399FF", 2.5);

    // Pickup de Cofre (Tecla N - Verde) em Interior/VirtualWorld específico
    AddPickupInteract(1274, 225.0, -84.0, 1005.2, "N", "#2ECC71", 1.5, .worldid = 1, .interiorid = 6);

    return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if ((newkeys & KEY_SECONDARY_ATTACK) && IsPlayerInAnyInteractArea(playerid))
    {
        SendClientMessage(playerid, -1, "[WHATNOT]: Você acionou a interação do pickup!");
    }
    return 1;
}